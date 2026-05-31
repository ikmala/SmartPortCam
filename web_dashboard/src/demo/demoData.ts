/**
 * Data dummy dan fungsi simulasi untuk Demo Mode.
 * Dipakai ketika VITE_DEMO_MODE=true (mode demo, tanpa koneksi ROS).
 */

/* Local type declarations — sama dengan App.tsx agar tidak circular */
interface GateStatusMsg {
  gate_id: string; gate_name: string; location: string; camera_serial: string
  active: boolean; camera_online: boolean
  proc_detector: boolean; proc_estimator: boolean; proc_tariff: boolean; proc_logger: boolean
  total_detected: number; last_detection_iso: string; error_message: string
}
interface CameraDeviceMsg {
  serial_number: string; model_name: string; usb_type: string
  in_use: boolean; assigned_gate_id: string
}
interface HistoryEntry {
  time: Date; class_id: number; category: string
  length_mm: number; width_mm: number; height_mm: number
  tariff: number; currency: string
}

/* ── Gate dummy ────────────────────────────────────────────────────── */
export const DEMO_GATES: GateStatusMsg[] = [
  {
    gate_id: 'gate_masuk', gate_name: 'Gate Masuk', location: 'Pintu Masuk Utama',
    camera_serial: 'DEMO12345678', active: true,
    camera_online: true, proc_detector: true, proc_estimator: true,
    proc_tariff: true, proc_logger: true,
    total_detected: 47, last_detection_iso: new Date().toISOString(), error_message: '',
  },
  {
    gate_id: 'gate_keluar', gate_name: 'Gate Keluar', location: 'Pintu Keluar',
    camera_serial: 'DEMO98765432', active: true,
    camera_online: true, proc_detector: true, proc_estimator: true,
    proc_tariff: true, proc_logger: true,
    total_detected: 23, last_detection_iso: new Date().toISOString(), error_message: '',
  },
]

/* ── Kamera dummy ──────────────────────────────────────────────────── */
export const DEMO_CAMERAS: CameraDeviceMsg[] = [
  { serial_number: 'DEMO12345678', model_name: 'Intel RealSense D455', usb_type: 'USB 3.2 Gen 1', in_use: true,  assigned_gate_id: 'gate_masuk'  },
  { serial_number: 'DEMO98765432', model_name: 'Intel RealSense D435i',usb_type: 'USB 3.2 Gen 1', in_use: true,  assigned_gate_id: 'gate_keluar' },
  { serial_number: 'DEMO55566677', model_name: 'Intel RealSense D435', usb_type: 'USB 3.0',       in_use: false, assigned_gate_id: ''            },
]

/* ── Skenario kendaraan (realistik untuk pelabuhan) ────────────────── */
export interface VehicleScenario {
  class_id: number
  category: string
  label: string
  length: [number, number]   // mm range
  width:  [number, number]
  height: [number, number]
  tariff: number
  weight: number             // probabilitas relatif
}

export const VEHICLE_SCENARIOS: VehicleScenario[] = [
  { class_id: 7, category: 'heavy_truck',   label: 'Truk',          length: [8000, 14000], width: [2200, 2600], height: [3000, 4200], tariff: 150_000, weight: 45 },
  { class_id: 5, category: 'bus',           label: 'Bus',           length: [9000, 12500], width: [2400, 2600], height: [3200, 3800], tariff: 120_000, weight: 20 },
  { class_id: 2, category: 'passenger_car', label: 'Mobil',         length: [3800, 5200],  width: [1600, 1950], height: [1400, 1750], tariff: 50_000,  weight: 25 },
  { class_id: 3, category: 'motorcycle',    label: 'Sepeda Motor',  length: [1800, 2300],  width: [600,  850],  height: [1000, 1300], tariff: 20_000,  weight: 10 },
]

/* ── Fungsi helper ─────────────────────────────────────────────────── */
function rand(min: number, max: number) {
  return Math.floor(Math.random() * (max - min + 1)) + min
}

/** Pilih skenario kendaraan berdasarkan bobot probabilitas. */
export function pickVehicle(): VehicleScenario {
  const total = VEHICLE_SCENARIOS.reduce((s, v) => s + v.weight, 0)
  let r = Math.random() * total
  for (const v of VEHICLE_SCENARIOS) {
    r -= v.weight
    if (r <= 0) return v
  }
  return VEHICLE_SCENARIOS[0]
}

/** Buat satu entri deteksi dummy. */
export function makeDemoDetection(gateId: string, overrideVehicle?: VehicleScenario) {
  const v = overrideVehicle ?? pickVehicle()
  const ts = new Date()
  const length_mm = rand(v.length[0], v.length[1])
  const width_mm  = rand(v.width[0],  v.width[1])
  const height_mm = rand(v.height[0], v.height[1])

  const dimension = { length: length_mm, width: width_mm, height: height_mm, class_id: v.class_id, updatedAt: ts }
  const tariff    = { category: v.category, tariff: v.tariff, currency: 'IDR', dimension, updatedAt: ts }

  const historyEntry: HistoryEntry & { gate_id: string } = {
    time: ts, gate_id: gateId,
    class_id: v.class_id, category: v.category,
    length_mm, width_mm, height_mm,
    tariff: v.tariff, currency: 'IDR',
  }

  return { dimension, tariff, historyEntry, vehicle: v }
}

/** Generate riwayat awal agar dashboard langsung terlihat berisi data. */
export function generateInitialHistory(): Array<HistoryEntry & { gate_id: string }> {
  const gates = ['gate_masuk', 'gate_keluar']
  const now   = Date.now()
  return Array.from({ length: 20 }, (_, i) => {
    const v       = VEHICLE_SCENARIOS[i % VEHICLE_SCENARIOS.length]
    const gateId  = gates[i % 2]
    const time    = new Date(now - i * 5 * 60_000) // setiap 5 menit ke belakang
    const length_mm = rand(v.length[0], v.length[1])
    const width_mm  = rand(v.width[0],  v.width[1])
    const height_mm = rand(v.height[0], v.height[1])
    return {
      time, gate_id: gateId,
      class_id: v.class_id, category: v.category,
      length_mm, width_mm, height_mm,
      tariff: v.tariff, currency: 'IDR',
    }
  })
}
