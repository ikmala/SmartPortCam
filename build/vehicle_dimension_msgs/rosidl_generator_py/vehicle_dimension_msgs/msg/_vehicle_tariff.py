# generated from rosidl_generator_py/resource/_idl.py.em
# with input from vehicle_dimension_msgs:msg/VehicleTariff.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_VehicleTariff(type):
    """Metaclass of message 'VehicleTariff'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('vehicle_dimension_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'vehicle_dimension_msgs.msg.VehicleTariff')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__vehicle_tariff
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__vehicle_tariff
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__vehicle_tariff
            cls._TYPE_SUPPORT = module.type_support_msg__msg__vehicle_tariff
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__vehicle_tariff

            from std_msgs.msg import Header
            if Header.__class__._TYPE_SUPPORT is None:
                Header.__class__.__import_type_support__()

            from vehicle_dimension_msgs.msg import VehicleDimension
            if VehicleDimension.__class__._TYPE_SUPPORT is None:
                VehicleDimension.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class VehicleTariff(metaclass=Metaclass_VehicleTariff):
    """Message class 'VehicleTariff'."""

    __slots__ = [
        '_header',
        '_category',
        '_tariff',
        '_currency',
        '_dimension',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'category': 'string',
        'tariff': 'double',
        'currency': 'string',
        'dimension': 'vehicle_dimension_msgs/VehicleDimension',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
        rosidl_parser.definition.NamespacedType(['vehicle_dimension_msgs', 'msg'], 'VehicleDimension'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        self.category = kwargs.get('category', str())
        self.tariff = kwargs.get('tariff', float())
        self.currency = kwargs.get('currency', str())
        from vehicle_dimension_msgs.msg import VehicleDimension
        self.dimension = kwargs.get('dimension', VehicleDimension())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.header != other.header:
            return False
        if self.category != other.category:
            return False
        if self.tariff != other.tariff:
            return False
        if self.currency != other.currency:
            return False
        if self.dimension != other.dimension:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def header(self):
        """Message field 'header'."""
        return self._header

    @header.setter
    def header(self, value):
        if __debug__:
            from std_msgs.msg import Header
            assert \
                isinstance(value, Header), \
                "The 'header' field must be a sub message of type 'Header'"
        self._header = value

    @builtins.property
    def category(self):
        """Message field 'category'."""
        return self._category

    @category.setter
    def category(self, value):
        if __debug__:
            assert \
                isinstance(value, str), \
                "The 'category' field must be of type 'str'"
        self._category = value

    @builtins.property
    def tariff(self):
        """Message field 'tariff'."""
        return self._tariff

    @tariff.setter
    def tariff(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'tariff' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'tariff' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._tariff = value

    @builtins.property
    def currency(self):
        """Message field 'currency'."""
        return self._currency

    @currency.setter
    def currency(self, value):
        if __debug__:
            assert \
                isinstance(value, str), \
                "The 'currency' field must be of type 'str'"
        self._currency = value

    @builtins.property
    def dimension(self):
        """Message field 'dimension'."""
        return self._dimension

    @dimension.setter
    def dimension(self, value):
        if __debug__:
            from vehicle_dimension_msgs.msg import VehicleDimension
            assert \
                isinstance(value, VehicleDimension), \
                "The 'dimension' field must be a sub message of type 'VehicleDimension'"
        self._dimension = value
