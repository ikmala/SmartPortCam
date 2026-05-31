#!/usr/bin/env python3
"""
CLI utama SmartPortCam.

Mode:
  deploy  — jalankan gate_manager + camera_manager (multi-gate, baca config/gates.json)
  train   — rekam dataset dengan RealSense + rosbag
  single  — jalankan satu gate tanpa gate_manager (compat mode, untuk testing)
"""

from __future__ import annotations

import argparse
import datetime as dt
import os
import shlex
import signal
import subprocess
import sys
import time
from pathlib import Path
from typing import List, Optional, Sequence, Tuple

WORKSPACE_ROOT   = Path(__file__).resolve().parents[1]
SETUP_SCRIPT     = WORKSPACE_ROOT / "install" / "setup.bash"
DEFAULT_PARAMS   = WORKSPACE_ROOT / "config" / "smartport.yaml"
GATES_CONFIG     = WORKSPACE_ROOT / "config" / "gates.json"

REALSENSE_CMD = (
    "ros2 launch realsense2_camera rs_launch.py"
    " align_depth.enable:=true enable_sync:=true"
    " enable_infra1:=false enable_infra2:=false"
    " pointcloud.enable:=false"
)


# ══════════════════════════════════════════════════════════════════════════
# Process manager
# ══════════════════════════════════════════════════════════════════════════
class ProcessManager:
    """Lacak dan hentikan subprocess secara graceful."""

    def __init__(self) -> None:
        self._procs: List[Tuple[str, subprocess.Popen]] = []
        self._stopping = False

    def launch(self, label: str, command: str, env: Optional[dict] = None) -> None:
        if not SETUP_SCRIPT.exists():
            raise RuntimeError(
                f"Setup script '{SETUP_SCRIPT}' tidak ditemukan. "
                "Jalankan 'colcon build' terlebih dahulu."
            )
        merged_env = {**os.environ, **(env or {})}
        proc = subprocess.Popen(
            ["bash", "-lc", f"source {shlex.quote(str(SETUP_SCRIPT))} && {command}"],
            cwd=str(WORKSPACE_ROOT),
            env=merged_env,
        )
        self._procs.append((label, proc))
        print(f"  [+] {label} (PID={proc.pid})")

    def wait(self) -> None:
        try:
            while True:
                alive = [p for _, p in self._procs if p.poll() is None]
                if not alive:
                    print("Semua proses selesai.")
                    break
                time.sleep(1.0)
        except KeyboardInterrupt:
            print("\nCtrl+C — menghentikan semua proses…")
        finally:
            self.shutdown()

    def shutdown(self) -> None:
        if self._stopping:
            return
        self._stopping = True
        for label, proc in reversed(self._procs):
            if proc.poll() is None:
                print(f"  [-] Menghentikan {label} (PID={proc.pid})")
                proc.send_signal(signal.SIGINT)
                try:
                    proc.wait(timeout=5.0)
                except subprocess.TimeoutExpired:
                    proc.terminate()
                    try:
                        proc.wait(timeout=3.0)
                    except subprocess.TimeoutExpired:
                        proc.kill()
        self._procs.clear()


# ══════════════════════════════════════════════════════════════════════════
# Mode: deploy (multi-gate via gate_manager)
# ══════════════════════════════════════════════════════════════════════════
def run_deploy_mode(args: argparse.Namespace) -> None:
    """
    Jalankan arsitektur multi-gate penuh:
      1. gate_manager  — baca gates.json, spawn semua gate aktif
      2. camera_manager — deteksi kamera RealSense
      3. (opsional) rosbridge + web_video_server
    """
    _ensure_gates_file()

    params = f"--params-file {shlex.quote(str(DEFAULT_PARAMS))}" \
        if DEFAULT_PARAMS.exists() else ""

    mgr = ProcessManager()
    print("\n=== SmartPortCam — Deploy Mode (Multi-Gate) ===")

    print("\n[1/2] Memulai core services…")
    mgr.launch(
        "GateManager",
        f"ros2 run gate_manager gate_manager --ros-args {params}",
    )
    mgr.launch(
        "CameraManager",
        f"ros2 run camera_manager camera_manager --ros-args {params}",
    )

    if args.with_rosbridge:
        print("\n[2/2] Memulai bridge & video server…")
        mgr.launch(
            "ROSBridge",
            "ros2 launch rosbridge_server rosbridge_websocket_launch.xml",
        )
        mgr.launch(
            "WebVideoServer",
            "ros2 run web_video_server web_video_server",
        )

    print("\nSistem berjalan. Tambah/kelola gate via:")
    print(f"  ros2 service call /smartport/gates/add ...")
    print(f"  Atau lewat web dashboard → Pengaturan → Tambah Gate")
    print(f"\nGates config: {GATES_CONFIG}")
    print("Tekan Ctrl+C untuk menghentikan semua.\n")
    mgr.wait()


# ══════════════════════════════════════════════════════════════════════════
# Mode: single (satu gate tanpa gate_manager — untuk testing)
# ══════════════════════════════════════════════════════════════════════════
def run_single_mode(args: argparse.Namespace) -> None:
    """
    Jalankan satu gate secara langsung (tanpa gate_manager).
    Berguna saat testing node individual atau deployment sangat sederhana.
    """
    params = f"--params-file {shlex.quote(str(DEFAULT_PARAMS))}" \
        if DEFAULT_PARAMS.exists() else ""

    gate_id  = args.gate_id
    serial   = args.camera_serial
    ns       = f"/{gate_id}"
    cam_base = f"/{gate_id}/cam"

    mgr = ProcessManager()
    print(f"\n=== SmartPortCam — Single Gate: {gate_id} ===")

    # Kamera
    mgr.launch(
        f"RealSense[{gate_id}]",
        f"ros2 launch realsense2_camera rs_launch.py"
        f" serial_no:=_{serial}"
        f" camera_namespace:={gate_id}"
        f" camera_name:=cam"
        f" align_depth.enable:=true enable_sync:=true"
        f" enable_infra1:=false enable_infra2:=false"
        f" pointcloud.enable:=false",
    )

    # Detector
    mgr.launch(
        f"Detector[{gate_id}]",
        f"ros2 run yolov8_detector yolov8_detector --ros-args"
        f" -r __ns:={ns}"
        f" -p model_path:={shlex.quote(args.model_path)}"
        f" -p image_topic:={cam_base}/color/image_raw"
        f" -p confidence_threshold:={args.confidence}"
        f" -p max_detections:={args.max_detections}"
        f" -p publish_annotated_image:=true"
        f" {params}",
    )

    # Estimator
    mgr.launch(
        f"Estimator[{gate_id}]",
        f"ros2 run dimension_estimator dimension_estimator --ros-args"
        f" -r __ns:={ns}"
        f" -p bbox_topic:={ns}/vehicle/bounding_boxes"
        f" -p depth_image_topic:={cam_base}/aligned_depth_to_color/image_raw"
        f" -p depth_camera_info_topic:={cam_base}/aligned_depth_to_color/camera_info"
        f" -p min_valid_points:={args.min_valid_points}"
        f" {params}",
    )

    # Tariff
    mgr.launch(
        f"Tariff[{gate_id}]",
        f"ros2 run tariff_calculator tariff_calculator --ros-args"
        f" -r __ns:={ns} {params}",
    )

    # Data Logger
    if args.with_db:
        mgr.launch(
            f"DataLogger[{gate_id}]",
            f"ros2 run data_logger data_logger --ros-args"
            f" -r __ns:={ns}"
            f" -p gate_id:={gate_id}"
            f" {params}",
            env=_load_env(),
        )

    if not args.no_gui:
        mgr.launch(
            f"GUI[{gate_id}]",
            f"ros2 run dimension_gui dimension_gui --ros-args"
            f" -r __ns:={ns}"
            f" -p depth_camera_info_topic:={cam_base}/aligned_depth_to_color/camera_info"
            f" {params}",
        )

    print("\nTekan Ctrl+C untuk menghentikan.\n")
    mgr.wait()


# ══════════════════════════════════════════════════════════════════════════
# Mode: train
# ══════════════════════════════════════════════════════════════════════════
def run_train_mode(args: argparse.Namespace) -> None:
    bag_dir = _resolve_bag_dir(args.output_dir)
    bag_dir.parent.mkdir(parents=True, exist_ok=True)
    print(f"Training mode — merekam ke '{bag_dir}'.")

    topics = " ".join([
        "/camera/camera/color/image_raw",
        "/camera/camera/aligned_depth_to_color/image_raw",
        "/camera/camera/aligned_depth_to_color/camera_info",
    ])
    mgr = ProcessManager()
    mgr.launch("RealSense", REALSENSE_CMD)
    mgr.launch("RosbagRecorder",
               f"ros2 bag record -o {shlex.quote(str(bag_dir))} {topics}")
    print("Merekam… Tekan Ctrl+C untuk berhenti.\n")
    mgr.wait()
    print(f"\nData tersimpan di: {bag_dir}")
    print("Lihat docs/training_guide.md untuk langkah selanjutnya.")


# ══════════════════════════════════════════════════════════════════════════
# Helpers
# ══════════════════════════════════════════════════════════════════════════
def _ensure_gates_file() -> None:
    if not GATES_CONFIG.exists():
        GATES_CONFIG.parent.mkdir(parents=True, exist_ok=True)
        GATES_CONFIG.write_text(
            '{\n  "version": "1.0",\n  "gates": []\n}\n',
            encoding="utf-8",
        )
        print(f"File gates.json dibuat: {GATES_CONFIG}")
        print("Tambah gate via dashboard atau ros2 service call.\n")


def _resolve_bag_dir(path_arg: Optional[str]) -> Path:
    if path_arg:
        return Path(path_arg).expanduser()
    ts = dt.datetime.now().strftime("%Y%m%d_%H%M%S")
    return Path.home() / "datasets" / f"smartport_{ts}"


def _load_env() -> dict:
    """Muat variabel DB dari .env jika ada."""
    env_file = WORKSPACE_ROOT / ".env"
    result: dict[str, str] = {}
    if env_file.exists():
        for line in env_file.read_text().splitlines():
            line = line.strip()
            if line and not line.startswith("#") and "=" in line:
                k, _, v = line.partition("=")
                result[k.strip()] = v.strip()
    # Environment aktif override .env
    for k in ("POSTGRES_HOST", "POSTGRES_PORT", "POSTGRES_DB",
              "POSTGRES_USER", "POSTGRES_PASSWORD"):
        if k in os.environ:
            result[k] = os.environ[k]
    return result


# ══════════════════════════════════════════════════════════════════════════
# CLI
# ══════════════════════════════════════════════════════════════════════════
def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="SmartPortCam CLI — deploy / single / train",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    sub = parser.add_subparsers(dest="mode", required=True)

    # ── deploy ──────────────────────────────────────────────────────
    dp = sub.add_parser("deploy", help="Jalankan sistem multi-gate penuh.")
    dp.add_argument("--with-rosbridge", action="store_true",
                    help="Jalankan rosbridge + web_video_server.")

    # ── single ──────────────────────────────────────────────────────
    sg = sub.add_parser("single",
                        help="Jalankan satu gate langsung (testing/sederhana).")
    sg.add_argument("--gate-id",         required=True,
                    help="ID gate, contoh: gate_1")
    sg.add_argument("--camera-serial",   required=True,
                    help="Serial number kamera RealSense.")
    sg.add_argument("--model-path",
                    default=str(WORKSPACE_ROOT / "models" / "yolov8_smartport.pt"),
                    help="Path ke model YOLOv8.")
    sg.add_argument("--confidence",      type=float, default=0.70)
    sg.add_argument("--min-valid-points",type=int,   default=200)
    sg.add_argument("--max-detections",  type=int,   default=1)
    sg.add_argument("--with-db",         action="store_true",
                    help="Aktifkan data_logger (simpan ke PostgreSQL).")
    sg.add_argument("--no-gui",          action="store_true",
                    help="Nonaktifkan window Tkinter.")

    # ── train ────────────────────────────────────────────────────────
    tr = sub.add_parser("train", help="Rekam dataset RealSense + rosbag.")
    tr.add_argument("--output-dir",
                    help="Direktori output rosbag (default: ~/datasets/smartport_<ts>).")

    return parser.parse_args(argv)


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = parse_args(argv or sys.argv[1:])
    try:
        if args.mode == "deploy":
            run_deploy_mode(args)
        elif args.mode == "single":
            run_single_mode(args)
        elif args.mode == "train":
            run_train_mode(args)
    except RuntimeError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
