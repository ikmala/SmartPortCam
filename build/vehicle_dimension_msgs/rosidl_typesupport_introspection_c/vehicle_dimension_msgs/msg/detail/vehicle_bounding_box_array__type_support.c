// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from vehicle_dimension_msgs:msg/VehicleBoundingBoxArray.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box_array__rosidl_typesupport_introspection_c.h"
#include "vehicle_dimension_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box_array__functions.h"
#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box_array__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `boxes`
#include "vehicle_dimension_msgs/msg/vehicle_bounding_box.h"
// Member `boxes`
#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__init(message_memory);
}

void vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_fini_function(void * message_memory)
{
  vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__fini(message_memory);
}

size_t vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__size_function__VehicleBoundingBoxArray__boxes(
  const void * untyped_member)
{
  const vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * member =
    (const vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence *)(untyped_member);
  return member->size;
}

const void * vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__get_const_function__VehicleBoundingBoxArray__boxes(
  const void * untyped_member, size_t index)
{
  const vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * member =
    (const vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence *)(untyped_member);
  return &member->data[index];
}

void * vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__get_function__VehicleBoundingBoxArray__boxes(
  void * untyped_member, size_t index)
{
  vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * member =
    (vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence *)(untyped_member);
  return &member->data[index];
}

void vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__fetch_function__VehicleBoundingBoxArray__boxes(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const vehicle_dimension_msgs__msg__VehicleBoundingBox * item =
    ((const vehicle_dimension_msgs__msg__VehicleBoundingBox *)
    vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__get_const_function__VehicleBoundingBoxArray__boxes(untyped_member, index));
  vehicle_dimension_msgs__msg__VehicleBoundingBox * value =
    (vehicle_dimension_msgs__msg__VehicleBoundingBox *)(untyped_value);
  *value = *item;
}

void vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__assign_function__VehicleBoundingBoxArray__boxes(
  void * untyped_member, size_t index, const void * untyped_value)
{
  vehicle_dimension_msgs__msg__VehicleBoundingBox * item =
    ((vehicle_dimension_msgs__msg__VehicleBoundingBox *)
    vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__get_function__VehicleBoundingBoxArray__boxes(untyped_member, index));
  const vehicle_dimension_msgs__msg__VehicleBoundingBox * value =
    (const vehicle_dimension_msgs__msg__VehicleBoundingBox *)(untyped_value);
  *item = *value;
}

bool vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__resize_function__VehicleBoundingBoxArray__boxes(
  void * untyped_member, size_t size)
{
  vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * member =
    (vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence *)(untyped_member);
  vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence__fini(member);
  return vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_message_member_array[2] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleBoundingBoxArray, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "boxes",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(vehicle_dimension_msgs__msg__VehicleBoundingBoxArray, boxes),  // bytes offset in struct
    NULL,  // default value
    vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__size_function__VehicleBoundingBoxArray__boxes,  // size() function pointer
    vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__get_const_function__VehicleBoundingBoxArray__boxes,  // get_const(index) function pointer
    vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__get_function__VehicleBoundingBoxArray__boxes,  // get(index) function pointer
    vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__fetch_function__VehicleBoundingBoxArray__boxes,  // fetch(index, &value) function pointer
    vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__assign_function__VehicleBoundingBoxArray__boxes,  // assign(index, value) function pointer
    vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__resize_function__VehicleBoundingBoxArray__boxes  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_message_members = {
  "vehicle_dimension_msgs__msg",  // message namespace
  "VehicleBoundingBoxArray",  // message name
  2,  // number of fields
  sizeof(vehicle_dimension_msgs__msg__VehicleBoundingBoxArray),
  vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_message_member_array,  // message members
  vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_init_function,  // function to initialize message memory (memory has to be allocated)
  vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_message_type_support_handle = {
  0,
  &vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_vehicle_dimension_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, vehicle_dimension_msgs, msg, VehicleBoundingBoxArray)() {
  vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, vehicle_dimension_msgs, msg, VehicleBoundingBox)();
  if (!vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_message_type_support_handle.typesupport_identifier) {
    vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__rosidl_typesupport_introspection_c__VehicleBoundingBoxArray_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
