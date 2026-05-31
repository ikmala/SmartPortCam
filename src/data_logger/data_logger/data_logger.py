"""ROS 2 node untuk mencatat data kendaraan ke PostgreSQL."""

import datetime
import os
import threading
from typing import Optional

import psycopg2
import psycopg2.extras
import rclpy
from rclpy.node import Node

from vehicle_dimension_msgs.msg import VehicleTariff


class DataLoggerNode(Node):
    """Subscribe ke /vehicle/tariff_info dan simpan setiap record ke PostgreSQL."""

    _CREATE_TABLE = """
        CREATE TABLE IF NOT EXISTS vehicle_records (
            id             SERIAL PRIMARY KEY,
            recorded_at    TIMESTAMPTZ NOT NULL DEFAULT NOW(),
            ros_timestamp  TIMESTAMPTZ,
            gate_id        VARCHAR(32)  NOT NULL DEFAULT 'default',
            class_id       INTEGER,
            category       VARCHAR(50),
            length_mm      DOUBLE PRECISION,
            width_mm       DOUBLE PRECISION,
            height_mm      DOUBLE PRECISION,
            tariff         DOUBLE PRECISION,
            currency       VARCHAR(10)
        );
        CREATE INDEX IF NOT EXISTS idx_vehicle_records_recorded_at
            ON vehicle_records (recorded_at DESC);
        CREATE INDEX IF NOT EXISTS idx_vehicle_records_gate_id
            ON vehicle_records (gate_id);
    """

    _INSERT = """
        INSERT INTO vehicle_records
            (ros_timestamp, gate_id, class_id, category,
             length_mm, width_mm, height_mm, tariff, currency)
        VALUES (%(ros_timestamp)s, %(gate_id)s, %(class_id)s, %(category)s,
                %(length_mm)s, %(width_mm)s, %(height_mm)s,
                %(tariff)s, %(currency)s)
    """

    def __init__(self) -> None:
        super().__init__("data_logger")
        self._declare_parameters()

        self._conn: Optional[psycopg2.extensions.connection] = None
        self._lock = threading.Lock()

        self._gate_id = str(self.get_parameter("gate_id").value) or "default"
        self._connect()
        self._ensure_schema()

        self.create_subscription(
            VehicleTariff,
            "/vehicle/tariff_info",
            self._tariff_callback,
            10,
        )
        # Timer untuk cek koneksi setiap 30 detik dan reconnect bila perlu
        self.create_timer(30.0, self._heartbeat)
        self.get_logger().info("Data logger node siap — menyimpan ke PostgreSQL.")

    def _declare_parameters(self) -> None:
        self.declare_parameter("db_host",  os.environ.get("POSTGRES_HOST",     "localhost"))
        self.declare_parameter("db_port",  int(os.environ.get("POSTGRES_PORT", "5432")))
        self.declare_parameter("db_name",  os.environ.get("POSTGRES_DB",       "smartportcam"))
        self.declare_parameter("db_user",  os.environ.get("POSTGRES_USER",     "smartport"))
        self.declare_parameter("db_password", os.environ.get("POSTGRES_PASSWORD", "smartport123"))
        # gate_id diisi oleh gate_process.py via -p gate_id:=gate_1
        # atau via env var SMARTPORT_GATE_ID (fallback)
        self.declare_parameter(
            "gate_id",
            os.environ.get("SMARTPORT_GATE_ID", "default"),
        )

    def _dsn(self) -> dict:
        return {
            "host": self.get_parameter("db_host").value,
            "port": int(self.get_parameter("db_port").value),
            "dbname": self.get_parameter("db_name").value,
            "user": self.get_parameter("db_user").value,
            "password": self.get_parameter("db_password").value,
            "connect_timeout": 5,
        }

    def _connect(self) -> bool:
        dsn = self._dsn()
        try:
            conn = psycopg2.connect(**dsn)
            conn.autocommit = True
            with self._lock:
                if self._conn and not self._conn.closed:
                    self._conn.close()
                self._conn = conn
            self.get_logger().info(
                f"Terhubung ke PostgreSQL {dsn['host']}:{dsn['port']}/{dsn['dbname']}"
            )
            return True
        except psycopg2.Error as exc:
            self.get_logger().error(f"Gagal konek ke PostgreSQL: {exc}")
            return False

    def _ensure_schema(self) -> None:
        with self._lock:
            conn = self._conn
        if conn is None or conn.closed:
            return
        try:
            with conn.cursor() as cur:
                cur.execute(self._CREATE_TABLE)
            self.get_logger().info("Skema database siap.")
        except psycopg2.Error as exc:
            self.get_logger().error(f"Gagal membuat tabel: {exc}")

    def _tariff_callback(self, msg: VehicleTariff) -> None:
        ros_ts: Optional[datetime.datetime] = None
        if msg.header.stamp.sec > 0:
            epoch_s = msg.header.stamp.sec + msg.header.stamp.nanosec * 1e-9
            ros_ts = datetime.datetime.fromtimestamp(epoch_s, tz=datetime.timezone.utc)

        record = {
            "ros_timestamp": ros_ts,
            "gate_id":   self._gate_id,
            "class_id":  msg.dimension.class_id,
            "category":  msg.category,
            "length_mm": msg.dimension.length,
            "width_mm":  msg.dimension.width,
            "height_mm": msg.dimension.height,
            "tariff":    msg.tariff,
            "currency":  msg.currency,
        }

        with self._lock:
            conn = self._conn

        if conn is None or conn.closed:
            self.get_logger().warning("Tidak ada koneksi DB — record dilewati.")
            return

        try:
            with conn.cursor() as cur:
                cur.execute(self._INSERT, record)
            self.get_logger().debug(
                f"Record disimpan: {msg.category} tarif={msg.tariff:.0f} {msg.currency}"
            )
        except psycopg2.Error as exc:
            self.get_logger().error(f"Gagal insert record: {exc}")
            self._connect()

    def _heartbeat(self) -> None:
        with self._lock:
            conn = self._conn
        if conn is None or conn.closed:
            self.get_logger().warning("Koneksi DB hilang — mencoba reconnect…")
            if self._connect():
                self._ensure_schema()
        else:
            try:
                conn.cursor().execute("SELECT 1")
            except psycopg2.Error:
                self.get_logger().warning("DB ping gagal — mencoba reconnect…")
                self._connect()

    def destroy_node(self) -> bool:
        with self._lock:
            if self._conn and not self._conn.closed:
                self._conn.close()
        return super().destroy_node()


def main(args=None) -> None:
    rclpy.init(args=args)
    node = DataLoggerNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
