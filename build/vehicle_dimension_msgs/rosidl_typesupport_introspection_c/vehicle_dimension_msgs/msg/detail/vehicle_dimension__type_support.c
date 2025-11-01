// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from vehicle_dimension_msgs:msg/VehicleDimension.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__rosidl_typesupport_introspection_c.h"
#include "vehicle_dimension_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__functions.h"
#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  vehicle_dimension_msgs__msg__VehicleDimension__init(message_memory);
}

void vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_fini_function(void * message_memory)
{
  vehicle_dimension_msgs__msg__VehicleDimension__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_message_member_array[5] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleDimension, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "length",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleDimension, length),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "width",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleDimension, width),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "height",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleDimension, height),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "class_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleDimension, class_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_message_members = {
  "vehicle_dimension_msgs__msg",  // message namespace
  "VehicleDimension",  // message name
  5,  // number of fields
  sizeof(vehicle_dimension_msgs__msg__VehicleDimension),
  vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_message_member_array,  // message members
  vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_init_function,  // function to initialize message memory (memory has to be allocated)
  vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_message_type_support_handle = {
  0,
  &vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_vehicle_dimension_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, vehicle_dimension_msgs, msg, VehicleDimension)() {
  vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_message_type_support_handle.typesupport_identifier) {
    vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &vehicle_dimension_msgs__msg__VehicleDimension__rosidl_typesupport_introspection_c__VehicleDimension_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
