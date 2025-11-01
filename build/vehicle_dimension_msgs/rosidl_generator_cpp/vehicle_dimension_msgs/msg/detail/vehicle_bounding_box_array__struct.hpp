// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleBoundingBoxArray.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX_ARRAY__STRUCT_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX_ARRAY__STRUCT_HPP_

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
// Member 'boxes'
#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__vehicle_dimension_msgs__msg__VehicleBoundingBoxArray __attribute__((deprecated))
#else
# define DEPRECATED__vehicle_dimension_msgs__msg__VehicleBoundingBoxArray __declspec(deprecated)
#endif

namespace vehicle_dimension_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct VehicleBoundingBoxArray_
{
  using Type = VehicleBoundingBoxArray_<ContainerAllocator>;

  explicit VehicleBoundingBoxArray_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit VehicleBoundingBoxArray_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _boxes_type =
    std::vector<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator>>>;
  _boxes_type boxes;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__boxes(
    const std::vector<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<vehicle_dimension_msgs::msg::VehicleBoundingBox_<ContainerAllocator>>> & _arg)
  {
    this->boxes = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator> *;
  using ConstRawPtr =
    const vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__vehicle_dimension_msgs__msg__VehicleBoundingBoxArray
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__vehicle_dimension_msgs__msg__VehicleBoundingBoxArray
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const VehicleBoundingBoxArray_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->boxes != other.boxes) {
      return false;
    }
    return true;
  }
  bool operator!=(const VehicleBoundingBoxArray_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct VehicleBoundingBoxArray_

// alias to use template instance with default allocator
using VehicleBoundingBoxArray =
  vehicle_dimension_msgs::msg::VehicleBoundingBoxArray_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace vehicle_dimension_msgs

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX_ARRAY__STRUCT_HPP_
