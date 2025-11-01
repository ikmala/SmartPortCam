// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleTariff.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__TRAITS_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "vehicle_dimension_msgs/msg/detail/vehicle_tariff__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'dimension'
#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__traits.hpp"

namespace vehicle_dimension_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const VehicleTariff & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: category
  {
    out << "category: ";
    rosidl_generator_traits::value_to_yaml(msg.category, out);
    out << ", ";
  }

  // member: tariff
  {
    out << "tariff: ";
    rosidl_generator_traits::value_to_yaml(msg.tariff, out);
    out << ", ";
  }

  // member: currency
  {
    out << "currency: ";
    rosidl_generator_traits::value_to_yaml(msg.currency, out);
    out << ", ";
  }

  // member: dimension
  {
    out << "dimension: ";
    to_flow_style_yaml(msg.dimension, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const VehicleTariff & msg,
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

  // member: category
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "category: ";
    rosidl_generator_traits::value_to_yaml(msg.category, out);
    out << "\n";
  }

  // member: tariff
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "tariff: ";
    rosidl_generator_traits::value_to_yaml(msg.tariff, out);
    out << "\n";
  }

  // member: currency
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "currency: ";
    rosidl_generator_traits::value_to_yaml(msg.currency, out);
    out << "\n";
  }

  // member: dimension
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "dimension:\n";
    to_block_style_yaml(msg.dimension, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const VehicleTariff & msg, bool use_flow_style = false)
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
  const vehicle_dimension_msgs::msg::VehicleTariff & msg,
  std::ostream & out, size_t indentation = 0)
{
  vehicle_dimension_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use vehicle_dimension_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const vehicle_dimension_msgs::msg::VehicleTariff & msg)
{
  return vehicle_dimension_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<vehicle_dimension_msgs::msg::VehicleTariff>()
{
  return "vehicle_dimension_msgs::msg::VehicleTariff";
}

template<>
inline const char * name<vehicle_dimension_msgs::msg::VehicleTariff>()
{
  return "vehicle_dimension_msgs/msg/VehicleTariff";
}

template<>
struct has_fixed_size<vehicle_dimension_msgs::msg::VehicleTariff>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<vehicle_dimension_msgs::msg::VehicleTariff>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<vehicle_dimension_msgs::msg::VehicleTariff>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__TRAITS_HPP_
