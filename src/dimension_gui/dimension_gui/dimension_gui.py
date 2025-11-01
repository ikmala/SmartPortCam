"""Simple GUI/console dashboard for SmartPortCam."""

from queue import Empty, Queue
from threading import Event, Thread
from typing import Optional

try:
    import tkinter as tk
    from tkinter import ttk
except ImportError:  # pragma: no cover - optional dependency.
    tk = None
    ttk = None

import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import CameraInfo

from vehicle_dimension_msgs.msg import VehicleDimension


class DimensionDashboardNode(Node):
    """Display vehicle dimension updates in a GUI or log output."""

    def __init__(self) -> None:
        super().__init__("dimension_gui")
        self.declare_parameter("enable_gui", bool(tk is not None))
        self.declare_parameter("stale_timeout_sec", 2.0)
        self._enable_gui = bool(self.get_parameter("enable_gui").value and tk)
        self._stale_timeout = float(self.get_parameter("stale_timeout_sec").value)
        self._latest_dimension: Optional[VehicleDimension] = None
        self._last_dimension_time = None
        self._last_camera_time = None
        self._update_queue: Queue = Queue()
        self._shutdown_event = Event()
        self._gui_thread: Optional[Thread] = None

        self.create_subscription(
            VehicleDimension,
            "/vehicle/dimension_info",
            self._dimension_callback,
            10,
        )
        self.create_subscription(
            CameraInfo,
            "/camera/aligned_depth_to_color/camera_info",
            self._camera_callback,
            qos_profile_sensor_data,
        )
        self.create_timer(1.0, self._status_timer_callback)

        if self._enable_gui:
            self._start_gui()
        else:
            if tk is None:
                self.get_logger().warning(
                    "Tkinter is not available; falling back to console logs."
                )
            else:
                self.get_logger().info(
                    "GUI disabled via parameter; using console logs."
                )

    def _start_gui(self) -> None:
        """Launch the Tkinter dashboard in a background thread."""
        self._gui_thread = Thread(target=self._run_gui_loop, daemon=True)
        self._gui_thread.start()

    def _run_gui_loop(self) -> None:
        """Execute Tkinter main loop and process queued updates."""
        if tk is None:
            return
        root = tk.Tk()
        root.title("SmartPortCam Dimension Dashboard")
        root.geometry("380x200")

        self._length_var = tk.StringVar(value="Length: -- mm")
        self._width_var = tk.StringVar(value="Width: -- mm")
        self._height_var = tk.StringVar(value="Height: -- mm")
        self._class_var = tk.StringVar(value="Class ID: --")
        self._status_var = tk.StringVar(value="Camera: Unknown")
        self._timestamp_var = tk.StringVar(value="Updated: --")

        content = ttk.Frame(root, padding=12)
        content.pack(fill=tk.BOTH, expand=True)
        ttk.Label(content, textvariable=self._length_var).pack(anchor=tk.W)
        ttk.Label(content, textvariable=self._width_var).pack(anchor=tk.W)
        ttk.Label(content, textvariable=self._height_var).pack(anchor=tk.W)
        ttk.Label(content, textvariable=self._class_var).pack(anchor=tk.W, pady=(0, 8))
        ttk.Label(content, textvariable=self._status_var).pack(anchor=tk.W)
        ttk.Label(content, textvariable=self._timestamp_var).pack(anchor=tk.W)

        def poll_queue() -> None:
            while True:
                try:
                    kind, payload = self._update_queue.get_nowait()
                except Empty:
                    break
                if kind == "dimensions":
                    self._length_var.set(f"Length: {payload.length:.1f} mm")
                    self._width_var.set(f"Width: {payload.width:.1f} mm")
                    self._height_var.set(f"Height: {payload.height:.1f} mm")
                    self._class_var.set(f"Class ID: {payload.class_id}")
                    self._timestamp_var.set(
                        f"Updated: {payload.header.stamp.sec}.{payload.header.stamp.nanosec:09d}"
                    )
                elif kind == "status":
                    self._status_var.set(payload)
            if not self._shutdown_event.is_set():
                root.after(200, poll_queue)
            else:
                root.destroy()

        root.after(200, poll_queue)
        root.mainloop()

    def _dimension_callback(self, msg: VehicleDimension) -> None:
        """Handle incoming dimension measurements."""
        self._latest_dimension = msg
        self._last_dimension_time = self.get_clock().now()
        if self._enable_gui:
            self._update_queue.put(("dimensions", msg))
        else:
            self.get_logger().info(
                "Vehicle class %d - L: %.1f mm, W: %.1f mm, H: %.1f mm",
                msg.class_id,
                msg.length,
                msg.width,
                msg.height,
            )

    def _camera_callback(self, _msg: CameraInfo) -> None:
        """Track camera activity using camera info timestamps."""
        self._last_camera_time = self.get_clock().now()

    def _status_timer_callback(self) -> None:
        """Update camera status indicator."""
        now = self.get_clock().now()
        camera_ok = (
            self._last_camera_time
            and (now - self._last_camera_time).nanoseconds * 1e-9
            < self._stale_timeout
        )
        status_text = "Camera: Online" if camera_ok else "Camera: Offline"
        if self._enable_gui:
            self._update_queue.put(("status", status_text))
        else:
            self.get_logger().debug(status_text)

    def destroy_node(self) -> bool:
        """Signal GUI shutdown before destroying the node."""
        self._shutdown_event.set()
        return super().destroy_node()


def main(args=None) -> None:
    """Entry point for the dimension GUI node."""
    rclpy.init(args=args)
    node = DimensionDashboardNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
