// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleDimension.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__TRAITS_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace vehicle_dimension_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const VehicleDimension & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: length
  {
    out << "length: ";
    rosidl_generator_traits::value_to_yaml(msg.length, out);
    out << ", ";
  }

  // member: width
  {
    out << "width: ";
    rosidl_generator_traits::value_to_yaml(msg.width, out);
    out << ", ";
  }

  // member: height
  {
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << ", ";
  }

  // member: class_id
  {
    out << "class_id: ";
    rosidl_generator_traits::value_to_yaml(msg.class_id, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const VehicleDimension & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: length
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "length: ";
    rosidl_generator_traits::value_to_yaml(msg.length, out);
    out << "\n";
  }

  // member: width
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "width: ";
    rosidl_generator_traits::value_to_yaml(msg.width, out);
    out << "\n";
  }

  // member: height
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << "\n";
  }

  // member: class_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "class_id: ";
    rosidl_generator_traits::value_to_yaml(msg.class_id, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const VehicleDimension & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace vehicle_dimension_msgs

namespace rosidl_generator_traits
{

[[deprecated("use vehicle_dimension_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const vehicle_dimension_msgs::msg::VehicleDimension & msg,
  std::ostream & out, size_t indentation = 0)
{
  vehicle_dimension_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use vehicle_dimension_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const vehicle_dimension_msgs::msg::VehicleDimension & msg)
{
  return vehicle_dimension_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<vehicle_dimension_msgs::msg::VehicleDimension>()
{
  return "vehicle_dimension_msgs::msg::VehicleDimension";
}

template<>
inline const char * name<vehicle_dimension_msgs::msg::VehicleDimension>()
{
  return "vehicle_dimension_msgs/msg/VehicleDimension";
}

template<>
struct has_fixed_size<vehicle_dimension_msgs::msg::VehicleDimension>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<vehicle_dimension_msgs::msg::VehicleDimension>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<vehicle_dimension_msgs::msg::VehicleDimension>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__TRAITS_HPP_
