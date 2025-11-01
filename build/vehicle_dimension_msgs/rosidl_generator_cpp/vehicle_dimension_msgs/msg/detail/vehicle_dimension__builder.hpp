// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleDimension.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__BUILDER_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "vehicle_dimension_msgs/msg/detail/vehicle_dimension__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace vehicle_dimension_msgs
{

namespace msg
{

namespace builder
{

class Init_VehicleDimension_class_id
{
public:
  explicit Init_VehicleDimension_class_id(::vehicle_dimension_msgs::msg::VehicleDimension & msg)
  : msg_(msg)
  {}
  ::vehicle_dimension_msgs::msg::VehicleDimension class_id(::vehicle_dimension_msgs::msg::VehicleDimension::_class_id_type arg)
  {
    msg_.class_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleDimension msg_;
};

class Init_VehicleDimension_height
{
public:
  explicit Init_VehicleDimension_height(::vehicle_dimension_msgs::msg::VehicleDimension & msg)
  : msg_(msg)
  {}
  Init_VehicleDimension_class_id height(::vehicle_dimension_msgs::msg::VehicleDimension::_height_type arg)
  {
    msg_.height = std::move(arg);
    return Init_VehicleDimension_class_id(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleDimension msg_;
};

class Init_VehicleDimension_width
{
public:
  explicit Init_VehicleDimension_width(::vehicle_dimension_msgs::msg::VehicleDimension & msg)
  : msg_(msg)
  {}
  Init_VehicleDimension_height width(::vehicle_dimension_msgs::msg::VehicleDimension::_width_type arg)
  {
    msg_.width = std::move(arg);
    return Init_VehicleDimension_height(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleDimension msg_;
};

class Init_VehicleDimension_length
{
public:
  explicit Init_VehicleDimension_length(::vehicle_dimension_msgs::msg::VehicleDimension & msg)
  : msg_(msg)
  {}
  Init_VehicleDimension_width length(::vehicle_dimension_msgs::msg::VehicleDimension::_length_type arg)
  {
    msg_.length = std::move(arg);
    return Init_VehicleDimension_width(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleDimension msg_;
};

class Init_VehicleDimension_header
{
public:
  Init_VehicleDimension_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_VehicleDimension_length header(::vehicle_dimension_msgs::msg::VehicleDimension::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_VehicleDimension_length(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleDimension msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::vehicle_dimension_msgs::msg::VehicleDimension>()
{
  return vehicle_dimension_msgs::msg::builder::Init_VehicleDimension_header();
}

}  // namespace vehicle_dimension_msgs

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_DIMENSION__BUILDER_HPP_
