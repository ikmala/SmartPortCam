// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from vehicle_dimension_msgs:msg/VehicleTariff.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "vehicle_dimension_msgs/msg/detail/vehicle_tariff__struct.h"
#include "vehicle_dimension_msgs/msg/detail/vehicle_tariff__functions.h"

#include "rosidl_runtime_c/string.h"
#include "rosidl_runtime_c/string_functions.h"

ROSIDL_GENERATOR_C_IMPORT
bool std_msgs__msg__header__convert_from_py(PyObject * _pymsg, void * _ros_message);
ROSIDL_GENERATOR_C_IMPORT
PyObject * std_msgs__msg__header__convert_to_py(void * raw_ros_message);
bool vehicle_dimension_msgs__msg__vehicle_dimension__convert_from_py(PyObject * _pymsg, void * _ros_message);
PyObject * vehicle_dimension_msgs__msg__vehicle_dimension__convert_to_py(void * raw_ros_message);

ROSIDL_GENERATOR_C_EXPORT
bool vehicle_dimension_msgs__msg__vehicle_tariff__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[57];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("vehicle_dimension_msgs.msg._vehicle_tariff.VehicleTariff", full_classname_dest, 56) == 0);
  }
  vehicle_dimension_msgs__msg__VehicleTariff * ros_message = _ros_message;
  {  // header
    PyObject * field = PyObject_GetAttrString(_pymsg, "header");
    if (!field) {
      return false;
    }
    if (!std_msgs__msg__header__convert_from_py(field, &ros_message->header)) {
      Py_DECREF(field);
      return false;
    }
    Py_DECREF(field);
  }
  {  // category
    PyObject * field = PyObject_GetAttrString(_pymsg, "category");
    if (!field) {
      return false;
    }
    assert(PyUnicode_Check(field));
    PyObject * encoded_field = PyUnicode_AsUTF8String(field);
    if (!encoded_field) {
      Py_DECREF(field);
      return false;
    }
    rosidl_runtime_c__String__assign(&ros_message->category, PyBytes_AS_STRING(encoded_field));
    Py_DECREF(encoded_field);
    Py_DECREF(field);
  }
  {  // tariff
    PyObject * field = PyObject_GetAttrString(_pymsg, "tariff");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->tariff = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // currency
    PyObject * field = PyObject_GetAttrString(_pymsg, "currency");
    if (!field) {
      return false;
    }
    assert(PyUnicode_Check(field));
    PyObject * encoded_field = PyUnicode_AsUTF8String(field);
    if (!encoded_field) {
      Py_DECREF(field);
      return false;
    }
    rosidl_runtime_c__String__assign(&ros_message->currency, PyBytes_AS_STRING(encoded_field));
    Py_DECREF(encoded_field);
    Py_DECREF(field);
  }
  {  // dimension
    PyObject * field = PyObject_GetAttrString(_pymsg, "dimension");
    if (!field) {
      return false;
    }
    if (!vehicle_dimension_msgs__msg__vehicle_dimension__convert_from_py(field, &ros_message->dimension)) {
      Py_DECREF(field);
      return false;
    }
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * vehicle_dimension_msgs__msg__vehicle_tariff__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of VehicleTariff */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("vehicle_dimension_msgs.msg._vehicle_tariff");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "VehicleTariff");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  vehicle_dimension_msgs__msg__VehicleTariff * ros_message = (vehicle_dimension_msgs__msg__VehicleTariff *)raw_ros_message;
  {  // header
    PyObject * field = NULL;
    field = std_msgs__msg__header__convert_to_py(&ros_message->header);
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "header", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // category
    PyObject * field = NULL;
    field = PyUnicode_DecodeUTF8(
      ros_message->category.data,
      strlen(ros_message->category.data),
      "replace");
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "category", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // tariff
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->tariff);
    {
      int rc = PyObject_SetAttrString(_pymessage, "tariff", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // currency
    PyObject * field = NULL;
    field = PyUnicode_DecodeUTF8(
      ros_message->currency.data,
      strlen(ros_message->currency.data),
      "replace");
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "currency", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // dimension
    PyObject * field = NULL;
    field = vehicle_dimension_msgs__msg__vehicle_dimension__convert_to_py(&ros_message->dimension);
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "dimension", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
