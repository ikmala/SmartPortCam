cmake_minimum_required(VERSION 3.8)
project(vehicle_dimension_msgs)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

set(msg_files
  "msg/VehicleBoundingBox.msg"
  "msg/VehicleBoundingBoxArray.msg"
  "msg/VehicleDimension.msg"
  "msg/VehicleTariff.msg"
  "msg/GateStatus.msg"
  "msg/CameraDevice.msg"
  "msg/CameraDeviceArray.msg"
)

set(srv_files
  "srv/AddGate.srv"
  "srv/RemoveGate.srv"
  "srv/SetGateActive.srv"
  "srv/ListCameras.srv"
  "srv/ListGates.srv"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  ${srv_files}
  DEPENDENCIES
    std_msgs
)

ament_export_dependencies(rosidl_default_runtime)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
