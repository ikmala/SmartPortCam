// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from vehicle_dimension_msgs:msg/VehicleBoundingBox.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box__rosidl_typesupport_introspection_c.h"
#include "vehicle_dimension_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box__functions.h"
#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  vehicle_dimension_msgs__msg__VehicleBoundingBox__init(message_memory);
}

void vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_fini_function(void * message_memory)
{
  vehicle_dimension_msgs__msg__VehicleBoundingBox__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_message_member_array[6] = {
  {
    "class_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleBoundingBox, class_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "confidence",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleBoundingBox, confidence),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "xmin",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleBoundingBox, xmin),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "ymin",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleBoundingBox, ymin),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "xmax",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleBoundingBox, xmax),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "ymax",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleBoundingBox, ymax),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_message_members = {
  "vehicle_dimension_msgs__msg",  // message namespace
  "VehicleBoundingBox",  // message name
  6,  // number of fields
  sizeof(vehicle_dimension_msgs__msg__VehicleBoundingBox),
  vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_message_member_array,  // message members
  vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_init_function,  // function to initialize message memory (memory has to be allocated)
  vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_message_type_support_handle = {
  0,
  &vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_vehicle_dimension_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, vehicle_dimension_msgs, msg, VehicleBoundingBox)() {
  if (!vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_message_type_support_handle.typesupport_identifier) {
    vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &vehicle_dimension_msgs__msg__VehicleBoundingBox__rosidl_typesupport_introspection_c__VehicleBoundingBox_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
