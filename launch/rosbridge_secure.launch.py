"""
Rosbridge dengan topic allowlist — hanya topik SmartPortCam yang bisa diakses.
Cegah subscribe/publish ke topik ROS arbitrary dari browser.

Penggunaan:
  ros2 launch smartport rosbridge_secure.launch.py
  ros2 launch smartport rosbridge_secure.launch.py port:=9091
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


# Pattern topik yang diizinkan untuk subscribe dari web (regex)
_SUBSCRIBE_TOPICS = [
    # Status gate dan kamera
    r"^/smartport/gates/status$",
    r"^/smartport/cameras$",
    # Per-gate: dimension, tariff, video
    r"^/gate_[a-z0-9_]+/vehicle/dimension_info$",
    r"^/gate_[a-z0-9_]+/vehicle/tariff_info$",
    r"^/gate_[a-z0-9_]+/vehicle/detections/image_raw$",
]

# Pattern topik yang diizinkan untuk publish dari web (SANGAT terbatas)
_PUBLISH_TOPICS: list = []  # Web dashboard hanya read-only

# Services yang diizinkan dipanggil dari web
_SERVICES = [
    r"^/smartport/gates/add$",
    r"^/smartport/gates/remove$",
    r"^/smartport/gates/set_active$",
    r"^/smartport/gates/list$",
    r"^/smartport/cameras/list$",
]


def generate_launch_description() -> LaunchDescription:
    port_arg = DeclareLaunchArgument(
        "port", default_value="9090",
        description="Port WebSocket rosbridge.",
    )
    address_arg = DeclareLaunchArgument(
        "address", default_value="0.0.0.0",
        description="Interface binding rosbridge.",
    )

    rosbridge_node = Node(
        package="rosbridge_server",
        executable="rosbridge_websocket",
        name="rosbridge_websocket",
        parameters=[{
            "port":                   LaunchConfiguration("port"),
            "address":                LaunchConfiguration("address"),
            # Topic security
            "topics_glob":            str(_SUBSCRIBE_TOPICS),
            "services_glob":          str(_SERVICES),
            # Connection limits
            "max_message_size":       10_000_000,   # 10 MB max per message
            "fragment_timeout":       600,
            "delay_between_messages": 0,
            "max_burst_size":         0,
            # Logging
            "websocket_ping_interval": 0,
            "websocket_ping_timeout":  30,
        }],
        output="screen",
    )

    web_video_node = Node(
        package="web_video_server",
        executable="web_video_server",
        name="web_video_server",
        output="screen",
    )

    return LaunchDescription([
        port_arg,
        address_arg,
        rosbridge_node,
        web_video_node,
    ])
