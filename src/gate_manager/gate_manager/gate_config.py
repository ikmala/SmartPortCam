"""Validasi, loading, dan saving konfigurasi gate dari gates.json."""

import json
import re
from dataclasses import dataclass, field, asdict
from pathlib import Path
from typing import List, Optional

# ── Pola keamanan ─────────────────────────────────────────────────────────
# gate_id: huruf kecil, angka, underscore; diawali huruf; max 32 karakter
_GATE_ID_RE   = re.compile(r'^[a-z][a-z0-9_]{0,31}$')
# camera_serial: alfanumerik saja; 6-30 karakter
_SERIAL_RE    = re.compile(r'^[A-Za-z0-9]{6,30}$')
# gate_name: boleh spasi, huruf, angka, tanda baca umum; max 64 karakter
_GATE_NAME_RE = re.compile(r'^[\w\s\-\.,:()]{1,64}$')


class GateConfigError(ValueError):
    """Kesalahan validasi konfigurasi gate."""


def validate_gate_id(gate_id: str) -> str:
    """Return gate_id yang sudah divalidasi, atau raise GateConfigError."""
    if not isinstance(gate_id, str) or not _GATE_ID_RE.match(gate_id):
        raise GateConfigError(
            f"gate_id tidak valid: '{gate_id}'. "
            "Harus: huruf kecil/angka/underscore, diawali huruf, max 32 karakter."
        )
    return gate_id


def validate_serial(serial: str) -> str:
    """Validasi serial number kamera RealSense."""
    if not isinstance(serial, str) or not _SERIAL_RE.match(serial):
        raise GateConfigError(
            f"camera_serial tidak valid: '{serial}'. "
            "Harus: alfanumerik, 6-30 karakter."
        )
    return serial


def validate_gate_name(name: str) -> str:
    """Validasi nama gate untuk display."""
    if not isinstance(name, str) or not _GATE_NAME_RE.match(name):
        raise GateConfigError(
            f"gate_name tidak valid: '{name}'. Max 64 karakter, "
            "boleh huruf/angka/spasi/tanda baca umum."
        )
    return name


@dataclass
class GateParams:
    confidence_threshold: float = 0.70
    min_valid_points: int = 200
    max_detections: int = 1

    def validate(self) -> None:
        if not 0.0 < self.confidence_threshold <= 1.0:
            raise GateConfigError("confidence_threshold harus antara 0.0 dan 1.0.")
        if self.min_valid_points < 10:
            raise GateConfigError("min_valid_points harus minimal 10.")
        if self.max_detections < 0:
            raise GateConfigError("max_detections tidak boleh negatif.")


@dataclass
class GateConfig:
    id:            str
    name:          str
    location:      str = ""
    camera_serial: str = ""
    active:        bool = True
    params:        GateParams = field(default_factory=GateParams)

    def validate(self) -> None:
        validate_gate_id(self.id)
        validate_gate_name(self.name)
        if self.camera_serial:
            validate_serial(self.camera_serial)
        if len(self.location) > 128:
            raise GateConfigError("location max 128 karakter.")
        self.params.validate()

    # ── Konversi ke/dari dict ────────────────────────────────────────
    def to_dict(self) -> dict:
        return {
            "id":            self.id,
            "name":          self.name,
            "location":      self.location,
            "camera_serial": self.camera_serial,
            "active":        self.active,
            "params": {
                "confidence_threshold": self.params.confidence_threshold,
                "min_valid_points":     self.params.min_valid_points,
                "max_detections":       self.params.max_detections,
            },
        }

    @classmethod
    def from_dict(cls, data: dict) -> "GateConfig":
        p = data.get("params", {})
        cfg = cls(
            id=str(data.get("id", "")),
            name=str(data.get("name", "")),
            location=str(data.get("location", "")),
            camera_serial=str(data.get("camera_serial", "")),
            active=bool(data.get("active", True)),
            params=GateParams(
                confidence_threshold=float(p.get("confidence_threshold", 0.70)),
                min_valid_points=int(p.get("min_valid_points", 200)),
                max_detections=int(p.get("max_detections", 1)),
            ),
        )
        cfg.validate()
        return cfg


# ── File I/O ───────────────────────────────────────────────────────────────
_SCHEMA_VERSION = "1.0"


def load_gates(path: Path) -> List[GateConfig]:
    """Baca gates.json dan kembalikan list GateConfig yang sudah tervalidasi."""
    if not path.exists():
        return []
    try:
        raw = json.loads(path.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, OSError) as exc:
        raise GateConfigError(f"Gagal membaca {path}: {exc}") from exc

    gates: List[GateConfig] = []
    for entry in raw.get("gates", []):
        try:
            gates.append(GateConfig.from_dict(entry))
        except GateConfigError as exc:
            raise GateConfigError(f"Gate tidak valid di {path}: {exc}") from exc
    return gates


def save_gates(path: Path, gates: List[GateConfig]) -> None:
    """Tulis gates ke gates.json secara atomic (write-then-rename)."""
    data = {
        "version": _SCHEMA_VERSION,
        "gates": [g.to_dict() for g in gates],
    }
    tmp = path.with_suffix(".tmp")
    try:
        tmp.write_text(json.dumps(data, indent=2, ensure_ascii=False), encoding="utf-8")
        tmp.replace(path)
    except OSError as exc:
        raise GateConfigError(f"Gagal menyimpan {path}: {exc}") from exc
