// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleBoundingBox.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__BUILDER_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace vehicle_dimension_msgs
{

namespace msg
{

namespace builder
{

class Init_VehicleBoundingBox_ymax
{
public:
  explicit Init_VehicleBoundingBox_ymax(::vehicle_dimension_msgs::msg::VehicleBoundingBox & msg)
  : msg_(msg)
  {}
  ::vehicle_dimension_msgs::msg::VehicleBoundingBox ymax(::vehicle_dimension_msgs::msg::VehicleBoundingBox::_ymax_type arg)
  {
    msg_.ymax = std::move(arg);
    return std::move(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleBoundingBox msg_;
};

class Init_VehicleBoundingBox_xmax
{
public:
  explicit Init_VehicleBoundingBox_xmax(::vehicle_dimension_msgs::msg::VehicleBoundingBox & msg)
  : msg_(msg)
  {}
  Init_VehicleBoundingBox_ymax xmax(::vehicle_dimension_msgs::msg::VehicleBoundingBox::_xmax_type arg)
  {
    msg_.xmax = std::move(arg);
    return Init_VehicleBoundingBox_ymax(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleBoundingBox msg_;
};

class Init_VehicleBoundingBox_ymin
{
public:
  explicit Init_VehicleBoundingBox_ymin(::vehicle_dimension_msgs::msg::VehicleBoundingBox & msg)
  : msg_(msg)
  {}
  Init_VehicleBoundingBox_xmax ymin(::vehicle_dimension_msgs::msg::VehicleBoundingBox::_ymin_type arg)
  {
    msg_.ymin = std::move(arg);
    return Init_VehicleBoundingBox_xmax(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleBoundingBox msg_;
};

class Init_VehicleBoundingBox_xmin
{
public:
  explicit Init_VehicleBoundingBox_xmin(::vehicle_dimension_msgs::msg::VehicleBoundingBox & msg)
  : msg_(msg)
  {}
  Init_VehicleBoundingBox_ymin xmin(::vehicle_dimension_msgs::msg::VehicleBoundingBox::_xmin_type arg)
  {
    msg_.xmin = std::move(arg);
    return Init_VehicleBoundingBox_ymin(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleBoundingBox msg_;
};

class Init_VehicleBoundingBox_confidence
{
public:
  explicit Init_VehicleBoundingBox_confidence(::vehicle_dimension_msgs::msg::VehicleBoundingBox & msg)
  : msg_(msg)
  {}
  Init_VehicleBoundingBox_xmin confidence(::vehicle_dimension_msgs::msg::VehicleBoundingBox::_confidence_type arg)
  {
    msg_.confidence = std::move(arg);
    return Init_VehicleBoundingBox_xmin(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleBoundingBox msg_;
};

class Init_VehicleBoundingBox_class_id
{
public:
  Init_VehicleBoundingBox_class_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_VehicleBoundingBox_confidence class_id(::vehicle_dimension_msgs::msg::VehicleBoundingBox::_class_id_type arg)
  {
    msg_.class_id = std::move(arg);
    return Init_VehicleBoundingBox_confidence(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleBoundingBox msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::vehicle_dimension_msgs::msg::VehicleBoundingBox>()
{
  return vehicle_dimension_msgs::msg::builder::Init_VehicleBoundingBox_class_id();
}

}  // namespace vehicle_dimension_msgs

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX__BUILDER_HPP_
