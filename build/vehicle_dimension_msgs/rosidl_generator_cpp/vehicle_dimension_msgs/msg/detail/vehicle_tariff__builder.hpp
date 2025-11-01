// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from vehicle_dimension_msgs:msg/VehicleTariff.idl
// generated code does not contain a copyright notice

#ifndef VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__BUILDER_HPP_
#define VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "vehicle_dimension_msgs/msg/detail/vehicle_tariff__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace vehicle_dimension_msgs
{

namespace msg
{

namespace builder
{

class Init_VehicleTariff_dimension
{
public:
  explicit Init_VehicleTariff_dimension(::vehicle_dimension_msgs::msg::VehicleTariff & msg)
  : msg_(msg)
  {}
  ::vehicle_dimension_msgs::msg::VehicleTariff dimension(::vehicle_dimension_msgs::msg::VehicleTariff::_dimension_type arg)
  {
    msg_.dimension = std::move(arg);
    return std::move(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleTariff msg_;
};

class Init_VehicleTariff_currency
{
public:
  explicit Init_VehicleTariff_currency(::vehicle_dimension_msgs::msg::VehicleTariff & msg)
  : msg_(msg)
  {}
  Init_VehicleTariff_dimension currency(::vehicle_dimension_msgs::msg::VehicleTariff::_currency_type arg)
  {
    msg_.currency = std::move(arg);
    return Init_VehicleTariff_dimension(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleTariff msg_;
};

class Init_VehicleTariff_tariff
{
public:
  explicit Init_VehicleTariff_tariff(::vehicle_dimension_msgs::msg::VehicleTariff & msg)
  : msg_(msg)
  {}
  Init_VehicleTariff_currency tariff(::vehicle_dimension_msgs::msg::VehicleTariff::_tariff_type arg)
  {
    msg_.tariff = std::move(arg);
    return Init_VehicleTariff_currency(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleTariff msg_;
};

class Init_VehicleTariff_category
{
public:
  explicit Init_VehicleTariff_category(::vehicle_dimension_msgs::msg::VehicleTariff & msg)
  : msg_(msg)
  {}
  Init_VehicleTariff_tariff category(::vehicle_dimension_msgs::msg::VehicleTariff::_category_type arg)
  {
    msg_.category = std::move(arg);
    return Init_VehicleTariff_tariff(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleTariff msg_;
};

class Init_VehicleTariff_header
{
public:
  Init_VehicleTariff_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_VehicleTariff_category header(::vehicle_dimension_msgs::msg::VehicleTariff::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_VehicleTariff_category(msg_);
  }

private:
  ::vehicle_dimension_msgs::msg::VehicleTariff msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::vehicle_dimension_msgs::msg::VehicleTariff>()
{
  return vehicle_dimension_msgs::msg::builder::Init_VehicleTariff_header();
}

}  // namespace vehicle_dimension_msgs

#endif  // VEHICLE_DIMENSION_MSGS__MSG__DETAIL__VEHICLE_TARIFF__BUILDER_HPP_
