"""
Launch file utama SmartPortCam — multi-gate.

Menjalankan:
  - gate_manager   : lifecycle semua gate (baca config/gates.json)
  - camera_manager : deteksi kamera RealSense yang terhubung
  - rosbridge      : (opsional) WebSocket untuk web dashboard
  - web_video_server: (opsional) MJPEG stream

Penggunaan:
  ros2 launch smartport smartport.launch.py
  ros2 launch smartport smartport.launch.py with_bridge:=true
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from pathlib import Path

WORKSPACE = Path(__file__).resolve().parents[1]
PARAMS    = str(WORKSPACE / "config" / "smartport.yaml")


def generate_launch_description() -> LaunchDescription:
    with_bridge_arg = DeclareLaunchArgument(
        "with_bridge",
        default_value="false",
        description="Jalankan rosbridge + web_video_server (true/false)",
    )

    gate_manager_node = Node(
        package="gate_manager",
        executable="gate_manager",
        name="gate_manager",
        parameters=[PARAMS],
        output="screen",
    )

    camera_manager_node = Node(
        package="camera_manager",
        executable="camera_manager",
        name="camera_manager",
        parameters=[PARAMS],
        output="screen",
    )

    def conditional_bridge(context, *args, **kwargs):
        val = LaunchConfiguration("with_bridge").perform(context).lower()
        if val not in ("true", "1", "yes"):
            return []
        from launch_ros.actions import Node as N
        from launch.actions import IncludeLaunchDescription
        from launch.launch_description_sources import PythonLaunchDescriptionSource
        import ament_index_python
        rosbridge_dir = ament_index_python.get_package_share_directory(
            "rosbridge_server"
        )
        return [
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    rosbridge_dir + "/launch/rosbridge_websocket_launch.xml"
                )
            ),
            N(
                package="web_video_server",
                executable="web_video_server",
                name="web_video_server",
                output="screen",
            ),
        ]

    return LaunchDescription([
        with_bridge_arg,
        gate_manager_node,
        camera_manager_node,
        OpaqueFunction(function=conditional_bridge),
    ])
