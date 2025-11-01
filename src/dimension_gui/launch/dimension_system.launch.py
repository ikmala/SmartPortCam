"""Launch file for SmartPortCam dimension pipeline."""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def _create_pipeline_nodes(context):
    """Create the list of launch nodes based on launch arguments."""
    model_path = LaunchConfiguration("model_path").perform(context)
    engine_path = LaunchConfiguration("engine_path").perform(context)
    enable_value = LaunchConfiguration("enable_gui").perform(context).lower()
    tariff_gui_value = LaunchConfiguration("enable_tariff_gui").perform(context).lower()
    use_gui = enable_value in ("true", "1", "yes", "on")
    use_tariff_gui = tariff_gui_value in ("true", "1", "yes", "on")

    realsense = Node(
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

    yolov8 = Node(
        package="yolov8_detector",
        executable="yolov8_detector",
        name="yolov8_detector",
        output="screen",
        parameters=[
            {"model_path": model_path},
            {"engine_path": engine_path},
        ],
    )

    estimator = Node(
        package="dimension_estimator",
        executable="dimension_estimator",
        name="dimension_estimator",
        output="screen",
    )

    tariff = Node(
        package="tariff_calculator",
        executable="tariff_calculator",
        name="tariff_calculator",
        output="screen",
        parameters=[{"enable_gui": use_tariff_gui}],
    )

    nodes = [realsense, yolov8, estimator, tariff]
    if use_gui:
        gui_node = Node(
            package="dimension_gui",
            executable="dimension_gui",
            name="dimension_gui",
            output="screen",
            parameters=[{"enable_gui": True}],
        )
        nodes.append(gui_node)
    return nodes


def generate_launch_description() -> LaunchDescription:
    """Generate launch description for the full dimension system."""
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
                "enable_gui",
                default_value="true",
                description="Enable the Tkinter dashboard node.",
            ),
            DeclareLaunchArgument(
                "enable_tariff_gui",
                default_value="true",
                description="Enable GUI pada node tariff_calculator.",
            ),
            OpaqueFunction(function=_create_pipeline_nodes),
        ]
    )
