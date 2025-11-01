// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from vehicle_dimension_msgs:msg/VehicleTariff.idl
// generated code does not contain a copyright notice
#include "vehicle_dimension_msgs/msg/detail/vehicle_tariff__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `category`
// Member `currency`
#include "rosidl_runtime_c/string_functions.h"
// Member `dimension`
#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__functions.h"

bool
vehicle_dimension_msgs__msg__VehicleTariff__init(vehicle_dimension_msgs__msg__VehicleTariff * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    vehicle_dimension_msgs__msg__VehicleTariff__fini(msg);
    return false;
  }
  // category
  if (!rosidl_runtime_c__String__init(&msg->category)) {
    vehicle_dimension_msgs__msg__VehicleTariff__fini(msg);
    return false;
  }
  // tariff
  // currency
  if (!rosidl_runtime_c__String__init(&msg->currency)) {
    vehicle_dimension_msgs__msg__VehicleTariff__fini(msg);
    return false;
  }
  // dimension
  if (!vehicle_dimension_msgs__msg__VehicleDimension__init(&msg->dimension)) {
    vehicle_dimension_msgs__msg__VehicleTariff__fini(msg);
    return false;
  }
  return true;
}

void
vehicle_dimension_msgs__msg__VehicleTariff__fini(vehicle_dimension_msgs__msg__VehicleTariff * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // category
  rosidl_runtime_c__String__fini(&msg->category);
  // tariff
  // currency
  rosidl_runtime_c__String__fini(&msg->currency);
  // dimension
  vehicle_dimension_msgs__msg__VehicleDimension__fini(&msg->dimension);
}

bool
vehicle_dimension_msgs__msg__VehicleTariff__are_equal(const vehicle_dimension_msgs__msg__VehicleTariff * lhs, const vehicle_dimension_msgs__msg__VehicleTariff * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // category
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->category), &(rhs->category)))
  {
    return false;
  }
  // tariff
  if (lhs->tariff != rhs->tariff) {
    return false;
  }
  // currency
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->currency), &(rhs->currency)))
  {
    return false;
  }
  // dimension
  if (!vehicle_dimension_msgs__msg__VehicleDimension__are_equal(
      &(lhs->dimension), &(rhs->dimension)))
  {
    return false;
  }
  return true;
}

bool
vehicle_dimension_msgs__msg__VehicleTariff__copy(
  const vehicle_dimension_msgs__msg__VehicleTariff * input,
  vehicle_dimension_msgs__msg__VehicleTariff * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // category
  if (!rosidl_runtime_c__String__copy(
      &(input->category), &(output->category)))
  {
    return false;
  }
  // tariff
  output->tariff = input->tariff;
  // currency
  if (!rosidl_runtime_c__String__copy(
      &(input->currency), &(output->currency)))
  {
    return false;
  }
  // dimension
  if (!vehicle_dimension_msgs__msg__VehicleDimension__copy(
      &(input->dimension), &(output->dimension)))
  {
    return false;
  }
  return true;
}

vehicle_dimension_msgs__msg__VehicleTariff *
vehicle_dimension_msgs__msg__VehicleTariff__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vehicle_dimension_msgs__msg__VehicleTariff * msg = (vehicle_dimension_msgs__msg__VehicleTariff *)allocator.allocate(sizeof(vehicle_dimension_msgs__msg__VehicleTariff), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(vehicle_dimension_msgs__msg__VehicleTariff));
  bool success = vehicle_dimension_msgs__msg__VehicleTariff__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
vehicle_dimension_msgs__msg__VehicleTariff__destroy(vehicle_dimension_msgs__msg__VehicleTariff * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    vehicle_dimension_msgs__msg__VehicleTariff__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
vehicle_dimension_msgs__msg__VehicleTariff__Sequence__init(vehicle_dimension_msgs__msg__VehicleTariff__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vehicle_dimension_msgs__msg__VehicleTariff * data = NULL;

  if (size) {
    data = (vehicle_dimension_msgs__msg__VehicleTariff *)allocator.zero_allocate(size, sizeof(vehicle_dimension_msgs__msg__VehicleTariff), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = vehicle_dimension_msgs__msg__VehicleTariff__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        vehicle_dimension_msgs__msg__VehicleTariff__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
vehicle_dimension_msgs__msg__VehicleTariff__Sequence__fini(vehicle_dimension_msgs__msg__VehicleTariff__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      vehicle_dimension_msgs__msg__VehicleTariff__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

vehicle_dimension_msgs__msg__VehicleTariff__Sequence *
vehicle_dimension_msgs__msg__VehicleTariff__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vehicle_dimension_msgs__msg__VehicleTariff__Sequence * array = (vehicle_dimension_msgs__msg__VehicleTariff__Sequence *)allocator.allocate(sizeof(vehicle_dimension_msgs__msg__VehicleTariff__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = vehicle_dimension_msgs__msg__VehicleTariff__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
vehicle_dimension_msgs__msg__VehicleTariff__Sequence__destroy(vehicle_dimension_msgs__msg__VehicleTariff__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    vehicle_dimension_msgs__msg__VehicleTariff__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
vehicle_dimension_msgs__msg__VehicleTariff__Sequence__are_equal(const vehicle_dimension_msgs__msg__VehicleTariff__Sequence * lhs, const vehicle_dimension_msgs__msg__VehicleTariff__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!vehicle_dimension_msgs__msg__VehicleTariff__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
vehicle_dimension_msgs__msg__VehicleTariff__Sequence__copy(
  const vehicle_dimension_msgs__msg__VehicleTariff__Sequence * input,
  vehicle_dimension_msgs__msg__VehicleTariff__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(vehicle_dimension_msgs__msg__VehicleTariff);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    vehicle_dimension_msgs__msg__VehicleTariff * data =
      (vehicle_dimension_msgs__msg__VehicleTariff *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!vehicle_dimension_msgs__msg__VehicleTariff__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          vehicle_dimension_msgs__msg__VehicleTariff__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!vehicle_dimension_msgs__msg__VehicleTariff__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
