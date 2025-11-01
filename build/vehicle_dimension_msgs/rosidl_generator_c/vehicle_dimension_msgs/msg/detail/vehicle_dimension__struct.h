// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from vehicle_dimension_msgs:msg/VehicleDimension.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__STRUCT_H_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"

/// Struct defined in msg/VehicleDimension in the package vehicle_dimension_msgs.
typedef struct vehicle_dimension_msgs__msg__VehicleDimension
{
  std_msgs__msg__Header header;
  double length;
  double width;
  double height;
  int32_t class_id;
} vehicle_dimension_msgs__msg__VehicleDimension;

// Struct for a sequence of vehicle_dimension_msgs__msg__VehicleDimension.
typedef struct vehicle_dimension_msgs__msg__VehicleDimension__Sequence
{
  vehicle_dimension_msgs__msg__VehicleDimension * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} vehicle_dimension_msgs__msg__VehicleDimension__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__STRUCT_H_
