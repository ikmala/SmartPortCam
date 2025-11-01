// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from vehicle_dimension_msgs:msg/VehicleBoundingBox.idl
// generated code does not contain a copyright notice
#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
vehicle_dimension_msgs__msg__VehicleBoundingBox__init(vehicle_dimension_msgs__msg__VehicleBoundingBox * msg)
{
  if (!msg) {
    return false;
  }
  // class_id
  // confidence
  // xmin
  // ymin
  // xmax
  // ymax
  return true;
}

void
vehicle_dimension_msgs__msg__VehicleBoundingBox__fini(vehicle_dimension_msgs__msg__VehicleBoundingBox * msg)
{
  if (!msg) {
    return;
  }
  // class_id
  // confidence
  // xmin
  // ymin
  // xmax
  // ymax
}

bool
vehicle_dimension_msgs__msg__VehicleBoundingBox__are_equal(const vehicle_dimension_msgs__msg__VehicleBoundingBox * lhs, const vehicle_dimension_msgs__msg__VehicleBoundingBox * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // class_id
  if (lhs->class_id != rhs->class_id) {
    return false;
  }
  // confidence
  if (lhs->confidence != rhs->confidence) {
    return false;
  }
  // xmin
  if (lhs->xmin != rhs->xmin) {
    return false;
  }
  // ymin
  if (lhs->ymin != rhs->ymin) {
    return false;
  }
  // xmax
  if (lhs->xmax != rhs->xmax) {
    return false;
  }
  // ymax
  if (lhs->ymax != rhs->ymax) {
    return false;
  }
  return true;
}

bool
vehicle_dimension_msgs__msg__VehicleBoundingBox__copy(
  const vehicle_dimension_msgs__msg__VehicleBoundingBox * input,
  vehicle_dimension_msgs__msg__VehicleBoundingBox * output)
{
  if (!input || !output) {
    return false;
  }
  // class_id
  output->class_id = input->class_id;
  // confidence
  output->confidence = input->confidence;
  // xmin
  output->xmin = input->xmin;
  // ymin
  output->ymin = input->ymin;
  // xmax
  output->xmax = input->xmax;
  // ymax
  output->ymax = input->ymax;
  return true;
}

vehicle_dimension_msgs__msg__VehicleBoundingBox *
vehicle_dimension_msgs__msg__VehicleBoundingBox__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vehicle_dimension_msgs__msg__VehicleBoundingBox * msg = (vehicle_dimension_msgs__msg__VehicleBoundingBox *)allocator.allocate(sizeof(vehicle_dimension_msgs__msg__VehicleBoundingBox), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(vehicle_dimension_msgs__msg__VehicleBoundingBox));
  bool success = vehicle_dimension_msgs__msg__VehicleBoundingBox__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
vehicle_dimension_msgs__msg__VehicleBoundingBox__destroy(vehicle_dimension_msgs__msg__VehicleBoundingBox * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    vehicle_dimension_msgs__msg__VehicleBoundingBox__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence__init(vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vehicle_dimension_msgs__msg__VehicleBoundingBox * data = NULL;

  if (size) {
    data = (vehicle_dimension_msgs__msg__VehicleBoundingBox *)allocator.zero_allocate(size, sizeof(vehicle_dimension_msgs__msg__VehicleBoundingBox), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = vehicle_dimension_msgs__msg__VehicleBoundingBox__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        vehicle_dimension_msgs__msg__VehicleBoundingBox__fini(&data[i - 1]);
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
vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence__fini(vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * array)
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
      vehicle_dimension_msgs__msg__VehicleBoundingBox__fini(&array->data[i]);
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

vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence *
vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * array = (vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence *)allocator.allocate(sizeof(vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence__destroy(vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence__are_equal(const vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * lhs, const vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!vehicle_dimension_msgs__msg__VehicleBoundingBox__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence__copy(
  const vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * input,
  vehicle_dimension_msgs__msg__VehicleBoundingBox__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(vehicle_dimension_msgs__msg__VehicleBoundingBox);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    vehicle_dimension_msgs__msg__VehicleBoundingBox * data =
      (vehicle_dimension_msgs__msg__VehicleBoundingBox *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!vehicle_dimension_msgs__msg__VehicleBoundingBox__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          vehicle_dimension_msgs__msg__VehicleBoundingBox__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!vehicle_dimension_msgs__msg__VehicleBoundingBox__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
