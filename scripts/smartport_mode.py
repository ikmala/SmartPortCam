#!/usr/bin/env python3
"""CLI helper to run SmartPortCam in training-data or deployment mode."""

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
from typing import List, Sequence, Tuple

WORKSPACE_ROOT = Path(__file__).resolve().parents[1]
SETUP_SCRIPT = WORKSPACE_ROOT / "install" / "setup.bash"

REAL_SENSE_COMMAND = (
    "ros2 launch realsense2_camera rs_launch.py "
    "align_depth.enable:=true "
    "enable_sync:=true "
    "enable_infra1:=false "
    "enable_infra2:=false "
    "pointcloud.enable:=false"
)

DEFAULT_CLASS_IDS = "[2,3,5,7]"


class ProcessManager:
    """Track spawned processes and shut them down gracefully."""

    def __init__(self) -> None:
        self._procs: List[Tuple[str, subprocess.Popen]] = []
        self._stopping = False

    def launch(self, label: str, command: str) -> None:
        """Spawn a command in a new shell with the workspace sourced."""
        if not SETUP_SCRIPT.exists():
            raise RuntimeError(
                f"Setup script '{SETUP_SCRIPT}' not found. Run colcon build first."
            )
        sourced_cmd = (
            f"source {shlex.quote(str(SETUP_SCRIPT))} && {command}"
        )
        proc = subprocess.Popen(
            ["bash", "-lc", sourced_cmd],
            cwd=str(WORKSPACE_ROOT),
        )
        self._procs.append((label, proc))
        print(f"[{label}] started (PID={proc.pid})")

    def wait(self) -> None:
        """Keep the script alive while any child process is running."""
        try:
            while True:
                alive = [proc for _, proc in self._procs if proc.poll() is None]
                if not alive:
                    print("All child processes have exited.")
                    break
                time.sleep(1.0)
        except KeyboardInterrupt:
            print("\nKeyboard interrupt received, stopping all processes...")
        finally:
            self.shutdown()

    def shutdown(self) -> None:
        """Terminate all tracked processes."""
        if self._stopping:
            return
        self._stopping = True
        for label, proc in reversed(self._procs):
            if proc.poll() is None:
                print(f"Stopping [{label}] (PID={proc.pid})")
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


def run_train_mode(args: argparse.Namespace) -> None:
    """Start RealSense and rosbag recording for dataset collection."""
    bag_dir = resolve_bag_directory(args.output_dir)
    bag_parent = bag_dir.parent
    bag_parent.mkdir(parents=True, exist_ok=True)
    print(f"Training mode: files will be recorded to '{bag_dir}'.")

    topics = " ".join(
        [
            "/camera/camera/color/image_raw",
            "/camera/camera/aligned_depth_to_color/image_raw",
            "/camera/camera/aligned_depth_to_color/camera_info",
        ]
    )
    record_cmd = (
        f"ros2 bag record -o {shlex.quote(str(bag_dir))} {topics}"
    )

    manager = ProcessManager()
    manager.launch("RealSense", REAL_SENSE_COMMAND)
    manager.launch("RosbagRecorder", record_cmd)
    print("Recording... Press Ctrl+C when you want to stop.")
    manager.wait()
    print(
        "\nNext steps: auto-label frames and train YOLO (see running.txt MODE 1)."
    )


def run_deploy_mode(args: argparse.Namespace) -> None:
    """Launch detector, estimator, GUI, and optional tariff calculator."""
    manager = ProcessManager()
    manager.launch("RealSense", REAL_SENSE_COMMAND)

    detector_cmd = build_detector_command(
        model_path=args.model_path,
        image_topic=args.image_topic,
        class_ids=args.class_ids,
        max_detections=args.max_detections,
        display_results=args.display_results,
        publish_annotated=args.publish_annotated_image,
    )
    manager.launch("Yolov8Detector", detector_cmd)

    estimator_cmd = (
        "ros2 run dimension_estimator dimension_estimator --ros-args "
        f"-p bbox_topic:={shlex.quote(args.bbox_topic)} "
        f"-p depth_image_topic:={shlex.quote(args.depth_image_topic)} "
        f"-p depth_camera_info_topic:={shlex.quote(args.depth_camera_info_topic)} "
        f"-p min_valid_points:={args.min_valid_points}"
    )
    manager.launch("DimensionEstimator", estimator_cmd)

    gui_cmd = (
        "ros2 run dimension_gui dimension_gui --ros-args "
        f"-p depth_camera_info_topic:={shlex.quote(args.depth_camera_info_topic)}"
    )
    manager.launch("DimensionGUI", gui_cmd)

    if args.with_tariff:
        manager.launch("TariffCalculator", "ros2 run tariff_calculator tariff_calculator")

    print("Deployment mode is running. Press Ctrl+C to stop everything.")
    manager.wait()


def resolve_bag_directory(path_arg: str | None) -> Path:
    """Return the rosbag output directory."""
    if path_arg:
        return Path(path_arg).expanduser()
    timestamp = dt.datetime.now().strftime("%Y%m%d_%H%M%S")
    return Path.home() / "datasets" / f"smartport_{timestamp}"


def build_detector_command(
    *,
    model_path: str,
    image_topic: str,
    class_ids: str,
    max_detections: int,
    display_results: bool,
    publish_annotated: bool,
) -> str:
    """Format the ros2 run command for the detector node."""
    params = {
        "model_path": model_path,
        "image_topic": image_topic,
        "class_ids": class_ids,
        "max_detections": str(max(0, max_detections)),
        "display_results": str(display_results).lower(),
        "publish_annotated_image": str(publish_annotated).lower(),
    }
    param_list = " ".join(
        f"-p {key}:={shlex.quote(value)}" for key, value in params.items()
    )
    return f"ros2 run yolov8_detector yolov8_detector --ros-args {param_list}"


def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Run SmartPortCam in training-data or deployment mode.",
    )
    sub = parser.add_subparsers(dest="mode", required=True)

    train = sub.add_parser("train", help="Collect data via RealSense + rosbag record.")
    train.add_argument(
        "--output-dir",
        help="Directory name for rosbag output (default: ~/datasets/smartport_<timestamp>).",
    )

    deploy = sub.add_parser("deploy", help="Run full SmartPortCam inference pipeline.")
    deploy.add_argument(
        "--model-path",
        default=str(WORKSPACE_ROOT / "yolov8n.pt"),
        help="Path to YOLO model weights.",
    )
    deploy.add_argument(
        "--image-topic",
        default="/camera/camera/color/image_raw",
        help="RGB topic used by the detector.",
    )
    deploy.add_argument(
        "--class-ids",
        default=DEFAULT_CLASS_IDS,
        help="List of COCO class IDs (e.g. \"[2,3,5,7]\").",
    )
    deploy.add_argument(
        "--max-detections",
        type=int,
        default=1,
        help="Maximum detections published per frame (0 = unlimited).",
    )
    deploy.add_argument(
        "--display-results",
        action="store_true",
        help="Show OpenCV window with annotated detections.",
    )
    deploy.add_argument(
        "--publish-annotated-image",
        action="store_true",
        help="Publish annotated image topic /vehicle/detections/image_raw.",
    )
    deploy.add_argument(
        "--bbox-topic",
        default="/vehicle/bounding_boxes",
        help="Bounding box topic consumed by dimension estimator.",
    )
    deploy.add_argument(
        "--depth-image-topic",
        default="/camera/camera/aligned_depth_to_color/image_raw",
        help="Aligned depth topic used for dimension estimation.",
    )
    deploy.add_argument(
        "--depth-camera-info-topic",
        default="/camera/camera/aligned_depth_to_color/camera_info",
        help="Camera info topic for dimension estimator & GUI.",
    )
    deploy.add_argument(
        "--min-valid-points",
        type=int,
        default=200,
        help="Minimum depth points required per bounding box.",
    )
    deploy.add_argument(
        "--with-tariff",
        action="store_true",
        help="Launch tariff_calculator node as part of deployment.",
    )

    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv or sys.argv[1:])
    try:
        if args.mode == "train":
            run_train_mode(args)
        elif args.mode == "deploy":
            run_deploy_mode(args)
        else:
            raise ValueError(f"Unknown mode '{args.mode}'")
    except RuntimeError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
