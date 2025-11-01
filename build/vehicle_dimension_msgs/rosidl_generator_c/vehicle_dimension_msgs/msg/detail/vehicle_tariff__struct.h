// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from vehicle_dimension_msgs:msg/VehicleTariff.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__STRUCT_H_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__STRUCT_H_

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
// Member 'category'
// Member 'currency'
#include "rosidl_runtime_c/string.h"
// Member 'dimension'
#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__struct.h"

/// Struct defined in msg/VehicleTariff in the package vehicle_dimension_msgs.
typedef struct vehicle_dimension_msgs__msg__VehicleTariff
{
  std_msgs__msg__Header header;
  rosidl_runtime_c__String category;
  double tariff;
  rosidl_runtime_c__String currency;
  vehicle_dimension_msgs__msg__VehicleDimension dimension;
} vehicle_dimension_msgs__msg__VehicleTariff;

// Struct for a sequence of vehicle_dimension_msgs__msg__VehicleTariff.
typedef struct vehicle_dimension_msgs__msg__VehicleTariff__Sequence
{
  vehicle_dimension_msgs__msg__VehicleTariff * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} vehicle_dimension_msgs__msg__VehicleTariff__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__STRUCT_H_
