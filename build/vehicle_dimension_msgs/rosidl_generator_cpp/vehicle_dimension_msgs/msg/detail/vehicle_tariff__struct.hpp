// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleTariff.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__STRUCT_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__STRUCT_HPP_

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
// Member 'dimension'
#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__vehicle_dimension_msgs__msg__VehicleTariff __attribute__((deprecated))
#else
# define DEPRECATED__vehicle_dimension_msgs__msg__VehicleTariff __declspec(deprecated)
#endif

namespace vehicle_dimension_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct VehicleTariff_
{
  using Type = VehicleTariff_<ContainerAllocator>;

  explicit VehicleTariff_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    dimension(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->category = "";
      this->tariff = 0.0;
      this->currency = "";
    }
  }

  explicit VehicleTariff_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    category(_alloc),
    currency(_alloc),
    dimension(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->category = "";
      this->tariff = 0.0;
      this->currency = "";
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _category_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _category_type category;
  using _tariff_type =
    double;
  _tariff_type tariff;
  using _currency_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _currency_type currency;
  using _dimension_type =
    vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator>;
  _dimension_type dimension;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__category(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->category = _arg;
    return *this;
  }
  Type & set__tariff(
    const double & _arg)
  {
    this->tariff = _arg;
    return *this;
  }
  Type & set__currency(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->currency = _arg;
    return *this;
  }
  Type & set__dimension(
    const vehicle_dimension_msgs::msg::VehicleDimension_<ContainerAllocator> & _arg)
  {
    this->dimension = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator> *;
  using ConstRawPtr =
    const vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__vehicle_dimension_msgs__msg__VehicleTariff
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__vehicle_dimension_msgs__msg__VehicleTariff
    std::shared_ptr<vehicle_dimension_msgs::msg::VehicleTariff_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const VehicleTariff_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->category != other.category) {
      return false;
    }
    if (this->tariff != other.tariff) {
      return false;
    }
    if (this->currency != other.currency) {
      return false;
    }
    if (this->dimension != other.dimension) {
      return false;
    }
    return true;
  }
  bool operator!=(const VehicleTariff_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct VehicleTariff_

// alias to use template instance with default allocator
using VehicleTariff =
  vehicle_dimension_msgs::msg::VehicleTariff_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace vehicle_dimension_msgs

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__STRUCT_HPP_
