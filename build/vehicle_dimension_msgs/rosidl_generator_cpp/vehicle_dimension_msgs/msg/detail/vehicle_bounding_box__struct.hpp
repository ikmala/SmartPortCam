// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleBoundingBox.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__STRUCT_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__vehicle_dimension_msgs__msg__VehicleBoundingBox __attribute__((deprecated))
#else
# define DEPRECATED__vehicle_dimension_msgs__msg__VehicleBoundingBox __declspec(deprecated)
#endif

namespace vehicle_dimension_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct VehicleBoundingBox_
{
  using Type = VehicleBoundingBox_<ContainerAllocator>;

  explicit VehicleBoundingBox_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->class_id = 0l;
      this->confidence = 0.0f;
      this->xmin = 0l;
      this->ymin = 0l;
      this->xmax = 0l;
      this->ymax = 0l;
    }
  }

  explicit VehicleBoundingBox_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->class_id = 0l;
      this->confidence = 0.0f;
      this->xmin = 0l;
      this->ymin = 0l;
      this->xmax = 0l;
      this->ymax = 0l;
    }
  }

  // field types and members
  using _class_id_type =
    int32_t;
  _class_id_type class_id;
  using _confidence_type =
    float;
  _confidence_type confidence;
  using _xmin_type =
    int32_t;
  _xmin_type xmin;
  using _ymin_type =
    int32_t;
  _ymin_type ymin;
  using _xmax_type =
    int32_t;
  _xmax_type xmax;
  using _ymax_type =
    int32_t;
  _ymax_type ymax;

  // setters for named parameter idiom
  Type & set__class_id(
    const int32_t & _arg)
  {
    this->class_id = _arg;
    return *this;
  }
  Type & set__confidence(
    const float & _arg)
  {
    this->confidence = _arg;
    return *this;
  }
  Type & set__xmin(
    const int32_t & _arg)
  {
    this->xmin = _arg;
    return *this;
  }
  Type & set__ymin(
    const int32_t & _arg)
  {
    this->ymin = _arg;
    return *this;
  }
  Type & set__xmax(
    const int32_t & _arg)
  {
    this->xmax = _arg;
    return *this;
  }
  Type & set__ymax(
    const int32_t & _arg)
  {
    this->ymax = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator> *;
  using ConstRawPtr =
    const vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__vehicle_dimension_msgs__msg__VehicleBoundingBox
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__vehicle_dimension_msgs__msg__VehicleBoundingBox
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const VehicleBoundingBox_ & other) const
  {
    if (this->class_id != other.class_id) {
      return false;
    }
    if (this->confidence != other.confidence) {
      return false;
    }
    if (this->xmin != other.xmin) {
      return false;
    }
    if (this->ymin != other.ymin) {
      return false;
    }
    if (this->xmax != other.xmax) {
      return false;
    }
    if (this->ymax != other.ymax) {
      return false;
    }
    return true;
  }
  bool operator!=(const VehicleBoundingBox_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct VehicleBoundingBox_

// alias to use template instance with default allocator
using VehicleBoundingBox =
  vehicle_dimension_msgs::msg::VehicleBoundingBox_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace vehicle_dimension_msgs

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__STRUCT_HPP_
