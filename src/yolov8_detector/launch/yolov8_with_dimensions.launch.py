"""Launch YOLOv8 detector with optional RealSense and dimension estimator."""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def _as_bool(value: str) -> bool:
    """Return True if the string represents a truthy value."""
    return value.lower() in ("true", "1", "yes", "on")


def _create_nodes(context):
    """Assemble nodes once launch arguments are resolved."""
    model_path = LaunchConfiguration("model_path").perform(context)
    engine_path = LaunchConfiguration("engine_path").perform(context)
    display_flag = LaunchConfiguration("display_results").perform(context)
    realsense_flag = LaunchConfiguration("start_realsense").perform(context)

    display_results = _as_bool(display_flag)
    start_realsense = _as_bool(realsense_flag)

    nodes = []
    if start_realsense:
        nodes.append(
            Node(
                package="realsense2_camera",
                executable="realsense2_camera_node",
                name="realsense2_camera",
                output="screen",
                parameters=[
                    {"align_depth.enable": True},
                    {"enable_sync": True},
                    {"enable_infra1": False},
                    {"enable_infra2": False},
                    {"enable_pointcloud": False},
                    {"color_fps": 30.0},
                    {"depth_fps": 30.0},
                ],
            )
        )
    nodes.append(
        Node(
            package="yolov8_detector",
            executable="yolov8_detector",
            name="yolov8_detector",
            output="screen",
            parameters=[
                {"model_path": model_path},
                {"engine_path": engine_path},
                {"display_results": display_results},
            ],
        )
    )
    nodes.append(
        Node(
            package="dimension_estimator",
            executable="dimension_estimator",
            name="dimension_estimator",
            output="screen",
        )
    )
    return nodes


def generate_launch_description() -> LaunchDescription:
    """Launch detector with optional visualization and dimension estimation."""
    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "model_path",
                default_value="",
                description="Path to YOLOv8 model (.pt).",
            ),
            DeclareLaunchArgument(
                "engine_path",
                default_value="",
                description="Optional TensorRT engine path (.engine).",
            ),
            DeclareLaunchArgument(
                "display_results",
                default_value="false",
                description="Enable OpenCV display window for detections.",
            ),
            DeclareLaunchArgument(
                "start_realsense",
                default_value="false",
                description="Start the RealSense camera driver as part of this launch.",
            ),
            OpaqueFunction(function=_create_nodes),
        ]
    )
