"""ROS 2 YOLOv8 detector node."""

import ast
from pathlib import Path
from typing import Iterable, List, Optional

import cv2
import numpy as np
import rclpy
from cv_bridge import CvBridge, CvBridgeError
from rclpy.exceptions import ParameterUninitializedException
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import Image
from ultralytics import YOLO

from vehicle_dimension_msgs.msg import (
    VehicleBoundingBox,
    VehicleBoundingBoxArray,
)

DEFAULT_CLASSES = [2, 3, 5, 7]  # COCO IDs for car, motorcycle, bus, truck.


class Yolov8DetectorNode(Node):
    """Run YOLOv8 inference and publish vehicle bounding boxes."""

    def __init__(self) -> None:
        super().__init__("yolov8_detector")
        self._declare_detector_parameters()
        self.bridge = CvBridge()
        self._class_ids = self._resolve_class_ids()
        self._confidence = float(self.get_parameter("confidence_threshold").value)
        self._iou = float(self.get_parameter("iou_threshold").value)
        self._model = self._load_model()
        self._display_enabled = bool(self.get_parameter("display_results").value)
        self._log_summary = bool(self.get_parameter("log_detection_summary").value)
        self._publish_annotated = bool(
            self.get_parameter("publish_annotated_image").value
        )
        self._max_detections = int(self.get_parameter("max_detections").value)
        if self._max_detections < 0:
            self.get_logger().warn(
                "Parameter 'max_detections' is negative; treating as unlimited."
            )
            self._max_detections = 0
        self._display_failed = False
        names = getattr(self._model, "names", {})
        if isinstance(names, dict):
            self._class_name_map = names
        elif isinstance(names, (list, tuple)):
            self._class_name_map = {idx: name for idx, name in enumerate(names)}
        else:
            self._class_name_map = {}
        image_topic = self.get_parameter("image_topic").value
        if not image_topic:
            image_topic = "/camera/color/image_raw"
        self._image_topic = image_topic
        self.create_subscription(
            Image,
            image_topic,
            self._image_callback,
            qos_profile_sensor_data,
        )
        self._annotated_pub = (
            self.create_publisher(
                Image,
                "/vehicle/detections/image_raw",
                10,
            )
            if self._publish_annotated
            else None
        )
        self._first_image_logged = False
        self._bbox_pub = self.create_publisher(
            VehicleBoundingBoxArray,
            "/vehicle/bounding_boxes",
            10,
        )
        self.get_logger().info("YOLOv8 detector node is ready.")

    def _declare_detector_parameters(self) -> None:
        """Declare configurable parameters with sensible defaults."""
        self.declare_parameter("model_path", "yolov8n.pt")
        self.declare_parameter("engine_path", "")
        self.declare_parameter("confidence_threshold", 0.35)
        self.declare_parameter("iou_threshold", 0.45)
        self.declare_parameter("class_ids", DEFAULT_CLASSES)
        self.declare_parameter("display_results", False)
        self.declare_parameter("image_topic", "/camera/color/image_raw")
        self.declare_parameter("log_detection_summary", True)
        self.declare_parameter("publish_annotated_image", True)
        self.declare_parameter("max_detections", 0)

    def _resolve_class_ids(self) -> Optional[List[int]]:
        """Return class IDs as a list of integers or None for all classes."""
        try:
            param = self.get_parameter("class_ids")
        except ParameterUninitializedException:
            self.get_logger().warn(
                "Parameter 'class_ids' not initialized; using default vehicle classes."
            )
            return DEFAULT_CLASSES
        raw_value = param.value

        def normalize(value) -> Optional[List[int]]:
            if value is None:
                return None
            if isinstance(value, (int, float)):
                return [int(value)]
            if isinstance(value, Iterable) and not isinstance(value, (str, bytes)):
                return [int(item) for item in value]
            return None

        class_ids = normalize(raw_value)
        if class_ids is None and isinstance(raw_value, str):
            try:
                parsed = ast.literal_eval(raw_value)
            except (ValueError, SyntaxError):
                parsed = None
                self.get_logger().warn(
                    "Unable to parse 'class_ids' parameter; falling back to defaults."
                )
            class_ids = normalize(parsed)
        if class_ids is None:
            return DEFAULT_CLASSES
        return class_ids or None

    def _load_model(self) -> YOLO:
        """Load PyTorch or TensorRT YOLOv8 model based on parameters."""
        model_param = self.get_parameter("model_path").value
        engine_param = self.get_parameter("engine_path").value
        model_path = Path(model_param) if model_param else None
        engine_path = Path(engine_param) if engine_param else None

        candidate_paths: List[Path] = []
        if engine_path and engine_path.exists():
            candidate_paths.append(engine_path)
        if model_path and model_path.exists():
            candidate_paths.append(model_path)
        if not candidate_paths:
            default_path = Path("yolov8n.pt")
            self.get_logger().warn(
                f"No model file found. Falling back to default '{default_path}'."
            )
            candidate_paths.append(default_path)
        for model_candidate in candidate_paths:
            try:
                self.get_logger().info(f"Loading YOLOv8 model from {model_candidate}")
                return YOLO(str(model_candidate))
            except Exception as exc:  # pragma: no cover - log and try fallback.
                self.get_logger().error(
                    f"Failed to load model '{model_candidate}': {exc}"
                )
        raise RuntimeError("Unable to load any YOLOv8 model candidate.")

    def _image_callback(self, msg: Image) -> None:
        """Run inference on incoming RGB images and publish bounding boxes."""
        try:
            frame = self.bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
        except CvBridgeError as exc:
            self.get_logger().error(f"Failed to convert image: {exc}")
            return
        if not self._first_image_logged:
            self._first_image_logged = True
            self.get_logger().info(f"Receiving images from '{self._image_topic}'.")

        results = self._model.predict(
            frame,
            conf=self._confidence,
            iou=self._iou,
            classes=self._class_ids,
            verbose=False,
        )
        if not results:
            return
        result = results[0]
        boxes_xyxy = getattr(result.boxes, "xyxy", None)
        scores_tensor = getattr(result.boxes, "conf", None)
        class_ids_tensor = getattr(result.boxes, "cls", None)
        bbox_array = VehicleBoundingBoxArray()
        bbox_array.header = msg.header
        if boxes_xyxy is None:
            self._bbox_pub.publish(bbox_array)
            return
        boxes_list = boxes_xyxy.tolist()
        scores: List[float] = (
            scores_tensor.tolist() if scores_tensor is not None else []
        )
        class_ids: List[float] = (
            class_ids_tensor.tolist() if class_ids_tensor is not None else []
        )
        detections = []
        for idx, box in enumerate(boxes_list):
            if idx >= len(scores) or idx >= len(class_ids):
                continue
            vehicle_box = VehicleBoundingBox()
            x1, y1, x2, y2 = box
            vehicle_box.xmin = int(np.clip(x1, 0, frame.shape[1] - 1))
            vehicle_box.ymin = int(np.clip(y1, 0, frame.shape[0] - 1))
            vehicle_box.xmax = int(np.clip(x2, 0, frame.shape[1] - 1))
            vehicle_box.ymax = int(np.clip(y2, 0, frame.shape[0] - 1))
            vehicle_box.class_id = int(class_ids[idx])
            vehicle_box.confidence = float(scores[idx])
            width = max(0, vehicle_box.xmax - vehicle_box.xmin)
            height = max(0, vehicle_box.ymax - vehicle_box.ymin)
            area = float(width * height)
            detections.append(
                {
                    "box": vehicle_box,
                    "area": area,
                    "score": vehicle_box.confidence,
                    "class_name": self._class_name_map.get(vehicle_box.class_id),
                }
            )

        if self._max_detections and len(detections) > self._max_detections:
            detections.sort(key=lambda item: item["area"], reverse=True)
            detections = detections[: self._max_detections]

        annotated = frame if not self._display_enabled else frame.copy()
        window_title = "YOLOv8 Detections"
        for detection in detections:
            vehicle_box = detection["box"]
            bbox_array.boxes.append(vehicle_box)
            if self._display_enabled:
                top_left = (vehicle_box.xmin, vehicle_box.ymin)
                bottom_right = (vehicle_box.xmax, vehicle_box.ymax)
                cv2.rectangle(annotated, top_left, bottom_right, (0, 255, 0), 2)
                class_name = detection["class_name"]
                label = class_name or f"id:{vehicle_box.class_id}"
                text = f"{label} {detection['score']:.2f}"
                text_origin = (vehicle_box.xmin, max(vehicle_box.ymin - 10, 0))
                cv2.putText(
                    annotated,
                    text,
                    text_origin,
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.5,
                    (0, 255, 0),
                    1,
                    cv2.LINE_AA,
                )

        self._bbox_pub.publish(bbox_array)
        if self._log_summary:
            self.get_logger().info(f"Detected {len(bbox_array.boxes)} object(s).")
        if self._publish_annotated and self._annotated_pub is not None:
            try:
                annotated_msg = self.bridge.cv2_to_imgmsg(annotated, encoding="bgr8")
            except CvBridgeError as exc:
                self.get_logger().error(f"Failed to convert annotated image: {exc}")
            else:
                annotated_msg.header = msg.header
                self._annotated_pub.publish(annotated_msg)
        if self._display_enabled and not self._display_failed:
            try:
                cv2.imshow(window_title, annotated)
                cv2.waitKey(1)
            except cv2.error as exc:  # pragma: no cover - requires GUI
                self._display_failed = True
                self.get_logger().error(f"Failed to display detections: {exc}")


def main(args: Optional[List[str]] = None) -> None:
    """Initialize ROS client library and spin the detector node."""
    rclpy.init(args=args)
    node = Yolov8DetectorNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":  # pragma: no cover - convenience for local testing.
    main()
