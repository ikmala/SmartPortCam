"""Node ROS 2 utama untuk manajemen siklus hidup semua gate."""

import datetime
import threading
from pathlib import Path
from typing import Dict, Optional

import rclpy
from rclpy.node import Node
from std_msgs.msg import Header

from vehicle_dimension_msgs.msg import GateStatus
from vehicle_dimension_msgs.srv import (
    AddGate,
    ListGates,
    RemoveGate,
    SetGateActive,
)

from .gate_config import (
    GateConfig,
    GateConfigError,
    GateParams,
    load_gates,
    save_gates,
    validate_gate_id,
    validate_serial,
    validate_gate_name,
)
from .gate_process import GateProcess

_WORKSPACE    = Path(__file__).resolve().parents[4]
_SETUP_SCRIPT = _WORKSPACE / "install" / "setup.bash"
_GATES_FILE   = _WORKSPACE / "config" / "gates.json"

_HEALTH_INTERVAL  = 10.0   # detik: health check & auto-restart
_STATUS_INTERVAL  =  5.0   # detik: publish status


class GateManagerNode(Node):
    """
    Mengelola semua gate secara terpusat.

    Services:
      /smartport/gates/add         — tambah gate baru
      /smartport/gates/remove      — hapus gate
      /smartport/gates/set_active  — aktifkan / nonaktifkan gate
      /smartport/gates/list        — daftar status semua gate

    Publishers:
      /smartport/gates/status (GateStatus, satu per gate)
    """

    def __init__(self) -> None:
        super().__init__("gate_manager")
        self._lock    = threading.Lock()
        self._configs: Dict[str, GateConfig]  = {}
        self._procs:   Dict[str, GateProcess] = {}
        self._counts:  Dict[str, int]         = {}  # gate_id -> total kendaraan sesi ini

        # Load konfigurasi
        self._load_config()

        # Publisher status per gate
        self._status_pub = self.create_publisher(GateStatus, "/smartport/gates/status", 10)

        # Services
        self.create_service(AddGate,      "/smartport/gates/add",        self._svc_add_gate)
        self.create_service(RemoveGate,   "/smartport/gates/remove",     self._svc_remove_gate)
        self.create_service(SetGateActive,"/smartport/gates/set_active", self._svc_set_active)
        self.create_service(ListGates,    "/smartport/gates/list",       self._svc_list_gates)

        # Timers
        self.create_timer(_HEALTH_INTERVAL,  self._health_check)
        self.create_timer(_STATUS_INTERVAL,  self._publish_all_status)

        # Mulai semua gate yang aktif
        self._start_active_gates()
        self.get_logger().info(
            f"Gate manager siap — {len(self._configs)} gate dimuat dari {_GATES_FILE}."
        )

    # ══════════════════════════════════════════════════════════════════
    # Load / Save config
    # ══════════════════════════════════════════════════════════════════
    def _load_config(self) -> None:
        try:
            gates = load_gates(_GATES_FILE)
        except GateConfigError as exc:
            self.get_logger().error(f"Gagal memuat gates.json: {exc}")
            gates = []
        with self._lock:
            for g in gates:
                self._configs[g.id] = g

    def _persist_config(self) -> None:
        """Simpan konfigurasi saat ini ke gates.json."""
        with self._lock:
            gates = list(self._configs.values())
        try:
            save_gates(_GATES_FILE, gates)
        except GateConfigError as exc:
            self.get_logger().error(f"Gagal menyimpan gates.json: {exc}")

    # ══════════════════════════════════════════════════════════════════
    # Lifecycle
    # ══════════════════════════════════════════════════════════════════
    def _start_active_gates(self) -> None:
        with self._lock:
            active = [g for g in self._configs.values() if g.active]
        for cfg in active:
            self._start_gate(cfg)

    def _start_gate(self, cfg: GateConfig) -> bool:
        if not _SETUP_SCRIPT.exists():
            self.get_logger().error(
                "Setup script tidak ditemukan. Jalankan 'colcon build' terlebih dahulu."
            )
            return False
        proc = GateProcess(cfg, _WORKSPACE, _SETUP_SCRIPT)
        ok   = proc.start()
        if ok:
            with self._lock:
                self._procs[cfg.id]  = proc
                self._counts[cfg.id] = 0
            self.get_logger().info(f"Gate '{cfg.id}' ({cfg.name}) berhasil distart.")
        else:
            self.get_logger().error(f"Gagal start gate '{cfg.id}'.")
        return ok

    def _stop_gate(self, gate_id: str) -> None:
        with self._lock:
            proc = self._procs.pop(gate_id, None)
        if proc:
            proc.stop()
            self.get_logger().info(f"Gate '{gate_id}' dihentikan.")

    # ══════════════════════════════════════════════════════════════════
    # Health check & auto-restart
    # ══════════════════════════════════════════════════════════════════
    def _health_check(self) -> None:
        with self._lock:
            procs_snapshot = dict(self._procs)
            cfgs_snapshot  = dict(self._configs)
        for gate_id, proc in procs_snapshot.items():
            cfg = cfgs_snapshot.get(gate_id)
            if cfg is None or not cfg.active:
                continue
            h = proc.health()
            dead = [name for name, alive in h.items() if not alive]
            if dead:
                self.get_logger().warning(
                    f"Gate '{gate_id}': proses mati [{', '.join(dead)}] — restart…"
                )
                proc.restart_dead()

    # ══════════════════════════════════════════════════════════════════
    # Status publisher
    # ══════════════════════════════════════════════════════════════════
    def _publish_all_status(self) -> None:
        with self._lock:
            configs = dict(self._configs)
            procs   = dict(self._procs)
            counts  = dict(self._counts)

        for gate_id, cfg in configs.items():
            proc  = procs.get(gate_id)
            h     = proc.health() if proc else {}
            msg   = GateStatus()
            msg.header             = Header()
            msg.header.stamp       = self.get_clock().now().to_msg()
            msg.gate_id            = cfg.id
            msg.gate_name          = cfg.name
            msg.location           = cfg.location
            msg.camera_serial      = cfg.camera_serial
            msg.active             = cfg.active
            msg.camera_online      = h.get("realsense", False)
            msg.proc_detector      = h.get("detector",  False)
            msg.proc_estimator     = h.get("estimator", False)
            msg.proc_tariff        = h.get("tariff",    False)
            msg.proc_logger        = h.get("data_logger", False)
            msg.total_detected     = counts.get(gate_id, 0)
            msg.last_detection_iso = datetime.datetime.now(
                tz=datetime.timezone.utc
            ).isoformat()
            self._status_pub.publish(msg)

    # ══════════════════════════════════════════════════════════════════
    # ROS 2 Services
    # ══════════════════════════════════════════════════════════════════
    def _svc_add_gate(
        self, req: AddGate.Request, res: AddGate.Response
    ) -> AddGate.Response:
        # Validasi input (security)
        try:
            validate_gate_id(req.gate_id)
            validate_gate_name(req.gate_name)
            if req.camera_serial:
                validate_serial(req.camera_serial)
        except GateConfigError as exc:
            res.success = False
            res.message = str(exc)
            return res

        with self._lock:
            if req.gate_id in self._configs:
                res.success = False
                res.message = f"Gate '{req.gate_id}' sudah ada."
                return res

        cfg = GateConfig(
            id=req.gate_id,
            name=req.gate_name,
            location="",
            camera_serial=req.camera_serial,
            active=req.start_immediately,
            params=GateParams(
                confidence_threshold=req.confidence_threshold or 0.70,
                min_valid_points=req.min_valid_points or 200,
                max_detections=req.max_detections or 1,
            ),
        )
        with self._lock:
            self._configs[cfg.id] = cfg

        self._persist_config()

        if req.start_immediately:
            ok = self._start_gate(cfg)
            if not ok:
                res.success = False
                res.message = f"Gate disimpan tapi gagal start. Cek serial kamera."
                return res

        res.success = True
        res.message = f"Gate '{cfg.id}' berhasil ditambahkan."
        self.get_logger().info(res.message)
        return res

    def _svc_remove_gate(
        self, req: RemoveGate.Request, res: RemoveGate.Response
    ) -> RemoveGate.Response:
        try:
            validate_gate_id(req.gate_id)
        except GateConfigError as exc:
            res.success = False
            res.message = str(exc)
            return res

        with self._lock:
            if req.gate_id not in self._configs:
                res.success = False
                res.message = f"Gate '{req.gate_id}' tidak ditemukan."
                return res
            proc = self._procs.get(req.gate_id)
            is_running = proc and proc.is_any_alive()

        if is_running and not req.force:
            res.success = False
            res.message = (
                f"Gate '{req.gate_id}' masih berjalan. "
                "Gunakan force=true untuk memaksa hapus."
            )
            return res

        self._stop_gate(req.gate_id)
        with self._lock:
            self._configs.pop(req.gate_id, None)
            self._counts.pop(req.gate_id, None)

        self._persist_config()
        res.success = True
        res.message = f"Gate '{req.gate_id}' berhasil dihapus."
        self.get_logger().info(res.message)
        return res

    def _svc_set_active(
        self, req: SetGateActive.Request, res: SetGateActive.Response
    ) -> SetGateActive.Response:
        try:
            validate_gate_id(req.gate_id)
        except GateConfigError as exc:
            res.success = False
            res.message = str(exc)
            return res

        with self._lock:
            cfg = self._configs.get(req.gate_id)
        if cfg is None:
            res.success = False
            res.message = f"Gate '{req.gate_id}' tidak ditemukan."
            return res

        cfg.active = req.active
        self._persist_config()

        if req.active:
            ok = self._start_gate(cfg)
            if not ok:
                res.success = False
                res.message = f"Gagal start gate '{req.gate_id}'."
                return res
        else:
            self._stop_gate(req.gate_id)

        res.success = True
        res.message = f"Gate '{req.gate_id}' {'diaktifkan' if req.active else 'dinonaktifkan'}."
        self.get_logger().info(res.message)
        return res

    def _svc_list_gates(
        self, _req: ListGates.Request, res: ListGates.Response
    ) -> ListGates.Response:
        with self._lock:
            configs = dict(self._configs)
            procs   = dict(self._procs)
            counts  = dict(self._counts)

        statuses = []
        for gate_id, cfg in configs.items():
            proc = procs.get(gate_id)
            h    = proc.health() if proc else {}
            s    = GateStatus()
            s.gate_id        = cfg.id
            s.gate_name      = cfg.name
            s.location       = cfg.location
            s.camera_serial  = cfg.camera_serial
            s.active         = cfg.active
            s.camera_online  = h.get("realsense",   False)
            s.proc_detector  = h.get("detector",    False)
            s.proc_estimator = h.get("estimator",   False)
            s.proc_tariff    = h.get("tariff",      False)
            s.proc_logger    = h.get("data_logger", False)
            s.total_detected = counts.get(gate_id, 0)
            statuses.append(s)

        res.gates = statuses
        return res

    def destroy_node(self) -> bool:
        with self._lock:
            gate_ids = list(self._procs.keys())
        for gid in gate_ids:
            self._stop_gate(gid)
        return super().destroy_node()


def main(args=None) -> None:
    rclpy.init(args=args)
    node = GateManagerNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
