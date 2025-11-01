// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleBoundingBoxArray.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX_ARRAY__BUILDER_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX_ARRAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "vehicle_dimension_msgs/msg/detail/vehicle_bounding_box_array__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace vehicle_dimension_msgs
{

namespace msg
{

namespace builder
{

class Init_VehicleBoundingBoxArray_boxes
{
public:
  explicit Init_VehicleBoundingBoxArray_boxes(::vehicle_dimension_msgs::msg::VehicleBoundingBoxArray & msg)
  : msg_(msg)
  {}
  ::vehicle_dimension_msgs::msg::VehicleBoundingBoxArray boxes(::vehicle_dimension_msgs::msg::VehicleBoundingBoxArray::_boxes_type arg)
  {
    msg_.boxes = std::move(arg);
    return std::move(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleBoundingBoxArray msg_;
};

class Init_VehicleBoundingBoxArray_header
{
public:
  Init_VehicleBoundingBoxArray_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_VehicleBoundingBoxArray_boxes header(::vehicle_dimension_msgs::msg::VehicleBoundingBoxArray::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_VehicleBoundingBoxArray_boxes(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleBoundingBoxArray msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::vehicle_dimension_msgs::msg::VehicleBoundingBoxArray>()
{
  return vehicle_dimension_msgs::msg::builder::Init_VehicleBoundingBoxArray_header();
}

}  // namespace vehicle_dimension_msgs

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_BOUNDING_BOX_ARRAY__BUILDER_HPP_
