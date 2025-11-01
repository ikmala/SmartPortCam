// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleBoundingBox.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__TRAITS_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace vehicle_dimension_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const VehicleBoundingBox & msg,
  std::ostream & out)
{
  out << "{";
  // member: class_id
  {
    out << "class_id: ";
    rosidl_generator_traits::value_to_yaml(msg.class_id, out);
    out << ", ";
  }

  // member: confidence
  {
    out << "confidence: ";
    rosidl_generator_traits::value_to_yaml(msg.confidence, out);
    out << ", ";
  }

  // member: xmin
  {
    out << "xmin: ";
    rosidl_generator_traits::value_to_yaml(msg.xmin, out);
    out << ", ";
  }

  // member: ymin
  {
    out << "ymin: ";
    rosidl_generator_traits::value_to_yaml(msg.ymin, out);
    out << ", ";
  }

  // member: xmax
  {
    out << "xmax: ";
    rosidl_generator_traits::value_to_yaml(msg.xmax, out);
    out << ", ";
  }

  // member: ymax
  {
    out << "ymax: ";
    rosidl_generator_traits::value_to_yaml(msg.ymax, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const VehicleBoundingBox & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: class_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "class_id: ";
    rosidl_generator_traits::value_to_yaml(msg.class_id, out);
    out << "\n";
  }

  // member: confidence
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "confidence: ";
    rosidl_generator_traits::value_to_yaml(msg.confidence, out);
    out << "\n";
  }

  // member: xmin
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "xmin: ";
    rosidl_generator_traits::value_to_yaml(msg.xmin, out);
    out << "\n";
  }

  // member: ymin
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ymin: ";
    rosidl_generator_traits::value_to_yaml(msg.ymin, out);
    out << "\n";
  }

  // member: xmax
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "xmax: ";
    rosidl_generator_traits::value_to_yaml(msg.xmax, out);
    out << "\n";
  }

  // member: ymax
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ymax: ";
    rosidl_generator_traits::value_to_yaml(msg.ymax, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const VehicleBoundingBox & msg, bool use_flow_style = false)
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
  const vehicle_dimension_msgs::msg::VehicleBoundingBox & msg,
  std::ostream & out, size_t indentation = 0)
{
  vehicle_dimension_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use vehicle_dimension_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const vehicle_dimension_msgs::msg::VehicleBoundingBox & msg)
{
  return vehicle_dimension_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<vehicle_dimension_msgs::msg::VehicleBoundingBox>()
{
  return "vehicle_dimension_msgs::msg::VehicleBoundingBox";
}

template<>
inline const char * name<vehicle_dimension_msgs::msg::VehicleBoundingBox>()
{
  return "vehicle_dimension_msgs/msg/VehicleBoundingBox";
}

template<>
struct has_fixed_size<vehicle_dimension_msgs::msg::VehicleBoundingBox>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<vehicle_dimension_msgs::msg::VehicleBoundingBox>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<vehicle_dimension_msgs::msg::VehicleBoundingBox>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__TRAITS_HPP_
