"""Mengelola grup subprocess ROS 2 untuk satu gate."""

import os
import signal
import subprocess
import threading
from pathlib import Path
from typing import Dict, Optional

from .gate_config import GateConfig


class GateProcess:
    """
    Spawn dan monitor semua node ROS 2 untuk satu gate.

    Konvensi topik per gate (gate_id = "gate_1"):
      Kamera  : /gate_1/cam/color/image_raw
                /gate_1/cam/aligned_depth_to_color/image_raw
                /gate_1/cam/aligned_depth_to_color/camera_info
      Detektor: /gate_1/vehicle/bounding_boxes
      Estimasi: /gate_1/vehicle/dimension_info
      Tarif   : /gate_1/vehicle/tariff_info
    """

    _NODE_ORDER = ["realsense", "detector", "estimator", "tariff", "data_logger"]

    def __init__(self, config: GateConfig, workspace: Path, setup_script: Path) -> None:
        self._cfg       = config
        self._workspace = workspace
        self._setup     = setup_script
        self._procs: Dict[str, subprocess.Popen] = {}
        self._lock      = threading.Lock()

    # ── Public ────────────────────────────────────────────────────────────
    def start(self) -> bool:
        """Spawn semua node. Return True jika semua berhasil."""
        commands = self._build_commands()
        with self._lock:
            for name in self._NODE_ORDER:
                cmd = commands[name]
                try:
                    proc = subprocess.Popen(
                        # Tidak pakai shell=True — cegah shell injection
                        ["bash", "-lc", f"source {self._setup} && {cmd}"],
                        cwd=str(self._workspace),
                        stdout=subprocess.DEVNULL,
                        stderr=subprocess.PIPE,
                        env=self._env(),
                    )
                    self._procs[name] = proc
                except OSError as exc:
                    self._kill_all_locked()
                    return False
        return True

    def stop(self) -> None:
        """Kirim SIGINT ke semua proses, fallback ke SIGTERM/SIGKILL."""
        with self._lock:
            self._kill_all_locked()

    def restart_dead(self) -> None:
        """Restart proses yang sudah mati tanpa menghentikan yang masih hidup."""
        commands = self._build_commands()
        with self._lock:
            for name in self._NODE_ORDER:
                proc = self._procs.get(name)
                if proc is not None and proc.poll() is not None:
                    try:
                        new_proc = subprocess.Popen(
                            ["bash", "-lc", f"source {self._setup} && {commands[name]}"],
                            cwd=str(self._workspace),
                            stdout=subprocess.DEVNULL,
                            stderr=subprocess.PIPE,
                            env=self._env(),
                        )
                        self._procs[name] = new_proc
                    except OSError:
                        pass

    def health(self) -> Dict[str, bool]:
        """Return dict {node_name: is_alive}."""
        with self._lock:
            return {name: (self._procs.get(name) is not None and
                           self._procs[name].poll() is None)
                    for name in self._NODE_ORDER}

    def is_any_alive(self) -> bool:
        with self._lock:
            return any(p.poll() is None for p in self._procs.values())

    # ── Internal ──────────────────────────────────────────────────────────
    def _env(self) -> dict:
        """Env saat ini + SMARTPORT_GATE_ID untuk node yang butuh."""
        env = os.environ.copy()
        env["SMARTPORT_GATE_ID"] = self._cfg.id
        return env

    def _build_commands(self) -> Dict[str, str]:
        gid    = self._cfg.id
        serial = self._cfg.camera_serial
        ns     = f"/{gid}"
        params = self._cfg.params
        cfg    = str(self._workspace / "config" / "smartport.yaml")

        # Namespace kamera: /gate_1/cam/...
        cam_ns   = gid
        cam_name = "cam"
        cam_base = f"/{cam_ns}/{cam_name}"

        return {
            # ── RealSense driver ──────────────────────────────────────
            "realsense": (
                f"ros2 launch realsense2_camera rs_launch.py"
                f" serial_no:=_{serial}"
                f" camera_namespace:={cam_ns}"
                f" camera_name:={cam_name}"
                f" align_depth.enable:=true"
                f" enable_sync:=true"
                f" enable_infra1:=false"
                f" enable_infra2:=false"
                f" pointcloud.enable:=false"
            ),

            # ── YOLOv8 Detector ───────────────────────────────────────
            "detector": (
                f"ros2 run yolov8_detector yolov8_detector --ros-args"
                f" -r __ns:={ns}"
                f" -p image_topic:={cam_base}/color/image_raw"
                f" -p confidence_threshold:={params.confidence_threshold}"
                f" -p max_detections:={params.max_detections}"
                f" -p publish_annotated_image:=true"
                f" --params-file {cfg}"
            ),

            # ── Dimension Estimator ───────────────────────────────────
            "estimator": (
                f"ros2 run dimension_estimator dimension_estimator --ros-args"
                f" -r __ns:={ns}"
                f" -p bbox_topic:={ns}/vehicle/bounding_boxes"
                f" -p depth_image_topic:={cam_base}/aligned_depth_to_color/image_raw"
                f" -p depth_camera_info_topic:={cam_base}/aligned_depth_to_color/camera_info"
                f" -p min_valid_points:={params.min_valid_points}"
                f" --params-file {cfg}"
            ),

            # ── Tariff Calculator ─────────────────────────────────────
            "tariff": (
                f"ros2 run tariff_calculator tariff_calculator --ros-args"
                f" -r __ns:={ns}"
                f" --params-file {cfg}"
            ),

            # ── Data Logger ───────────────────────────────────────────
            "data_logger": (
                f"ros2 run data_logger data_logger --ros-args"
                f" -r __ns:={ns}"
                f" -p gate_id:={gid}"
                f" --params-file {cfg}"
            ),
        }

    def _kill_all_locked(self) -> None:
        """Hentikan semua proses (harus dipanggil dengan _lock dipegang)."""
        for name in reversed(self._NODE_ORDER):
            proc = self._procs.get(name)
            if proc is None:
                continue
            if proc.poll() is None:
                try:
                    proc.send_signal(signal.SIGINT)
                    proc.wait(timeout=5.0)
                except subprocess.TimeoutExpired:
                    proc.terminate()
                    try:
                        proc.wait(timeout=3.0)
                    except subprocess.TimeoutExpired:
                        proc.kill()
                except OSError:
                    pass
        self._procs.clear()
