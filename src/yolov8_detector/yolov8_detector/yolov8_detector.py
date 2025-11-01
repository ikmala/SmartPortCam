"""ROS 2 YOLOv8 detector node."""

from pathlib import Path
from typing import Iterable, List, Optional

import numpy as np
import rclpy
from cv_bridge import CvBridge, CvBridgeError
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
        self.create_subscription(
            Image,
            "/camera/color/image_raw",
            self._image_callback,
            qos_profile_sensor_data,
        )
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

    def _resolve_class_ids(self) -> List[int]:
        """Return class IDs as a list of integers."""
        param = self.get_parameter("class_ids")
        value = param.value
        if isinstance(value, Iterable) and not isinstance(value, (str, bytes)):
            class_ids = [int(item) for item in value]
        else:
            class_ids = DEFAULT_CLASSES
        return class_ids or DEFAULT_CLASSES

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
            bbox_array.boxes.append(vehicle_box)
        self._bbox_pub.publish(bbox_array)


def main(args: Optional[List[str]] = None) -> None:
    """Initialize ROS client library and spin the detector node."""
    rclpy.init(args=args)
    node = Yolov8DetectorNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":  # pragma: no cover - convenience for local testing.
    main()
