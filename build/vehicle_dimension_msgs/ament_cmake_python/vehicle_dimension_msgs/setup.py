from setuptools import find_packages
from setuptools import setup

setup(
    name='vehicle_dimension_msgs',
    version='0.1.0',
    packages=find_packages(
        include=('vehicle_dimension_msgs', 'vehicle_dimension_msgs.*')),
)
