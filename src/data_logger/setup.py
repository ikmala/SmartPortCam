from setuptools import setup

package_name = "data_logger"

setup(
    name=package_name,
    version="0.1.0",
    packages=[package_name],
    data_files=[
        ("share/ament_index/resource_index/packages", [f"resource/{package_name}"]),
        (f"share/{package_name}", ["package.xml"]),
    ],
    install_requires=["setuptools", "psycopg2-binary"],
    zip_safe=True,
    maintainer="SmartPortCam",
    maintainer_email="maintainer@example.com",
    description="Simpan data dimensi dan tarif kendaraan ke PostgreSQL.",
    license="Apache-2.0",
    entry_points={
        "console_scripts": [
            f"data_logger = {package_name}.data_logger:main",
        ],
    },
)
