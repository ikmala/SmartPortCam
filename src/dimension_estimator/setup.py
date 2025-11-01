from setuptools import find_packages, setup

package_name = "dimension_estimator"

setup(
    name=package_name,
    version="0.1.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        (
            "share/ament_index/resource_index/packages",
            ["resource/" + package_name],
        ),
        ("share/" + package_name, ["package.xml"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="ais",
    maintainer_email="ais@todo.todo",
    description="Estimate vehicle dimensions from aligned RealSense depth data.",
    license="MIT",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "dimension_estimator = dimension_estimator.dimension_estimator:main",
        ],
    },
)
