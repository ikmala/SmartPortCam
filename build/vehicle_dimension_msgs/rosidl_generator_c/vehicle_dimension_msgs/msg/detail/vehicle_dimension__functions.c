// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from vehicle_dimension_msgs:msg/VehicleDimension.idl
// generated code does not contain a copyright notice
#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"

bool
vehicle_dimension_msgs__msg__VehicleDimension__init(vehicle_dimension_msgs__msg__VehicleDimension * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    vehicle_dimension_msgs__msg__VehicleDimension__fini(msg);
    return false;
  }
  // length
  // width
  // height
  // class_id
  return true;
}

void
vehicle_dimension_msgs__msg__VehicleDimension__fini(vehicle_dimension_msgs__msg__VehicleDimension * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // length
  // width
  // height
  // class_id
}

bool
vehicle_dimension_msgs__msg__VehicleDimension__are_equal(const vehicle_dimension_msgs__msg__VehicleDimension * lhs, const vehicle_dimension_msgs__msg__VehicleDimension * rhs)
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
  // length
  if (lhs->length != rhs->length) {
    return false;
  }
  // width
  if (lhs->width != rhs->width) {
    return false;
  }
  // height
  if (lhs->height != rhs->height) {
    return false;
  }
  // class_id
  if (lhs->class_id != rhs->class_id) {
    return false;
  }
  return true;
}

bool
vehicle_dimension_msgs__msg__VehicleDimension__copy(
  const vehicle_dimension_msgs__msg__VehicleDimension * input,
  vehicle_dimension_msgs__msg__VehicleDimension * output)
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
  // length
  output->length = input->length;
  // width
  output->width = input->width;
  // height
  output->height = input->height;
  // class_id
  output->class_id = input->class_id;
  return true;
}

vehicle_dimension_msgs__msg__VehicleDimension *
vehicle_dimension_msgs__msg__VehicleDimension__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vehicle_dimension_msgs__msg__VehicleDimension * msg = (vehicle_dimension_msgs__msg__VehicleDimension *)allocator.allocate(sizeof(vehicle_dimension_msgs__msg__VehicleDimension), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(vehicle_dimension_msgs__msg__VehicleDimension));
  bool success = vehicle_dimension_msgs__msg__VehicleDimension__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
vehicle_dimension_msgs__msg__VehicleDimension__destroy(vehicle_dimension_msgs__msg__VehicleDimension * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    vehicle_dimension_msgs__msg__VehicleDimension__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
vehicle_dimension_msgs__msg__VehicleDimension__Sequence__init(vehicle_dimension_msgs__msg__VehicleDimension__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vehicle_dimension_msgs__msg__VehicleDimension * data = NULL;

  if (size) {
    data = (vehicle_dimension_msgs__msg__VehicleDimension *)allocator.zero_allocate(size, sizeof(vehicle_dimension_msgs__msg__VehicleDimension), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = vehicle_dimension_msgs__msg__VehicleDimension__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        vehicle_dimension_msgs__msg__VehicleDimension__fini(&data[i - 1]);
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
vehicle_dimension_msgs__msg__VehicleDimension__Sequence__fini(vehicle_dimension_msgs__msg__VehicleDimension__Sequence * array)
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
      vehicle_dimension_msgs__msg__VehicleDimension__fini(&array->data[i]);
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

vehicle_dimension_msgs__msg__VehicleDimension__Sequence *
vehicle_dimension_msgs__msg__VehicleDimension__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vehicle_dimension_msgs__msg__VehicleDimension__Sequence * array = (vehicle_dimension_msgs__msg__VehicleDimension__Sequence *)allocator.allocate(sizeof(vehicle_dimension_msgs__msg__VehicleDimension__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = vehicle_dimension_msgs__msg__VehicleDimension__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
vehicle_dimension_msgs__msg__VehicleDimension__Sequence__destroy(vehicle_dimension_msgs__msg__VehicleDimension__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    vehicle_dimension_msgs__msg__VehicleDimension__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
vehicle_dimension_msgs__msg__VehicleDimension__Sequence__are_equal(const vehicle_dimension_msgs__msg__VehicleDimension__Sequence * lhs, const vehicle_dimension_msgs__msg__VehicleDimension__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!vehicle_dimension_msgs__msg__VehicleDimension__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
vehicle_dimension_msgs__msg__VehicleDimension__Sequence__copy(
  const vehicle_dimension_msgs__msg__VehicleDimension__Sequence * input,
  vehicle_dimension_msgs__msg__VehicleDimension__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(vehicle_dimension_msgs__msg__VehicleDimension);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    vehicle_dimension_msgs__msg__VehicleDimension * data =
      (vehicle_dimension_msgs__msg__VehicleDimension *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!vehicle_dimension_msgs__msg__VehicleDimension__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          vehicle_dimension_msgs__msg__VehicleDimension__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!vehicle_dimension_msgs__msg__VehicleDimension__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
