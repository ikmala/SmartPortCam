"""Node ROS 2 untuk menghitung klasifikasi kendaraan dan tarif otomatis."""

from queue import Empty, Queue
from threading import Event, Thread
from typing import Dict, Iterable, List, Optional, Tuple

try:
    import tkinter as tk
    from tkinter import ttk
except ImportError:  # pragma: no cover - Tkinter opsional.
    tk = None
    ttk = None

import rclpy
from rclpy.node import Node

from vehicle_dimension_msgs.msg import VehicleDimension, VehicleTariff

DEFAULT_CLASS_TARIFFS: Tuple[Dict[str, float], ...] = (
    {"class_id": 3, "category": "motorcycle", "tariff": 20000.0},
    {"class_id": 2, "category": "passenger_car", "tariff": 50000.0},
    {"class_id": 5, "category": "bus", "tariff": 120000.0},
    {"class_id": 7, "category": "heavy_truck", "tariff": 150000.0},
)

DEFAULT_DIMENSION_RULES: Tuple[Dict[str, float], ...] = (
    {"name": "motorcycle", "max_length_mm": 2500.0, "tariff": 20000.0},
    {"name": "passenger_car", "max_length_mm": 5500.0, "tariff": 50000.0},
    {"name": "light_truck", "max_length_mm": 8000.0, "tariff": 90000.0},
    {"name": "bus", "max_length_mm": 12000.0, "tariff": 120000.0},
    {"name": "oversize", "max_length_mm": -1.0, "tariff": 150000.0},
)


class TariffCalculatorNode(Node):
    """Hitung tarif berdasarkan data dimensi kendaraan."""

    def __init__(self) -> None:
        super().__init__("tariff_calculator")
        self.declare_parameter("enable_gui", bool(tk is not None))
        self.declare_parameter("stale_timeout_sec", 3.0)
        self.declare_parameter("currency", "IDR")
        self.declare_parameter("dimension_rules", list(DEFAULT_DIMENSION_RULES))
        self.declare_parameter("class_tariffs", list(DEFAULT_CLASS_TARIFFS))

        self._enable_gui = bool(self.get_parameter("enable_gui").value and tk)
        self._stale_timeout = float(self.get_parameter("stale_timeout_sec").value)
        self._currency = str(self.get_parameter("currency").value)
        self._dimension_rules = self._load_dimension_rules()
        self._class_tariffs = self._load_class_tariffs()

        self._update_queue: Queue = Queue()
        self._shutdown_event = Event()
        self._gui_thread: Optional[Thread] = None
        self._latest_tariff: Optional[VehicleTariff] = None
        self._last_update_time = None

        self.create_subscription(
            VehicleDimension,
            "/vehicle/dimension_info",
            self._dimension_callback,
            10,
        )
        self._tariff_pub = self.create_publisher(
            VehicleTariff,
            "/vehicle/tariff_info",
            10,
        )
        self.create_timer(1.0, self._status_timer_callback)

        if self._enable_gui:
            self._start_gui()
        else:
            if tk is None:
                self.get_logger().warning(
                    "Tkinter tidak tersedia, menggunakan log konsol sebagai dashboard."
                )
            else:
                self.get_logger().info(
                    "GUI dimatikan via parameter, menampilkan hasil di log."
                )

    def _load_dimension_rules(self) -> List[Dict[str, float]]:
        """Konversi parameter rules dimensi menjadi list yang siap digunakan."""
        rules_param = self.get_parameter("dimension_rules").value
        rules: List[Dict[str, float]] = []
        if isinstance(rules_param, Iterable):
            for entry in rules_param:
                if not isinstance(entry, dict):
                    continue
                name = str(entry.get("name", "unknown"))
                try:
                    max_length = float(entry.get("max_length_mm", -1.0))
                    tariff = float(entry.get("tariff", 0.0))
                except (TypeError, ValueError):
                    continue
                rules.append(
                    {
                        "name": name,
                        "max_length_mm": max_length,
                        "tariff": tariff,
                    }
                )
        if not rules:
            for default in DEFAULT_DIMENSION_RULES:
                rules.append(default.copy())
        rules.sort(
            key=lambda item: (
                float("inf") if item["max_length_mm"] < 0.0 else item["max_length_mm"]
            )
        )
        return rules

    def _load_class_tariffs(self) -> Dict[int, Dict[str, float]]:
        """Muat pemetaan class_id terhadap kategori dan tarif default."""
        mapping: Dict[int, Dict[str, float]] = {}
        class_param = self.get_parameter("class_tariffs").value
        if isinstance(class_param, Iterable):
            for entry in class_param:
                if not isinstance(entry, dict) or "class_id" not in entry:
                    continue
                try:
                    class_id = int(entry["class_id"])
                    category = str(entry.get("category", f"class_{class_id}"))
                    tariff = float(entry.get("tariff", 0.0))
                except (TypeError, ValueError):
                    continue
                mapping[class_id] = {"category": category, "tariff": tariff}
        if not mapping:
            for default in DEFAULT_CLASS_TARIFFS:
                mapping[int(default["class_id"])] = {
                    "category": str(default["category"]),
                    "tariff": float(default["tariff"]),
                }
        return mapping

    def _start_gui(self) -> None:
        self._gui_thread = Thread(target=self._run_gui_loop, daemon=True)
        self._gui_thread.start()

    def _run_gui_loop(self) -> None:
        if tk is None:
            return
        root = tk.Tk()
        root.title("SmartPortCam - Tarif Otomatis")
        root.geometry("420x240")

        self._category_var = tk.StringVar(value="Kategori: --")
        self._tariff_var = tk.StringVar(value="Tarif: --")
        self._currency_var = tk.StringVar(value=f"Mata uang: {self._currency}")
        self._length_var = tk.StringVar(value="Panjang: -- mm")
        self._width_var = tk.StringVar(value="Lebar: -- mm")
        self._height_var = tk.StringVar(value="Tinggi: -- mm")
        self._class_var = tk.StringVar(value="Class ID: --")
        self._status_var = tk.StringVar(value="Status: Menunggu data")
        self._timestamp_var = tk.StringVar(value="Update: --")

        content = ttk.Frame(root, padding=12)
        content.pack(fill=tk.BOTH, expand=True)
        ttk.Label(content, textvariable=self._category_var).pack(anchor=tk.W)
        ttk.Label(content, textvariable=self._tariff_var).pack(anchor=tk.W)
        ttk.Label(content, textvariable=self._currency_var).pack(anchor=tk.W)
        ttk.Label(content, textvariable=self._class_var).pack(anchor=tk.W, pady=(0, 8))
        ttk.Label(content, textvariable=self._length_var).pack(anchor=tk.W)
        ttk.Label(content, textvariable=self._width_var).pack(anchor=tk.W)
        ttk.Label(content, textvariable=self._height_var).pack(anchor=tk.W)
        ttk.Label(content, textvariable=self._status_var).pack(anchor=tk.W, pady=(8, 0))
        ttk.Label(content, textvariable=self._timestamp_var).pack(anchor=tk.W)

        def poll_queue() -> None:
            while True:
                try:
                    kind, payload = self._update_queue.get_nowait()
                except Empty:
                    break
                if kind == "tariff":
                    self._category_var.set(f"Kategori: {payload.category}")
                    self._tariff_var.set(
                        f"Tarif: {payload.tariff:,.0f} {payload.currency}"
                    )
                    self._class_var.set(
                        f"Class ID: {payload.dimension.class_id}"
                    )
                    dim = payload.dimension
                    self._length_var.set(f"Panjang: {dim.length:.1f} mm")
                    self._width_var.set(f"Lebar: {dim.width:.1f} mm")
                    self._height_var.set(f"Tinggi: {dim.height:.1f} mm")
                    self._timestamp_var.set(
                        "Update: "
                        f"{payload.header.stamp.sec}.{payload.header.stamp.nanosec:09d}"
                    )
                elif kind == "status":
                    self._status_var.set(payload)
            if not self._shutdown_event.is_set():
                root.after(200, poll_queue)
            else:
                root.destroy()

        root.after(200, poll_queue)
        root.mainloop()

    def _dimension_callback(self, msg: VehicleDimension) -> None:
        """Hitung kategori dan tarif dari data dimensi kendaraan."""
        category_by_length, tariff_by_length = self._classify_by_length(msg.length)
        override = self._class_tariffs.get(msg.class_id)
        if override:
            category = override["category"]
            tariff = override["tariff"]
            if tariff_by_length > tariff:
                category = category_by_length
                tariff = tariff_by_length
        else:
            category = category_by_length
            tariff = tariff_by_length

        tariff_msg = VehicleTariff()
        tariff_msg.header = msg.header
        tariff_msg.category = category
        tariff_msg.tariff = float(tariff)
        tariff_msg.currency = self._currency
        tariff_msg.dimension = msg

        self._tariff_pub.publish(tariff_msg)
        self._latest_tariff = tariff_msg
        self._last_update_time = self.get_clock().now()

        if self._enable_gui:
            self._update_queue.put(("tariff", tariff_msg))
        else:
            self.get_logger().info(
                "Kategori %s (class_id=%d) - Tarif %.0f %s | L=%.1f mm, W=%.1f mm, H=%.1f mm",
                category,
                msg.class_id,
                tariff,
                self._currency,
                msg.length,
                msg.width,
                msg.height,
            )

    def _classify_by_length(self, length_mm: float) -> Tuple[str, float]:
        """Pilih rule berdasarkan panjang kendaraan (mm)."""
        for rule in self._dimension_rules:
            max_length = rule["max_length_mm"]
            if max_length < 0.0 or length_mm <= max_length:
                return rule["name"], rule["tariff"]
        return ("unknown", 0.0)

    def _status_timer_callback(self) -> None:
        """Perbarui status kelayakan data untuk GUI."""
        now = self.get_clock().now()
        age_ok = (
            self._last_update_time
            and (now - self._last_update_time).nanoseconds * 1e-9
            < self._stale_timeout
        )
        status_text = (
            "Status: Data terbaru" if age_ok else "Status: Data kedaluwarsa"
        )
        if self._enable_gui:
            self._update_queue.put(("status", status_text))
        else:
            if not age_ok:
                self.get_logger().debug(status_text)

    def destroy_node(self) -> bool:
        self._shutdown_event.set()
        return super().destroy_node()


def main(args=None) -> None:
    rclpy.init(args=args)
    node = TariffCalculatorNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
