"""Dimension estimator node for SmartPortCam."""

from threading import Lock
from typing import List, Optional, Tuple

import numpy as np
import open3d as o3d
import pyrealsense2 as rs
import rclpy
from cv_bridge import CvBridge, CvBridgeError
from rclpy.node import Node
from sensor_msgs.msg import CameraInfo, Image

from vehicle_dimension_msgs.msg import (
    VehicleBoundingBox,
    VehicleBoundingBoxArray,
    VehicleDimension,
)


class DimensionEstimator(Node):
    """Estimate vehicle dimensions using aligned depth images and detections."""

    def __init__(self) -> None:
        super().__init__("dimension_estimator")
        self.declare_parameters(
            "",
            [
                ("depth_scale", 0.001),
                ("min_depth_m", 0.5),
                ("max_depth_m", 20.0),
                ("min_valid_points", 500),
                ("voxel_size", 0.05),
                ("use_outlier_filter", True),
                ("bbox_topic", "/vehicle/bounding_boxes"),
                (
                    "depth_image_topic",
                    "/camera/aligned_depth_to_color/image_raw",
                ),
                (
                    "depth_camera_info_topic",
                    "/camera/aligned_depth_to_color/camera_info",
                ),
            ],
        )
        self.bridge = CvBridge()
        self._lock = Lock()
        self._latest_boxes: VehicleBoundingBoxArray = VehicleBoundingBoxArray()
        self._intrinsics: Optional[rs.intrinsics] = None
        self._depth_scale = float(self.get_parameter("depth_scale").value)
        self._min_depth = float(self.get_parameter("min_depth_m").value)
        self._max_depth = float(self.get_parameter("max_depth_m").value)
        self._min_points = int(self.get_parameter("min_valid_points").value)
        self._voxel_size = float(self.get_parameter("voxel_size").value)
        self._use_outlier_filter = bool(
            self.get_parameter("use_outlier_filter").value
        )
        bbox_topic = (
            self.get_parameter("bbox_topic").value or "/vehicle/bounding_boxes"
        )
        depth_topic = (
            self.get_parameter("depth_image_topic").value
            or "/camera/aligned_depth_to_color/image_raw"
        )
        camera_info_topic = (
            self.get_parameter("depth_camera_info_topic").value
            or "/camera/aligned_depth_to_color/camera_info"
        )

        self.create_subscription(
            VehicleBoundingBoxArray,
            bbox_topic,
            self._bbox_callback,
            10,
        )
        self.create_subscription(
            CameraInfo,
            camera_info_topic,
            self._camera_info_callback,
            10,
        )
        self.create_subscription(
            Image,
            depth_topic,
            self._depth_callback,
            10,
        )
        self._dimension_pub = self.create_publisher(
            VehicleDimension,
            "/vehicle/dimension_info",
            10,
        )
        self.get_logger().info("Dimension estimator node ready.")

    def _bbox_callback(self, msg: VehicleBoundingBoxArray) -> None:
        """Cache the most recent set of vehicle bounding boxes."""
        with self._lock:
            self._latest_boxes = msg

    def _camera_info_callback(self, msg: CameraInfo) -> None:
        """Convert camera info into RealSense intrinsics."""
        intrinsics = rs.intrinsics()
        intrinsics.width = msg.width
        intrinsics.height = msg.height
        intrinsics.fx = msg.k[0]
        intrinsics.fy = msg.k[4]
        intrinsics.ppx = msg.k[2]
        intrinsics.ppy = msg.k[5]
        intrinsics.model = rs.distortion.none
        intrinsics.coeffs = [0.0, 0.0, 0.0, 0.0, 0.0]
        with self._lock:
            self._intrinsics = intrinsics

    def _depth_callback(self, msg: Image) -> None:
        """Compute 3D extents for every bounding box using depth ROI."""
        with self._lock:
            boxes = list(self._latest_boxes.boxes)
            intrinsics = self._intrinsics
        if not boxes or intrinsics is None:
            return
        try:
            depth_image = self.bridge.imgmsg_to_cv2(
                msg,
                desired_encoding="passthrough",
            )
        except CvBridgeError as exc:
            self.get_logger().error(f"Failed to convert depth image: {exc}")
            return
        if depth_image.ndim != 2:
            self.get_logger().warning("Depth image is not single channel.")
            return
        height, width = depth_image.shape
        for box in boxes:
            points = self._depth_roi_to_points(depth_image, box, intrinsics, width)
            if points is None:
                continue
            length_m, width_m, height_m = self._compute_axis_aligned_bbox(points)
            if length_m is None:
                continue
            output = VehicleDimension()
            output.header = msg.header
            output.length = length_m * 1000.0
            output.width = width_m * 1000.0
            output.height = height_m * 1000.0
            output.class_id = box.class_id
            self._dimension_pub.publish(output)

    def _depth_roi_to_points(
        self,
        depth_image: np.ndarray,
        box: VehicleBoundingBox,
        intrinsics: rs.intrinsics,
        image_width: int,
    ) -> Optional[np.ndarray]:
        """Project depth ROI into 3D points."""
        xmin = max(box.xmin, 0)
        ymin = max(box.ymin, 0)
        xmax = min(box.xmax, image_width - 1)
        ymax = min(box.ymax, depth_image.shape[0] - 1)
        if xmin >= xmax or ymin >= ymax:
            return None
        roi = depth_image[ymin:ymax, xmin:xmax].astype(np.float32) * self._depth_scale
        valid_mask = (roi > self._min_depth) & (roi < self._max_depth)
        if np.count_nonzero(valid_mask) < self._min_points:
            return None
        v_coords, u_coords = np.where(valid_mask)
        u = u_coords + xmin
        v = v_coords + ymin
        z = roi[valid_mask]
        x = (u - intrinsics.ppx) * z / intrinsics.fx
        y = (v - intrinsics.ppy) * z / intrinsics.fy
        points = np.stack((x, y, z), axis=-1)
        return points

    def _compute_axis_aligned_bbox(
        self,
        points: np.ndarray,
    ) -> Tuple[Optional[float], Optional[float], Optional[float]]:
        """Return length, width, height (m) from a point cloud."""
        if points.shape[0] < self._min_points:
            return (None, None, None)
        pcd = o3d.geometry.PointCloud()
        pcd.points = o3d.utility.Vector3dVector(points)
        if self._voxel_size > 0.0:
            pcd = pcd.voxel_down_sample(self._voxel_size)
        if self._use_outlier_filter and len(pcd.points) >= self._min_points:
            pcd, _ = pcd.remove_statistical_outlier(
                nb_neighbors=20,
                std_ratio=2.0,
            )
        if len(pcd.points) < self._min_points // 4:
            return (None, None, None)
        aabb = pcd.get_axis_aligned_bounding_box()
        extent = aabb.get_extent()
        return float(extent[0]), float(extent[1]), float(extent[2])


def main(args: Optional[List[str]] = None) -> None:
    """Initialize ROS and spin the dimension estimator node."""
    rclpy.init(args=args)
    node = DimensionEstimator()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()
