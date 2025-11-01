// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleDimension.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__STRUCT_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__vehicle_dimension_msgs__msg__VehicleDimension __attribute__((deprecated))
#else
# define DEPRECATED__vehicle_dimension_msgs__msg__VehicleDimension __declspec(deprecated)
#endif

namespace vehicle_dimension_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct VehicleDimension_
{
  using Type = VehicleDimension_<ContainerAllocator>;

  explicit VehicleDimension_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->length = 0.0;
      this->width = 0.0;
      this->height = 0.0;
      this->class_id = 0l;
    }
  }

  explicit VehicleDimension_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->length = 0.0;
      this->width = 0.0;
      this->height = 0.0;
      this->class_id = 0l;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _length_type =
    double;
  _length_type length;
  using _width_type =
    double;
  _width_type width;
  using _height_type =
    double;
  _height_type height;
  using _class_id_type =
    int32_t;
  _class_id_type class_id;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__length(
    const double & _arg)
  {
    this->length = _arg;
    return *this;
  }
  Type & set__width(
    const double & _arg)
  {
    this->width = _arg;
    return *this;
  }
  Type & set__height(
    const double & _arg)
  {
    this->height = _arg;
    return *this;
  }
  Type & set__class_id(
    const int32_t & _arg)
  {
    this->class_id = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator> *;
  using ConstRawPtr =
    const vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__vehicle_dimension_msgs__msg__VehicleDimension
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__vehicle_dimension_msgs__msg__VehicleDimension
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const VehicleDimension_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->length != other.length) {
      return false;
    }
    if (this->width != other.width) {
      return false;
    }
    if (this->height != other.height) {
      return false;
    }
    if (this->class_id != other.class_id) {
      return false;
    }
    return true;
  }
  bool operator!=(const VehicleDimension_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct VehicleDimension_

// alias to use template instance with default allocator
using VehicleDimension =
  vehicle_dimension_msgs::msg::VehicleDimension_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace vehicle_dimension_msgs

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__STRUCT_HPP_
