export interface RosStamp { sec?: number; nanosec?: number }

export interface VehicleDimensionMsg {
  length?: number; width?: number; height?: number
  class_id?: number; header?: { stamp?: RosStamp }; updatedAt?: Date
}

export interface VehicleTariffMsg {
  category?: string; tariff?: number; currency?: string
  dimension?: VehicleDimensionMsg
  header?: { stamp?: RosStamp }; updatedAt?: Date
}

export interface LogEntry { time: string; text: string; type: 'info' | 'data' | 'error' }

export interface HistoryEntry {
  time: Date; class_id: number; category: string
  length_mm: number; width_mm: number; height_mm: number
  tariff: number; currency: string
}

export interface GateStatusMsg {
  gate_id: string; gate_name: string; location: string
  camera_serial: string; active: boolean
  camera_online: boolean
  proc_detector: boolean; proc_estimator: boolean
  proc_tariff: boolean; proc_logger: boolean
  total_detected: number; last_detection_iso: string
  error_message: string
}

export interface CameraDeviceMsg {
  serial_number: string; model_name: string
  usb_type: string; in_use: boolean; assigned_gate_id: string
}

export interface AddGateForm {
  gate_name: string; gate_id: string; location: string
  camera_serial: string
  confidence_threshold: number; min_valid_points: number; max_detections: number
  start_immediately: boolean; showAdvanced: boolean
}

export type ConnectionStatus = 'disconnected' | 'connecting' | 'connected' | 'error'
export type DimensionUnit    = 'mm' | 'cm' | 'm'
export type Page             = 'camera' | 'dashboard' | 'settings' | 'log'
