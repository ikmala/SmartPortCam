// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from vehicle_dimension_msgs:msg/VehicleTariff.idl
// generated code does not contain a copyright notice
#include "vehicle_dimension_msgs/msg/detail/vehicle_tariff__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "vehicle_dimension_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "vehicle_dimension_msgs/msg/detail/vehicle_tariff__struct.h"
#include "vehicle_dimension_msgs/msg/detail/vehicle_tariff__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

#include "rosidl_runtime_c/string.h"  // category, currency
#include "rosidl_runtime_c/string_functions.h"  // category, currency
#include "std_msgs/msg/detail/header__functions.h"  // header
#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__functions.h"  // dimension

// forward declare type support functions
ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_vehicle_dimension_msgs
size_t get_serialized_size_std_msgs__msg__Header(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_vehicle_dimension_msgs
size_t max_serialized_size_std_msgs__msg__Header(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_vehicle_dimension_msgs
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, std_msgs, msg, Header)();
size_t get_serialized_size_vehicle_dimension_msgs__msg__VehicleDimension(
  const void * untyped_ros_message,
  size_t current_alignment);

size_t max_serialized_size_vehicle_dimension_msgs__msg__VehicleDimension(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, vehicle_dimension_msgs, msg, VehicleDimension)();


using _VehicleTariff__ros_msg_type = vehicle_dimension_msgs__msg__VehicleTariff;

static bool _VehicleTariff__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _VehicleTariff__ros_msg_type * ros_message = static_cast<const _VehicleTariff__ros_msg_type *>(untyped_ros_message);
  // Field name: header
  {
    const message_type_support_callbacks_t * callbacks =
      static_cast<const message_type_support_callbacks_t *>(
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
        rosidl_typesupport_fastrtps_c, std_msgs, msg, Header
      )()->data);
    if (!callbacks->cdr_serialize(
        &ros_message->header, cdr))
    {
      return false;
    }
  }

  // Field name: category
  {
    const rosidl_runtime_c__String * str = &ros_message->category;
    if (str->capacity == 0 || str->capacity <= str->size) {
      fprintf(stderr, "string capacity not greater than size\n");
      return false;
    }
    if (str->data[str->size] != '\0') {
      fprintf(stderr, "string not null-terminated\n");
      return false;
    }
    cdr << str->data;
  }

  // Field name: tariff
  {
    cdr << ros_message->tariff;
  }

  // Field name: currency
  {
    const rosidl_runtime_c__String * str = &ros_message->currency;
    if (str->capacity == 0 || str->capacity <= str->size) {
      fprintf(stderr, "string capacity not greater than size\n");
      return false;
    }
    if (str->data[str->size] != '\0') {
      fprintf(stderr, "string not null-terminated\n");
      return false;
    }
    cdr << str->data;
  }

  // Field name: dimension
  {
    const message_type_support_callbacks_t * callbacks =
      static_cast<const message_type_support_callbacks_t *>(
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
        rosidl_typesupport_fastrtps_c, vehicle_dimension_msgs, msg, VehicleDimension
      )()->data);
    if (!callbacks->cdr_serialize(
        &ros_message->dimension, cdr))
    {
      return false;
    }
  }

  return true;
}

static bool _VehicleTariff__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _VehicleTariff__ros_msg_type * ros_message = static_cast<_VehicleTariff__ros_msg_type *>(untyped_ros_message);
  // Field name: header
  {
    const message_type_support_callbacks_t * callbacks =
      static_cast<const message_type_support_callbacks_t *>(
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
        rosidl_typesupport_fastrtps_c, std_msgs, msg, Header
      )()->data);
    if (!callbacks->cdr_deserialize(
        cdr, &ros_message->header))
    {
      return false;
    }
  }

  // Field name: category
  {
    std::string tmp;
    cdr >> tmp;
    if (!ros_message->category.data) {
      rosidl_runtime_c__String__init(&ros_message->category);
    }
    bool succeeded = rosidl_runtime_c__String__assign(
      &ros_message->category,
      tmp.c_str());
    if (!succeeded) {
      fprintf(stderr, "failed to assign string into field 'category'\n");
      return false;
    }
  }

  // Field name: tariff
  {
    cdr >> ros_message->tariff;
  }

  // Field name: currency
  {
    std::string tmp;
    cdr >> tmp;
    if (!ros_message->currency.data) {
      rosidl_runtime_c__String__init(&ros_message->currency);
    }
    bool succeeded = rosidl_runtime_c__String__assign(
      &ros_message->currency,
      tmp.c_str());
    if (!succeeded) {
      fprintf(stderr, "failed to assign string into field 'currency'\n");
      return false;
    }
  }

  // Field name: dimension
  {
    const message_type_support_callbacks_t * callbacks =
      static_cast<const message_type_support_callbacks_t *>(
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
        rosidl_typesupport_fastrtps_c, vehicle_dimension_msgs, msg, VehicleDimension
      )()->data);
    if (!callbacks->cdr_deserialize(
        cdr, &ros_message->dimension))
    {
      return false;
    }
  }

  return true;
}  // NOLINT(readability/fn_size)

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_vehicle_dimension_msgs
size_t get_serialized_size_vehicle_dimension_msgs__msg__VehicleTariff(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _VehicleTariff__ros_msg_type * ros_message = static_cast<const _VehicleTariff__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name header

  current_alignment += get_serialized_size_std_msgs__msg__Header(
    &(ros_message->header), current_alignment);
  // field.name category
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message->category.size + 1);
  // field.name tariff
  {
    size_t item_size = sizeof(ros_message->tariff);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name currency
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message->currency.size + 1);
  // field.name dimension

  current_alignment += get_serialized_size_vehicle_dimension_msgs__msg__VehicleDimension(
    &(ros_message->dimension), current_alignment);

  return current_alignment - initial_alignment;
}

static uint32_t _VehicleTariff__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_vehicle_dimension_msgs__msg__VehicleTariff(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_vehicle_dimension_msgs
size_t max_serialized_size_vehicle_dimension_msgs__msg__VehicleTariff(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // member: header
  {
    size_t array_size = 1;


    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_std_msgs__msg__Header(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }
  // member: category
  {
    size_t array_size = 1;

    full_bounded = false;
    is_plain = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }
  // member: tariff
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }
  // member: currency
  {
    size_t array_size = 1;

    full_bounded = false;
    is_plain = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }
  // member: dimension
  {
    size_t array_size = 1;


    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_vehicle_dimension_msgs__msg__VehicleDimension(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = vehicle_dimension_msgs__msg__VehicleTariff;
    is_plain =
      (
      offsetof(DataType, dimension) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static size_t _VehicleTariff__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_vehicle_dimension_msgs__msg__VehicleTariff(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_VehicleTariff = {
  "vehicle_dimension_msgs::msg",
  "VehicleTariff",
  _VehicleTariff__cdr_serialize,
  _VehicleTariff__cdr_deserialize,
  _VehicleTariff__get_serialized_size,
  _VehicleTariff__max_serialized_size
};

static rosidl_message_type_support_t _VehicleTariff__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_VehicleTariff,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, vehicle_dimension_msgs, msg, VehicleTariff)() {
  return &_VehicleTariff__type_support;
}

#if defined(__cplusplus)
}
#endif
