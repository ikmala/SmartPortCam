// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from vehicle_dimension_msgs:msg/VehicleBoundingBoxArray.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX_ARRAY__STRUCT_H_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX_ARRAY__STRUCT_H_

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
// Member 'boxes'
#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box__struct.h"

/// Struct defined in msg/VehicleBoundingBoxArray in the package vehicle_dimension_msgs.
typedef struct vehicle_dimension_msgs__msg__VehicleBoundingBoxArray
{
  std_msgs__msg__Header header;
  vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence boxes;
} vehicle_dimension_msgs__msg__VehicleBoundingBoxArray;

// Struct for a sequence of vehicle_dimension_msgs__msg__VehicleBoundingBoxArray.
typedef struct vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__Sequence
{
  vehicle_dimension_msgs__msg__VehicleBoundingBoxArray * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} vehicle_dimension_msgs__msg__VehicleBoundingBoxArray__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX_ARRAY__STRUCT_H_
