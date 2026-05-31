from setuptools import setup

package_name = "camera_manager"

setup(
    name=package_name,
    version="0.1.0",
    packages=[package_name],
    data_files=[
        ("share/ament_index/resource_index/packages", [f"resource/{package_name}"]),
        (f"share/{package_name}", ["package.xml"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="SmartPortCam",
    maintainer_email="maintainer@example.com",
    description="Deteksi dan manajemen kamera Intel RealSense.",
    license="Apache-2.0",
    entry_points={
        "console_scripts": [
            f"camera_manager = {package_name}.camera_manager:main",
        ],
    },
)
