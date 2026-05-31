"""Node ROS 2 untuk mendeteksi kamera Intel RealSense yang terhubung via USB."""

import threading
from typing import Dict, List

import pyrealsense2 as rs
import rclpy
from rclpy.node import Node
from std_msgs.msg import Header

from vehicle_dimension_msgs.msg import CameraDevice, CameraDeviceArray
from vehicle_dimension_msgs.srv import ListCameras


class CameraManagerNode(Node):
    """Scan kamera RealSense yang terhubung dan publikasikan daftarnya."""

    _SCAN_INTERVAL = 5.0  # detik

    def __init__(self) -> None:
        super().__init__("camera_manager")
        self._lock = threading.Lock()
        self._assigned: Dict[str, str] = {}  # serial -> gate_id

        self._pub = self.create_publisher(
            CameraDeviceArray,
            "/smartport/cameras",
            10,
        )
        self.create_service(
            ListCameras,
            "/smartport/cameras/list",
            self._list_cameras_cb,
        )
        self.create_timer(self._SCAN_INTERVAL, self._scan_and_publish)

        # Scan pertama langsung saat node start
        self._scan_and_publish()
        self.get_logger().info("Camera manager node siap — scan setiap 5 detik.")

    # ------------------------------------------------------------------
    # Publik API: tandai serial dipakai oleh gate tertentu
    # ------------------------------------------------------------------
    def mark_assigned(self, serial: str, gate_id: str) -> None:
        with self._lock:
            self._assigned[serial] = gate_id

    def mark_released(self, serial: str) -> None:
        with self._lock:
            self._assigned.pop(serial, None)

    # ------------------------------------------------------------------
    # Internal
    # ------------------------------------------------------------------
    def _enumerate_devices(self) -> List[CameraDevice]:
        devices: List[CameraDevice] = []
        try:
            ctx = rs.context()
            for dev in ctx.devices:
                serial = dev.get_info(rs.camera_info.serial_number)
                model  = dev.get_info(rs.camera_info.name)
                try:
                    usb = dev.get_info(rs.camera_info.usb_type_descriptor)
                except Exception:
                    usb = "unknown"

                msg = CameraDevice()
                msg.serial_number     = serial
                msg.model_name        = model
                msg.usb_type          = usb
                msg.assigned_gate_id  = self._assigned.get(serial, "")
                msg.in_use            = serial in self._assigned
                devices.append(msg)
        except Exception as exc:
            self.get_logger().warning(f"Gagal enumerate RealSense: {exc}")
        return devices

    def _scan_and_publish(self) -> None:
        with self._lock:
            devices = self._enumerate_devices()

        arr = CameraDeviceArray()
        arr.header = Header()
        arr.header.stamp = self.get_clock().now().to_msg()
        arr.devices = devices
        self._pub.publish(arr)

        if devices:
            serials = [d.serial_number for d in devices]
            self.get_logger().debug(f"Kamera terdeteksi: {serials}")
        else:
            self.get_logger().debug("Tidak ada kamera RealSense yang terhubung.")

    def _list_cameras_cb(
        self,
        _request: ListCameras.Request,
        response: ListCameras.Response,
    ) -> ListCameras.Response:
        with self._lock:
            response.devices = self._enumerate_devices()
        return response


def main(args=None) -> None:
    rclpy.init(args=args)
    node = CameraManagerNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
