// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from vehicle_dimension_msgs:msg/VehicleBoundingBox.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__STRUCT_H_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/VehicleBoundingBox in the package vehicle_dimension_msgs.
typedef struct vehicle_dimension_msgs__msg__VehicleBoundingBox
{
  int32_t class_id;
  float confidence;
  int32_t xmin;
  int32_t ymin;
  int32_t xmax;
  int32_t ymax;
} vehicle_dimension_msgs__msg__VehicleBoundingBox;

// Struct for a sequence of vehicle_dimension_msgs__msg__VehicleBoundingBox.
typedef struct vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence
{
  vehicle_dimension_msgs__msg__VehicleBoundingBox * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__STRUCT_H_
