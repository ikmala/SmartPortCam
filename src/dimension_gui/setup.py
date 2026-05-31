from setuptools import find_packages, setup

package_name = "dimension_gui"

setup(
    name=package_name,
    version="0.1.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        (
            "share/ament_index/resource_index/packages",
            ["resource/" + package_name, "resource/dimension_system"],
        ),
        ("share/" + package_name, ["package.xml"]),
        ("share/" + package_name + "/launch", ["launch/dimension_system.launch.py"]),
        ("share/dimension_system/launch", ["launch/dimension_system.launch.py"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="ais",
    maintainer_email="ais@todo.todo",
    description="Tkinter-based dashboard for SmartPortCam dimensions.",
    license="MIT",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "dimension_gui = dimension_gui.dimension_gui:main",
        ],
    },
)
