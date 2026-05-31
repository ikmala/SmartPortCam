import pytest

from tariff_calculator.tariff_calculator import TariffCalculatorNode


@pytest.fixture
def node():
    import rclpy

    rclpy.init(args=None)
    instance = TariffCalculatorNode()
    yield instance
    instance.destroy_node()
    rclpy.shutdown()


def test_classify_by_length(node):
    category, tariff = node._classify_by_length(3000.0)
    assert category == "passenger_car"
    assert tariff > 0.0


def test_rule_order(node):
    category, _ = node._classify_by_length(20000.0)
    assert category in {"oversize", "unknown"}
