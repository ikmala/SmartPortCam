import { useCallback, useEffect, useMemo, useRef, useState } from 'react'
import * as ROSLIB from 'roslib'
import './App.css'
import { DEMO_CAMERAS, DEMO_GATES, generateInitialHistory, makeDemoDetection } from './demo/demoData'

/* ═══════════════════════════════════════════════════════════
   Demo / Production flag
   ═══════════════════════════════════════════════════════════ */
const DEMO_MODE = import.meta.env.VITE_DEMO_MODE === 'true'

/* ═══════════════════════════════════════════════════════════
   Types
   ═══════════════════════════════════════════════════════════ */
type ConnectionStatus = 'disconnected' | 'connecting' | 'connected' | 'error'
type DimensionUnit    = 'mm' | 'cm' | 'm'
type Page             = 'camera' | 'dashboard' | 'settings' | 'log'

interface RosStamp { sec?: number; nanosec?: number }

interface VehicleDimensionMsg {
  length?: number; width?: number; height?: number
  class_id?: number; header?: { stamp?: RosStamp }; updatedAt?: Date
}
interface VehicleTariffMsg {
  category?: string; tariff?: number; currency?: string
  dimension?: VehicleDimensionMsg
  header?: { stamp?: RosStamp }; updatedAt?: Date
}
interface LogEntry { time: string; text: string; type: 'info' | 'data' | 'error' }
interface HistoryEntry {
  time: Date; class_id: number; category: string
  length_mm: number; width_mm: number; height_mm: number
  tariff: number; currency: string
}
interface GateStatusMsg {
  gate_id: string; gate_name: string; location: string
  camera_serial: string; active: boolean
  camera_online: boolean
  proc_detector: boolean; proc_estimator: boolean
  proc_tariff: boolean; proc_logger: boolean
  total_detected: number; last_detection_iso: string
  error_message: string
}
interface CameraDeviceMsg {
  serial_number: string; model_name: string
  usb_type: string; in_use: boolean; assigned_gate_id: string
}
interface AddGateForm {
  gate_name: string; gate_id: string; location: string
  camera_serial: string
  confidence_threshold: number; min_valid_points: number; max_detections: number
  start_immediately: boolean; showAdvanced: boolean
}

/* ═══════════════════════════════════════════════════════════
   Constants
   ═══════════════════════════════════════════════════════════ */
const DEFAULT_WS_URL =
  (import.meta.env.VITE_ROSBRIDGE_URL as string) ?? 'ws://localhost:9090'
const DEFAULT_VIDEO_BASE =
  (import.meta.env.VITE_VIDEO_BASE as string) ?? 'http://localhost:8080'

const CLASS_LABELS: Record<number, string> = {
  2: 'Mobil', 3: 'Sepeda Motor', 5: 'Bus', 7: 'Truk',
}
const MAX_TARIFF = 150_000

const cls = (id?: number) =>
  id === undefined || id === null ? '--' : (CLASS_LABELS[id] ?? `Class ${id}`)

function rosTimeToDate(s?: RosStamp) {
  if (s?.sec === undefined) return null
  return new Date((s.sec ?? 0) * 1000 + Math.floor((s.nanosec ?? 0) / 1e6))
}
function fmtTime(d?: Date | null) {
  if (!d) return '--'
  return d.toLocaleTimeString('id-ID', { hour: '2-digit', minute: '2-digit', second: '2-digit' })
}
function fmtDateTime(d?: Date | null) {
  if (!d) return '--'
  return d.toLocaleString('id-ID', { dateStyle: 'short', timeStyle: 'medium' })
}
function buildVideoUrl(base: string, gateId: string) {
  const b = base.replace(/\/$/, '')
  return `${b}/stream?topic=/${gateId}/vehicle/detections/image_raw&type=mjpeg`
}
function toGateId(name: string): string {
  const slug = name.toLowerCase()
    .normalize('NFD').replace(/[̀-ͯ]/g, '')
    .replace(/[^a-z0-9\s]/g, '').trim()
    .replace(/\s+/g, '_').substring(0, 32)
  return /^[a-z]/.test(slug) ? slug : 'gate_' + slug || 'gate_baru'
}
const GATE_ID_RE   = /^[a-z][a-z0-9_]{0,31}$/
const SERIAL_RE    = /^[A-Za-z0-9]{6,30}$/

/* ═══════════════════════════════════════════════════════════
   Icons
   ═══════════════════════════════════════════════════════════ */
const I = {
  logo:     'SPC',
  camera:   <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><path d="M14.5 4h-5L7 7H4a2 2 0 0 0-2 2v9a2 2 0 0 0 2 2h16a2 2 0 0 0 2-2V9a2 2 0 0 0-2-2h-3z"/><circle cx="12" cy="13" r="3"/></svg>,
  chart:    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><line x1="18" y1="20" x2="18" y2="10"/><line x1="12" y1="20" x2="12" y2="4"/><line x1="6" y1="20" x2="6" y2="14"/></svg>,
  gear:     <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-4 0v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83-2.83l.06-.06A1.65 1.65 0 0 0 4.68 15a1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1 0-4h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 2.83-2.83l.06.06A1.65 1.65 0 0 0 9 4.68a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 4 0v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 2.83l-.06.06A1.65 1.65 0 0 0 19.4 9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg>,
  log:      <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><line x1="8" y1="6" x2="21" y2="6"/><line x1="8" y1="12" x2="21" y2="12"/><line x1="8" y1="18" x2="21" y2="18"/><line x1="3" y1="6" x2="3.01" y2="6"/><line x1="3" y1="12" x2="3.01" y2="12"/><line x1="3" y1="18" x2="3.01" y2="18"/></svg>,
  more:     <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2.5" strokeLinecap="round"><circle cx="12" cy="5" r="1" fill="currentColor"/><circle cx="12" cy="12" r="1" fill="currentColor"/><circle cx="12" cy="19" r="1" fill="currentColor"/></svg>,
  plus:     <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2.5" strokeLinecap="round"><line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/></svg>,
  pencil:   <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"/><path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4z"/></svg>,
  trash:    <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><polyline points="3 6 5 6 21 6"/><path d="M19 6l-1 14H6L5 6"/><path d="M10 11v6"/><path d="M14 11v6"/><path d="M9 6V4h6v2"/></svg>,
  sun:      <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round"><circle cx="12" cy="12" r="5"/><line x1="12" y1="1" x2="12" y2="3"/><line x1="12" y1="21" x2="12" y2="23"/><line x1="4.22" y1="4.22" x2="5.64" y2="5.64"/><line x1="18.36" y1="18.36" x2="19.78" y2="19.78"/><line x1="1" y1="12" x2="3" y2="12"/><line x1="21" y1="12" x2="23" y2="12"/></svg>,
  link:     <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><path d="M10 13a5 5 0 0 0 7.54.54l3-3a5 5 0 0 0-7.07-7.07l-1.72 1.71"/><path d="M14 11a5 5 0 0 0-7.54-.54l-3 3a5 5 0 0 0 7.07 7.07l1.71-1.71"/></svg>,
  ruler:    <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><path d="M21.3 8.7 8.7 21.3c-1 1-2.5 1-3.4 0l-2.6-2.6c-1-1-1-2.5 0-3.4L15.3 2.7c1-1 2.5-1 3.4 0l2.6 2.6c1 1 1 2.5 0 3.4Z"/><path d="m7.5 10.5 2 2"/><path d="m10.5 7.5 2 2"/><path d="m13.5 4.5 2 2"/><path d="m4.5 13.5 2 2"/></svg>,
  truck:    <svg width="52" height="52" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="1.4" strokeLinecap="round" strokeLinejoin="round"><rect x="1" y="3" width="15" height="13"/><polygon points="16 8 20 8 23 11 23 16 16 16 16 8"/><circle cx="5.5" cy="18.5" r="2.5"/><circle cx="18.5" cy="18.5" r="2.5"/></svg>,
  camOff:   <svg width="44" height="44" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="1.2" strokeLinecap="round" strokeLinejoin="round"><path d="M14.5 4h-5L7 7H4a2 2 0 0 0-2 2v9a2 2 0 0 0 2 2h16a2 2 0 0 0 2-2V9a2 2 0 0 0-2-2h-3z"/><line x1="1" y1="1" x2="23" y2="23"/></svg>,
  wifi:     <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><path d="M5 12.55a11 11 0 0 1 14.08 0"/><path d="M1.42 9a16 16 0 0 1 21.16 0"/><path d="M8.53 16.11a6 6 0 0 1 6.95 0"/><line x1="12" y1="20" x2="12.01" y2="20"/></svg>,
  video:    <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><polygon points="23 7 16 12 23 17 23 7"/><rect x="1" y="5" width="15" height="14" rx="2"/></svg>,
  refresh:  <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2.2" strokeLinecap="round" strokeLinejoin="round"><polyline points="23 4 23 10 17 10"/><path d="M20.49 15a9 9 0 1 1-2.12-9.36L23 10"/></svg>,
  chevDown: <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2.5" strokeLinecap="round" strokeLinejoin="round"><polyline points="6 9 12 15 18 9"/></svg>,
  clock:    <svg width="11" height="11" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round"><circle cx="12" cy="12" r="10"/><polyline points="12 6 12 12 16 14"/></svg>,
  check:    <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2.5" strokeLinecap="round" strokeLinejoin="round"><polyline points="20 6 9 17 4 12"/></svg>,
}

/* ═══════════════════════════════════════════════════════════
   Small reusable components
   ═══════════════════════════════════════════════════════════ */
function Toggle({ on, onToggle }: { on: boolean; onToggle?: () => void }) {
  return <button role="switch" aria-checked={on} className={`sp-toggle${on ? ' on' : ''}`} onClick={onToggle} />
}

function TariffGauge({ value, max = MAX_TARIFF }: { value: number; max?: number }) {
  const cx = 70, cy = 70, r = 52
  const toXY = (deg: number) => ({ x: cx + r * Math.cos(deg * Math.PI / 180), y: cy + r * Math.sin(deg * Math.PI / 180) })
  const start = toXY(150), end = toXY(390)
  const pct = Math.min(1, Math.max(0, value / max))
  const valPt = toXY(150 + pct * 240)
  const fmt = (v: number) => v >= 1_000_000 ? `${(v/1_000_000).toFixed(1)}jt` : v >= 1_000 ? `${(v/1_000).toFixed(0)}rb` : v.toFixed(0)
  return (
    <svg viewBox="12 12 116 96" width="148" height="112">
      <path d={`M${start.x.toFixed(1)} ${start.y.toFixed(1)} A${r} ${r} 0 1 1 ${end.x.toFixed(1)} ${end.y.toFixed(1)}`} fill="none" stroke="var(--border)" strokeWidth="9" strokeLinecap="round"/>
      {pct > 0 && <path d={`M${start.x.toFixed(1)} ${start.y.toFixed(1)} A${r} ${r} 0 ${pct*240>180?1:0} 1 ${valPt.x.toFixed(1)} ${valPt.y.toFixed(1)}`} fill="none" stroke="var(--primary)" strokeWidth="9" strokeLinecap="round"/>}
      <text x={cx} y={cy+4} textAnchor="middle" fontSize="17" fontWeight="700" fill="var(--text)">{value > 0 ? fmt(value) : '--'}</text>
      <text x={cx} y={cx+18} textAnchor="middle" fontSize="9" fill="var(--muted)">IDR</text>
      <text x={start.x-2} y={start.y+14} textAnchor="middle" fontSize="8" fill="var(--muted)">0</text>
      <text x={end.x+2}   y={end.y+14}   textAnchor="middle" fontSize="8" fill="var(--muted)">150k</text>
    </svg>
  )
}

/* Node health indicator dots */
function ProcDots({ g }: { g: GateStatusMsg }) {
  const procs = [
    { key: 'cam',  ok: g.camera_online, label: 'Kamera' },
    { key: 'det',  ok: g.proc_detector, label: 'Detektor' },
    { key: 'est',  ok: g.proc_estimator,label: 'Estimator' },
    { key: 'tar',  ok: g.proc_tariff,   label: 'Tarif' },
    { key: 'log',  ok: g.proc_logger,   label: 'Logger' },
  ]
  const allOk = procs.every(p => p.ok)
  return (
    <div className="sp-gate-card-bottom">
      <div className="sp-gate-serial">
        📷 {g.camera_serial || '—'}
      </div>
      <div className="sp-proc-dots" title={procs.map(p => `${p.label}: ${p.ok?'✓':'✗'}`).join('\n')}>
        {procs.map(p => (
          <div key={p.key} className={`sp-proc-dot ${g.active ? (p.ok ? 'ok' : 'err') : 'off'}`} title={p.label} />
        ))}
        <span className="sp-proc-label">
          {!g.active ? 'nonaktif' : allOk ? '5/5 ok' : `${procs.filter(p=>p.ok).length}/5 ok`}
        </span>
      </div>
    </div>
  )
}

/* ═══════════════════════════════════════════════════════════
   Add Gate Modal
   ═══════════════════════════════════════════════════════════ */
const DEFAULT_FORM: AddGateForm = {
  gate_name: '', gate_id: '', location: '', camera_serial: '',
  confidence_threshold: 0.70, min_valid_points: 200, max_detections: 1,
  start_immediately: true, showAdvanced: false,
}

function AddGateModal({
  onClose, onAdd, cameras, onRefreshCameras, loadingCameras,
}: {
  onClose: () => void
  onAdd: (form: AddGateForm) => Promise<{ success: boolean; message: string }>
  cameras: CameraDeviceMsg[]
  onRefreshCameras: () => void
  loadingCameras: boolean
}) {
  const [form, setForm] = useState<AddGateForm>(DEFAULT_FORM)
  const [error, setError] = useState('')
  const [success, setSuccess] = useState('')
  const [saving, setSaving] = useState(false)

  const set = <K extends keyof AddGateForm>(k: K, v: AddGateForm[K]) =>
    setForm(prev => ({ ...prev, [k]: v }))

  const handleNameChange = (v: string) => {
    set('gate_name', v)
    if (!form.gate_id || form.gate_id === toGateId(form.gate_name)) {
      set('gate_id', toGateId(v))
    }
  }

  const validateForm = (): string => {
    if (!form.gate_name.trim()) return 'Nama gate wajib diisi.'
    if (!GATE_ID_RE.test(form.gate_id)) return 'ID gate: huruf kecil, angka, underscore, diawali huruf.'
    if (!form.camera_serial) return 'Pilih kamera terlebih dahulu.'
    if (!SERIAL_RE.test(form.camera_serial)) return 'Serial kamera tidak valid.'
    if (form.confidence_threshold <= 0 || form.confidence_threshold > 1) return 'Confidence harus 0.01–1.0'
    return ''
  }

  const handleSave = async () => {
    setError('')
    const err = validateForm()
    if (err) { setError(err); return }
    setSaving(true)
    const res = await onAdd(form)
    setSaving(false)
    if (res.success) {
      setSuccess(res.message)
      setTimeout(onClose, 1200)
    } else {
      setError(res.message)
    }
  }

  return (
    <div className="sp-modal-overlay" onClick={e => e.target === e.currentTarget && onClose()}>
      <div className="sp-modal">
        <div className="sp-modal-head">
          <span className="sp-modal-title">Tambah Gate Baru</span>
          <button className="sp-modal-close" onClick={onClose}>✕</button>
        </div>

        <div className="sp-modal-body">

          {/* Nama Gate */}
          <div className="sp-form-field">
            <label className="sp-form-label">Nama Gate <span className="req">*</span></label>
            <input className="sp-form-input" placeholder="Gate Masuk Utara"
              value={form.gate_name} onChange={e => handleNameChange(e.target.value)} />
          </div>

          {/* ID Gate (auto-fill) */}
          <div className="sp-form-field">
            <label className="sp-form-label">ID Namespace ROS <span className="req">*</span></label>
            <input className={`sp-form-input${form.gate_id && !GATE_ID_RE.test(form.gate_id) ? ' error' : ''}`}
              placeholder="gate_masuk_utara"
              value={form.gate_id}
              onChange={e => set('gate_id', e.target.value.toLowerCase().replace(/[^a-z0-9_]/g, ''))} />
            <span className="sp-form-hint">Huruf kecil, angka, underscore. Dipakai sebagai /{form.gate_id || 'gate_id'}/vehicle/...</span>
          </div>

          {/* Lokasi */}
          <div className="sp-form-field">
            <label className="sp-form-label">Lokasi / Deskripsi</label>
            <input className="sp-form-input" placeholder="Pintu masuk utara, dermaga 3"
              value={form.location} onChange={e => set('location', e.target.value)} />
          </div>

          {/* Kamera */}
          <div className="sp-form-field">
            <label className="sp-form-label">Kamera RealSense <span className="req">*</span></label>
            <div className="sp-camera-select-row">
              <input className="sp-form-input" placeholder="Serial number…"
                value={form.camera_serial} readOnly style={{ cursor: 'default' }} />
              <button className={`sp-refresh-btn${loadingCameras ? ' spinning' : ''}`}
                onClick={onRefreshCameras} title="Refresh daftar kamera">
                {I.refresh}
              </button>
            </div>
            <div className="sp-camera-list">
              {cameras.length === 0 ? (
                <div className="sp-camera-empty">
                  {loadingCameras ? 'Memuat…' : 'Tidak ada kamera terdeteksi. Pastikan RealSense terhubung.'}
                </div>
              ) : cameras.map(cam => (
                <div key={cam.serial_number}
                  className={`sp-camera-option${form.camera_serial === cam.serial_number ? ' selected' : ''}`}
                  onClick={() => set('camera_serial', cam.serial_number)}>
                  <div className={`sp-dot ${cam.in_use ? 'sp-dot--connecting' : 'sp-dot--on'}`} />
                  <div>
                    <div className="sp-camera-option-serial">{cam.serial_number}</div>
                    <div className="sp-camera-option-model">{cam.model_name}{cam.in_use ? ` · Dipakai oleh ${cam.assigned_gate_id}` : ''}</div>
                  </div>
                  <span className="sp-camera-option-usb">{cam.usb_type}</span>
                  {form.camera_serial === cam.serial_number && <span style={{ color: 'var(--primary)', marginLeft: 4 }}>{I.check}</span>}
                </div>
              ))}
            </div>
          </div>

          {/* Advanced toggle */}
          <button className="sp-advanced-toggle" onClick={() => set('showAdvanced', !form.showAdvanced)}>
            <span style={{ transform: form.showAdvanced ? 'rotate(180deg)' : 'none', display: 'inline-block', transition: 'transform .2s' }}>{I.chevDown}</span>
            &nbsp;Pengaturan Lanjutan
          </button>

          {form.showAdvanced && (
            <div className="sp-advanced-grid">
              <div className="sp-form-field">
                <label className="sp-form-label">Confidence</label>
                <input className="sp-form-input" type="number" step="0.05" min="0.1" max="1"
                  value={form.confidence_threshold}
                  onChange={e => set('confidence_threshold', parseFloat(e.target.value))} />
              </div>
              <div className="sp-form-field">
                <label className="sp-form-label">Min Points</label>
                <input className="sp-form-input" type="number" min="10"
                  value={form.min_valid_points}
                  onChange={e => set('min_valid_points', parseInt(e.target.value))} />
              </div>
              <div className="sp-form-field">
                <label className="sp-form-label">Max Det.</label>
                <input className="sp-form-input" type="number" min="1" max="10"
                  value={form.max_detections}
                  onChange={e => set('max_detections', parseInt(e.target.value))} />
              </div>
            </div>
          )}

          {/* Start immediately */}
          <label className="sp-check-row">
            <input type="checkbox" checked={form.start_immediately}
              onChange={e => set('start_immediately', e.target.checked)} />
            <span>Langsung aktifkan setelah disimpan</span>
          </label>

          {error   && <div className="sp-form-error">{error}</div>}
          {success && <div className="sp-form-success">{success}</div>}
        </div>

        <div className="sp-modal-footer">
          <button className="sp-btn-sm secondary" onClick={onClose} style={{ padding: '9px 20px', borderRadius: 10 }}>Batal</button>
          <button className="sp-btn-sm primary"   onClick={handleSave} disabled={saving}
            style={{ padding: '9px 20px', borderRadius: 10, opacity: saving ? .7 : 1 }}>
            {saving ? 'Menyimpan…' : 'Simpan Gate'}
          </button>
        </div>
      </div>
    </div>
  )
}

/* ═══════════════════════════════════════════════════════════
   Page: Camera (Live View) — with gate tabs
   ═══════════════════════════════════════════════════════════ */
function CameraPage({
  status, gates, activeGate, setActiveGate, onAddGate,
  dimension, tariff, videoBase, unit, setUnit,
  dims, tariffVal,
}: {
  status: ConnectionStatus
  gates: GateStatusMsg[]; activeGate: string | null
  setActiveGate: (id: string) => void; onAddGate: () => void
  dimension: VehicleDimensionMsg | null; tariff: VehicleTariffMsg | null
  videoBase: string; unit: DimensionUnit; setUnit: (u: DimensionUnit) => void
  dims: { L: string; W: string; H: string }; tariffVal: number
}) {
  const isLive = status === 'connected'
  const dotClass = { connected: 'sp-dot sp-dot--on', connecting: 'sp-dot sp-dot--connecting', disconnected: 'sp-dot sp-dot--off', error: 'sp-dot sp-dot--err' }[status]
  const statusText = { connected: 'Terhubung', connecting: 'Menghubungkan…', disconnected: 'Terputus', error: 'Gagal tersambung' }[status]
  const videoUrl = activeGate ? buildVideoUrl(videoBase, activeGate) : ''
  const activeGateData = gates.find(g => g.gate_id === activeGate)

  return (
    <>
      {/* Gate tabs */}
      <div className="sp-gate-tabs">
        {gates.map(g => (
          <button key={g.gate_id}
            className={`sp-gate-tab${activeGate === g.gate_id ? ' active' : ''}`}
            onClick={() => setActiveGate(g.gate_id)}>
            <div className={`sp-dot ${g.active && g.camera_online ? 'sp-dot--on' : g.active ? 'sp-dot--connecting' : 'sp-dot--off'}`} />
            {g.gate_name}
          </button>
        ))}
      </div>

      <div className="sp-body">
        {/* Center video */}
        <div className="sp-center">
          <div className="sp-center-actions">
            <button className="sp-glass-btn">{I.sun}</button>
            <button className="sp-glass-btn">{I.ruler}</button>
          </div>

          {gates.length === 0 ? (
            <div className="sp-no-gates">
              <div style={{ opacity: .3 }}>{I.camOff}</div>
              <span>Belum ada gate yang dikonfigurasi.</span>
              <button className="sp-no-gates-btn" onClick={onAddGate}>
                {I.plus}&nbsp; Tambah Gate Pertama
              </button>
            </div>
          ) : !activeGate ? (
            <div className="sp-video-placeholder">{I.camOff}<span>Pilih gate di atas.</span></div>
          ) : DEMO_MODE ? (
            <div className="sp-demo-video-wrap">
              <img src="/demo-gate.jpg" className="sp-video" alt="Demo Gate" />
              <div className="sp-demo-video-overlay">
                <span className="sp-demo-scan-line" />
                <div className="sp-demo-detection-box" />
                <span className="sp-demo-label">DETEKSI AKTIF</span>
              </div>
            </div>
          ) : isLive ? (
            <img src={videoUrl} className="sp-video" alt="YOLO stream" />
          ) : (
            <div className="sp-video-placeholder">{I.camOff}<span>Menunggu koneksi ROS…</span></div>
          )}

          {/* Camera panel */}
          {activeGate && (
            <div className="sp-cam-panel">
              <div className="sp-cam-panel-head">
                <span className="sp-cam-panel-title">Kamera</span>
                <span className="sp-cam-sub" style={{ marginBottom: 0 }}>{activeGateData?.gate_name}</span>
              </div>
              <p className="sp-cam-sub">{activeGateData?.location || 'Gate aktif'}</p>
              <div className="sp-cam-item">
                <div className="sp-cam-thumb">📷</div>
                <div className="sp-cam-info">
                  <div className="sp-cam-name">{activeGate}</div>
                  <div className="sp-cam-view">{activeGateData?.camera_serial || '—'}</div>
                  <span className={(activeGateData?.camera_online && isLive) ? 'sp-live-badge' : 'sp-cam-view'}>
                    {(activeGateData?.camera_online && isLive) ? 'Live' : 'Offline'}
                  </span>
                </div>
                <Toggle on={!!(activeGateData?.active && isLive)} />
              </div>
              {dimension && (
                <div className="sp-cam-item">
                  <div className="sp-cam-thumb">🚗</div>
                  <div className="sp-cam-info">
                    <div className="sp-cam-name">{cls(dimension.class_id)}</div>
                    <div className="sp-cam-view">Terdeteksi</div>
                    <span className="sp-live-badge">Live</span>
                  </div>
                  <Toggle on />
                </div>
              )}
            </div>
          )}

          {/* ROS status */}
          <div className="sp-ros-card">
            <div className="sp-ros-icon">{I.link}</div>
            <div className="sp-ros-info">
              <div className="sp-ros-name">ROS Bridge</div>
              <div className="sp-ros-sub">{statusText}</div>
            </div>
            <div className={dotClass} />
          </div>
        </div>

        {/* Right panel */}
        <aside className="sp-right">
          <div className="sp-dev">
            <div className="sp-dev-head">
              <div><div className="sp-dev-title">Deteksi Kendaraan</div><div className="sp-dev-sub">{activeGate || 'Pilih gate'}</div></div>
              <Toggle on={isLive && !!activeGate} />
            </div>
            <div className="sp-dev-vis">{I.truck}</div>
            <div className="sp-slider-row">
              <div className="sp-slider-sun">{I.sun}</div>
              <div className="sp-slider-track">
                <div className="sp-slider-fill" style={{ width: dimension ? '88%' : '0%' }} />
              </div>
              <span className="sp-slider-pct">{dimension ? '88%' : '--'}</span>
            </div>
            <div className="sp-ts">{I.clock}&nbsp;{dimension ? cls(dimension.class_id) : 'Menunggu deteksi…'}</div>
          </div>

          <div className="sp-dev">
            <div className="sp-dev-head">
              <div><div className="sp-dev-title">Dimensi Kendaraan</div><div className="sp-dev-sub">Estimasi real-time</div></div>
              <Toggle on={!!dimension} />
            </div>
            <div className="sp-dim-grid">
              {(['Panjang','Lebar','Tinggi'] as const).map((lbl,i) => (
                <div className="sp-dim-cell" key={lbl}>
                  <div className="sp-dim-lbl">{lbl}</div>
                  <div className="sp-dim-val">{[dims.L,dims.W,dims.H][i]}</div>
                  <div className="sp-dim-unit">{unit}</div>
                </div>
              ))}
            </div>
            <div className="sp-info-row">
              <span className="sp-info-time">{I.clock}&nbsp;{fmtTime(dimension?.updatedAt)}</span>
              <button className="sp-info-badge">{I.ruler}&nbsp;Ukur ulang</button>
            </div>
          </div>

          <div className="sp-dev">
            <div className="sp-dev-head">
              <div><div className="sp-dev-title">Tarif Otomatis</div><div className="sp-dev-sub">{tariff?.category ?? 'Menunggu data'}</div></div>
              <Toggle on={!!tariff} />
            </div>
            <div className="sp-gauge-wrap"><TariffGauge value={tariffVal} /></div>
            <div className="sp-tariff-cat">
              <span>Rp 0</span>
              <span style={{ fontWeight:700, color:'var(--text)' }}>
                {tariffVal > 0 ? `Rp ${tariffVal.toLocaleString('id-ID')}` : '--'}
              </span>
              <span>Rp 150rb</span>
            </div>
            <div className="sp-ts">{I.clock}&nbsp;{fmtTime(tariff?.updatedAt)}</div>
          </div>

          <div className="sp-unit-row">
            <span className="sp-unit-label">Unit dimensi</span>
            <select className="sp-unit-sel" value={unit} onChange={e => setUnit(e.target.value as DimensionUnit)}>
              <option value="mm">Milimeter</option>
              <option value="cm">Sentimeter</option>
              <option value="m">Meter</option>
            </select>
          </div>
        </aside>
      </div>
    </>
  )
}

/* ═══════════════════════════════════════════════════════════
   Page: Settings — connection + gate management
   ═══════════════════════════════════════════════════════════ */
function SettingsPage({
  wsUrl, urlInput, setUrlInput, onReconnect,
  videoInput, setVideoInput, onUpdateVideo,
  status, gates,
  onAddGate, onRemoveGate, onToggleGate,
}: {
  wsUrl: string; urlInput: string; setUrlInput: (v: string) => void; onReconnect: () => void
  videoInput: string; setVideoInput: (v: string) => void; onUpdateVideo: () => void
  status: ConnectionStatus
  gates: GateStatusMsg[]
  onAddGate: () => void
  onRemoveGate: (id: string) => void
  onToggleGate: (id: string, active: boolean) => void
}) {
  const [confirmDelete, setConfirmDelete] = useState<string | null>(null)
  const dotClass = { connected:'sp-dot sp-dot--on', connecting:'sp-dot sp-dot--connecting', disconnected:'sp-dot sp-dot--off', error:'sp-dot sp-dot--err' }[status]
  const statusText = { connected:'Terhubung', connecting:'Menghubungkan…', disconnected:'Terputus', error:'Gagal tersambung' }[status]

  return (
    <div className="sp-page-content">
      <div className="sp-page-inner">

        {/* ── Manajemen Gate ── */}
        <div className="sp-settings-page-title">Manajemen Gate</div>
        <p className="sp-settings-page-sub">Kelola semua gate dan kamera yang terhubung ke sistem.</p>

        <div className="sp-settings-section">
          <div className="sp-settings-section-head">
            <div className="sp-settings-section-icon">{I.camera}</div>
            <div>
              <div className="sp-settings-section-title">Gate Terdaftar</div>
              <div className="sp-settings-section-sub">{gates.length} gate — status diperbarui setiap 5 detik</div>
            </div>
          </div>

          {gates.length === 0 ? (
            <div className="sp-dash-empty" style={{ padding: '20px 0' }}>
              Belum ada gate. Klik "Tambah Gate" untuk memulai.
            </div>
          ) : (
            <div className="sp-gate-list">
              {gates.map(g => (
                <div key={g.gate_id} className={`sp-gate-card${g.active ? ' active' : ''}`}>
                  <div className="sp-gate-card-top">
                    <div>
                      <div className="sp-gate-card-name">{g.gate_name}</div>
                      <div className="sp-gate-card-id">/{g.gate_id}/</div>
                      {g.location && <div className="sp-gate-card-loc">📍 {g.location}</div>}
                    </div>
                    <div className="sp-gate-card-actions">
                      <Toggle on={g.active} onToggle={() => onToggleGate(g.gate_id, !g.active)} />
                      {confirmDelete === g.gate_id ? (
                        <>
                          <button className="sp-gate-card-del" onClick={() => { onRemoveGate(g.gate_id); setConfirmDelete(null) }} title="Konfirmasi hapus">
                            {I.check}
                          </button>
                          <button className="sp-gate-card-del" style={{ background:'var(--border)', borderColor:'var(--border)', color:'var(--muted)' }}
                            onClick={() => setConfirmDelete(null)} title="Batal">✕</button>
                        </>
                      ) : (
                        <button className="sp-gate-card-del" onClick={() => setConfirmDelete(g.gate_id)} title="Hapus gate">
                          {I.trash}
                        </button>
                      )}
                    </div>
                  </div>
                  <ProcDots g={g} />
                </div>
              ))}
            </div>
          )}

          <button className="sp-settings-save-btn" style={{ marginTop: 8, display: 'flex', alignItems: 'center', gap: 6 }}
            onClick={onAddGate}>
            {I.plus}&nbsp;Tambah Gate
          </button>
        </div>

        {/* ── Koneksi ROS ── */}
        <div className="sp-settings-page-title" style={{ marginTop: 28 }}>Pengaturan Koneksi</div>
        <p className="sp-settings-page-sub">Konfigurasi ROS bridge dan video stream.</p>

        <div className="sp-settings-status-card">
          <div className="sp-settings-status-left">
            <div className={dotClass} />
            <div>
              <div className="sp-settings-status-text">{statusText}</div>
              <div className="sp-settings-status-url">{wsUrl}</div>
            </div>
          </div>
        </div>

        <div className="sp-settings-section">
          <div className="sp-settings-section-head">
            <div className="sp-settings-section-icon">{I.wifi}</div>
            <div><div className="sp-settings-section-title">ROS Bridge WebSocket</div></div>
          </div>
          <div className="sp-settings-field">
            <label>URL WebSocket</label>
            <div className="sp-settings-input-row">
              <input value={urlInput} onChange={e => setUrlInput(e.target.value)} placeholder="ws://localhost:9090" />
              <button className="sp-settings-save-btn" onClick={onReconnect}>Sambungkan</button>
            </div>
          </div>
        </div>

        <div className="sp-settings-section">
          <div className="sp-settings-section-head">
            <div className="sp-settings-section-icon">{I.video}</div>
            <div><div className="sp-settings-section-title">Video Stream Base URL</div>
              <div className="sp-settings-section-sub">Stream per gate dibangun otomatis: base/stream?topic=/gate_id/vehicle/detections/...</div>
            </div>
          </div>
          <div className="sp-settings-field">
            <label>Base URL</label>
            <div className="sp-settings-input-row">
              <input value={videoInput} onChange={e => setVideoInput(e.target.value)} placeholder="http://localhost:8080" />
              <button className="sp-settings-save-btn" onClick={onUpdateVideo}>Terapkan</button>
            </div>
            <span className="sp-settings-hint">Contoh: <code>http://localhost:8080</code></span>
          </div>
        </div>

        <div className="sp-settings-section">
          <div className="sp-settings-section-head">
            <div className="sp-settings-section-icon">{I.link}</div>
            <div><div className="sp-settings-section-title">Topik ROS per Gate</div>
              <div className="sp-settings-section-sub">Pattern — ganti <code>gate_id</code> dengan ID gate sebenarnya</div>
            </div>
          </div>
          <div className="sp-settings-topic-list">
            {[
              ['/{gate_id}/vehicle/dimension_info',  'VehicleDimension',  'Dimensi kendaraan (mm)'],
              ['/{gate_id}/vehicle/tariff_info',     'VehicleTariff',     'Tarif otomatis (IDR)'],
              ['/{gate_id}/vehicle/detections/image_raw','sensor_msgs/Image','Video anotasi YOLOv8'],
              ['/smartport/gates/status',           'GateStatus',        'Status semua gate'],
              ['/smartport/cameras',                'CameraDeviceArray', 'Kamera RealSense terdeteksi'],
            ].map(([t,ty,d]) => (
              <div className="sp-settings-topic-row" key={t}>
                <code className="sp-settings-topic-name">{t}</code>
                <span className="sp-settings-topic-type">{ty}</span>
                <span className="sp-settings-topic-desc">{d}</span>
              </div>
            ))}
          </div>
        </div>

      </div>
    </div>
  )
}

/* ═══════════════════════════════════════════════════════════
   Page: Dashboard
   ═══════════════════════════════════════════════════════════ */
function DashboardPage({ history, status, gates }: { history: HistoryEntry[]; status: ConnectionStatus; gates: GateStatusMsg[] }) {
  const total = history.length
  const totalTariff = history.reduce((s,h) => s + h.tariff, 0)
  const dist = Object.entries(CLASS_LABELS).map(([id,name]) => ({ id:Number(id), name, count:history.filter(h=>h.class_id===Number(id)).length }))
  const maxCount = Math.max(1, ...dist.map(d=>d.count))
  const avgLen = total ? history.reduce((s,h)=>s+h.length_mm,0)/total/1000 : 0
  const avgWid = total ? history.reduce((s,h)=>s+h.width_mm,0)/total/1000 : 0
  const avgHei = total ? history.reduce((s,h)=>s+h.height_mm,0)/total/1000 : 0
  const isConn = status === 'connected'

  return (
    <div className="sp-page-content">
      <div className="sp-page-inner">
        <div className="sp-settings-page-title">Dashboard</div>
        <p className="sp-settings-page-sub">Ringkasan sesi aktif — {gates.length} gate terdaftar.</p>

        <div className="sp-dash-grid">
          <div className="sp-stat-card">
            <div className="sp-stat-card-head"><div className="sp-stat-icon">{I.truck}</div><div><div className="sp-stat-label">Total Kendaraan</div><div className="sp-stat-sub">Sesi ini</div></div></div>
            <div className="sp-stat-number">{total}</div>
            <div className="sp-stat-number-sub">kendaraan terdeteksi</div>
          </div>
          <div className="sp-stat-card">
            <div className="sp-stat-card-head"><div className="sp-stat-icon"><svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><line x1="12" y1="1" x2="12" y2="23"/><path d="M17 5H9.5a3.5 3.5 0 0 0 0 7h5a3.5 3.5 0 0 1 0 7H6"/></svg></div><div><div className="sp-stat-label">Total Pendapatan</div><div className="sp-stat-sub">Sesi ini</div></div></div>
            <div className="sp-stat-number" style={{fontSize:'1.5rem'}}>{totalTariff>=1_000_000?`${(totalTariff/1_000_000).toFixed(2)}jt`:totalTariff.toLocaleString('id-ID')}</div>
            <div className="sp-stat-number-sub">IDR</div>
          </div>
          <div className="sp-stat-card">
            <div className="sp-stat-card-head"><div className="sp-stat-icon">{I.ruler}</div><div><div className="sp-stat-label">Rata-rata Dimensi</div><div className="sp-stat-sub">P × L × T (m)</div></div></div>
            {total>0 ? (
              <div style={{display:'flex',gap:12,marginTop:4}}>
                {[['P',avgLen],['L',avgWid],['T',avgHei]].map(([lbl,v])=>(
                  <div key={lbl as string} className="sp-dim-cell" style={{flex:1}}>
                    <div className="sp-dim-lbl">{lbl as string}</div>
                    <div className="sp-dim-val">{(v as number).toFixed(2)}</div>
                    <div className="sp-dim-unit">m</div>
                  </div>
                ))}
              </div>
            ) : <div className="sp-stat-number-sub" style={{paddingTop:8}}>Belum ada data</div>}
          </div>
          <div className="sp-stat-card">
            <div className="sp-stat-card-head"><div className="sp-stat-icon"><svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"><polyline points="22 12 18 12 15 21 9 3 6 12 2 12"/></svg></div><div><div className="sp-stat-label">Status Gate</div><div className="sp-stat-sub">{gates.filter(g=>g.active).length}/{gates.length} aktif</div></div></div>
            <div className="sp-node-list">
              {gates.length===0 ? <div className="sp-dash-empty">Belum ada gate.</div> : gates.map(g=>(
                <div className="sp-node-row" key={g.gate_id}>
                  <span className="sp-node-name">{g.gate_name}</span>
                  <span className="sp-node-status">
                    <div className={`sp-dot ${g.active&&g.camera_online&&isConn?'sp-dot--on':g.active?'sp-dot--connecting':'sp-dot--off'}`}/>
                    {g.active&&isConn?`Online · ${g.total_detected} kend.`:'Offline'}
                  </span>
                </div>
              ))}
            </div>
          </div>
          <div className="sp-stat-card sp-stat-card--wide">
            <div className="sp-stat-card-head"><div className="sp-stat-icon">{I.chart}</div><div><div className="sp-stat-label">Distribusi Jenis Kendaraan</div></div></div>
            {total===0 ? <div className="sp-dash-empty">Belum ada kendaraan terdeteksi.</div> : (
              <div className="sp-bar-list">
                {dist.map(d=>(
                  <div className="sp-bar-item" key={d.id}>
                    <span className="sp-bar-name">{d.name}</span>
                    <div className="sp-bar-track"><div className="sp-bar-fill" style={{width:`${(d.count/maxCount)*100}%`}}/></div>
                    <span className="sp-bar-count">{d.count}</span>
                  </div>
                ))}
              </div>
            )}
          </div>
          <div className="sp-stat-card sp-stat-card--wide">
            <div className="sp-stat-card-head"><div className="sp-stat-icon">{I.clock}</div><div><div className="sp-stat-label">Riwayat Terbaru</div><div className="sp-stat-sub">10 kendaraan terakhir</div></div></div>
            {history.length===0 ? <div className="sp-dash-empty">Belum ada riwayat.</div> : (
              <table className="sp-history-table">
                <thead><tr><th>Waktu</th><th>Gate</th><th>Kategori</th><th>P×L×T (m)</th><th>Tarif</th></tr></thead>
                <tbody>
                  {history.slice(0,10).map((h,i)=>(
                    <tr key={i}>
                      <td>{fmtDateTime(h.time)}</td>
                      <td><code style={{fontSize:'.7rem'}}>{(h as HistoryEntry & {gate_id?:string}).gate_id||'—'}</code></td>
                      <td><span className="sp-category-chip">{h.category}</span></td>
                      <td>{(h.length_mm/1000).toFixed(2)}×{(h.width_mm/1000).toFixed(2)}×{(h.height_mm/1000).toFixed(2)}</td>
                      <td><span className="sp-tariff-text">Rp {h.tariff.toLocaleString('id-ID')}</span></td>
                    </tr>
                  ))}
                </tbody>
              </table>
            )}
          </div>
        </div>
      </div>
    </div>
  )
}

/* ═══════════════════════════════════════════════════════════
   Page: Log
   ═══════════════════════════════════════════════════════════ */
function LogPage({ logs, onClear }: { logs: LogEntry[]; onClear: () => void }) {
  const tc: Record<string,string> = { info:'#8B91B3', data:'#5C64D4', error:'#ef4444' }
  const tb: Record<string,string> = { info:'#F4F5FB', data:'#ECEDF9', error:'#FEF2F2' }
  return (
    <div className="sp-page-content">
      <div className="sp-page-inner">
        <div className="sp-log-page-head">
          <div><div className="sp-settings-page-title">Log Aktivitas</div><p className="sp-settings-page-sub">{logs.length} entri real-time.</p></div>
          <button className="sp-log-clear-btn" onClick={onClear} disabled={logs.length===0}>{I.trash}&nbsp;Hapus log</button>
        </div>
        {logs.length===0 ? (
          <div className="sp-log-empty-state"><div style={{opacity:.3}}>{I.log}</div><span>Belum ada aktivitas.</span></div>
        ) : (
          <div className="sp-log-table">
            <div className="sp-log-table-head"><span>Waktu</span><span>Tipe</span><span>Pesan</span></div>
            {logs.map((e,i)=>(
              <div className="sp-log-table-row" key={i}>
                <span className="sp-log-time">{e.time}</span>
                <span className="sp-log-type-badge" style={{color:tc[e.type],background:tb[e.type]}}>{e.type}</span>
                <span className="sp-log-msg">{e.text}</span>
              </div>
            ))}
          </div>
        )}
      </div>
    </div>
  )
}

/* ═══════════════════════════════════════════════════════════
   Main App
   ═══════════════════════════════════════════════════════════ */
export default function App() {
  const [page, setPage]             = useState<Page>('camera')
  const [status, setStatus]         = useState<ConnectionStatus>('disconnected')
  const [connected, setConnected]   = useState(false)
  const [wsUrl, setWsUrl]           = useState(DEFAULT_WS_URL)
  const [urlInput, setUrlInput]     = useState(DEFAULT_WS_URL)
  const [videoBase, setVideoBase]   = useState(DEFAULT_VIDEO_BASE)
  const [videoInput, setVideoInput] = useState(DEFAULT_VIDEO_BASE)

  const [dimension, setDimension]   = useState<VehicleDimensionMsg | null>(null)
  const [tariff, setTariff]         = useState<VehicleTariffMsg | null>(null)
  const [logs, setLogs]             = useState<LogEntry[]>([])
  const [history, setHistory]       = useState<(HistoryEntry & { gate_id?: string })[]>([])
  const [unit, setUnit]             = useState<DimensionUnit>('m')

  const [gates, setGates]           = useState<GateStatusMsg[]>([])
  const [cameras, setCameras]       = useState<CameraDeviceMsg[]>([])
  const [activeGate, setActiveGate] = useState<string | null>(null)
  const [loadingCameras, setLoadingCameras] = useState(false)
  const [showAddGate, setShowAddGate] = useState(false)

  const rosRef    = useRef<ROSLIB.Ros | null>(null)
  const gatesMap  = useRef<Map<string, GateStatusMsg>>(new Map())

  /* ── Log helper ── */
  const pushLog = useCallback((text: string, type: LogEntry['type'] = 'info') => {
    const time = new Date().toLocaleTimeString('id-ID', { hour:'2-digit', minute:'2-digit', second:'2-digit' })
    setLogs(prev => [{ time, text, type }, ...prev.slice(0, 99)])
  }, [])

  /* ── Effect 0: Demo simulation (hanya aktif saat DEMO_MODE=true) ── */
  useEffect(() => {
    if (!DEMO_MODE) return

    setStatus('connected')
    setConnected(true)
    setGates(DEMO_GATES.map(g => ({ ...g })))
    setCameras(DEMO_CAMERAS)
    setActiveGate(DEMO_GATES[0].gate_id)
    setHistory(generateInitialHistory() as (HistoryEntry & { gate_id?: string })[])
    pushLog('[DEMO] Sistem demo diaktifkan — data disimulasikan.', 'info')
    pushLog('[DEMO] Tidak ada koneksi ROS yang diperlukan.', 'info')

    let gateIndex = 0
    const interval = setInterval(() => {
      const gateId  = DEMO_GATES[gateIndex % DEMO_GATES.length].gate_id
      gateIndex++
      const det = makeDemoDetection(gateId)
      setDimension(det.dimension as VehicleDimensionMsg)
      setTariff(det.tariff as VehicleTariffMsg)
      setHistory(prev => [det.historyEntry as (HistoryEntry & { gate_id?: string }), ...prev.slice(0, 99)])
      setGates(prev => prev.map(g =>
        g.gate_id === gateId
          ? { ...g, total_detected: g.total_detected + 1 }
          : g
      ))
      pushLog(`[DEMO] [${gateId}] ${det.vehicle.label} — Rp ${det.vehicle.tariff.toLocaleString('id-ID')}`, 'data')
    }, 5_000)

    return () => clearInterval(interval)
  }, [pushLog]) // eslint-disable-line react-hooks/exhaustive-deps

  /* ── Effect 1: ROS connection (dilewati saat DEMO_MODE) ── */
  useEffect(() => {
    if (DEMO_MODE) return
    setStatus('connecting')
    setConnected(false)
    pushLog(`Menghubungkan ke ${wsUrl}`, 'info')

    const ros = new ROSLIB.Ros({ url: wsUrl })
    rosRef.current = ros

    ros.on('connection', () => {
      setStatus('connected'); setConnected(true)
      pushLog('ROS bridge terhubung.', 'info')
    })
    ros.on('error', (e: unknown) => {
      setStatus('error'); setConnected(false)
      pushLog(`Error: ${String((e as Error)?.message ?? e)}`, 'error')
    })
    ros.on('close', () => {
      setStatus('disconnected'); setConnected(false)
      pushLog('Koneksi ditutup.', 'info')
    })

    return () => {
      ros.close()
      rosRef.current = null
      setConnected(false)
      gatesMap.current.clear()
      setGates([]); setCameras([])
    }
  }, [wsUrl, pushLog])

  /* ── Effect 2: System topics (gates + cameras) — dilewati saat DEMO_MODE ── */
  useEffect(() => {
    if (DEMO_MODE || !connected || !rosRef.current) return
    const ros = rosRef.current

    const gatesTopic = new ROSLIB.Topic({
      ros, name: '/smartport/gates/status',
      messageType: 'vehicle_dimension_msgs/GateStatus',
    })
    gatesTopic.subscribe((msg) => {
      const m = msg as GateStatusMsg
      gatesMap.current.set(m.gate_id, m)
      const arr = Array.from(gatesMap.current.values())
      setGates(arr)
      setActiveGate(prev => prev ?? (arr[0]?.gate_id ?? null))
    })

    const camTopic = new ROSLIB.Topic({
      ros, name: '/smartport/cameras',
      messageType: 'vehicle_dimension_msgs/CameraDeviceArray',
    })
    camTopic.subscribe((msg) => {
      const m = msg as { devices: CameraDeviceMsg[] }
      setCameras(m.devices ?? [])
    })

    return () => { gatesTopic.unsubscribe(); camTopic.unsubscribe() }
  }, [connected])

  /* ── Effect 3: Active gate topics — dilewati saat DEMO_MODE ── */
  useEffect(() => {
    if (DEMO_MODE) return
    if (!connected || !rosRef.current || !activeGate) {
      setDimension(null); setTariff(null); return
    }
    const ros = rosRef.current

    const dimTopic = new ROSLIB.Topic({
      ros, name: `/${activeGate}/vehicle/dimension_info`,
      messageType: 'vehicle_dimension_msgs/VehicleDimension',
    })
    const tarTopic = new ROSLIB.Topic({
      ros, name: `/${activeGate}/vehicle/tariff_info`,
      messageType: 'vehicle_dimension_msgs/VehicleTariff',
    })

    dimTopic.subscribe((msg) => {
      const m = msg as VehicleDimensionMsg
      setDimension({ ...m, updatedAt: rosTimeToDate(m?.header?.stamp) ?? new Date() })
      pushLog(`[${activeGate}] Dimensi — ${cls(m.class_id)}`, 'data')
    })
    tarTopic.subscribe((msg) => {
      const m = msg as VehicleTariffMsg
      const ts = rosTimeToDate(m?.header?.stamp) ?? new Date()
      setTariff({ ...m, updatedAt: ts })
      pushLog(`[${activeGate}] Tarif Rp ${(m.tariff??0).toLocaleString('id-ID')}`, 'data')
      if (m.dimension) {
        setHistory(prev => [{
          time: ts, gate_id: activeGate,
          class_id: m.dimension!.class_id ?? 0,
          category: m.category ?? 'unknown',
          length_mm: m.dimension!.length ?? 0,
          width_mm: m.dimension!.width ?? 0,
          height_mm: m.dimension!.height ?? 0,
          tariff: m.tariff ?? 0,
          currency: m.currency ?? 'IDR',
        }, ...prev.slice(0, 99)])
      }
    })

    return () => { dimTopic.unsubscribe(); tarTopic.unsubscribe() }
  }, [connected, activeGate, pushLog])

  /* ── Service call helper ── */
  const callService = useCallback(<TReq, TRes>(
    name: string, serviceType: string, request: TReq,
    onSuccess: (r: TRes) => void, onError?: (e: string) => void,
  ) => {
    if (!rosRef.current) { onError?.('ROS tidak terhubung.'); return }
    const svc = new ROSLIB.Service({ ros: rosRef.current, name, serviceType })
    svc.callService(
      request as object,
      (res: unknown) => onSuccess(res as TRes),
      onError,
    )
  }, [])

  /* ── Fetch cameras (service call) ── */
  const fetchCameras = useCallback(() => {
    setLoadingCameras(true)
    callService<object, { devices: CameraDeviceMsg[] }>(
      '/smartport/cameras/list', 'vehicle_dimension_msgs/ListCameras', {},
      (res) => { setCameras(res.devices ?? []); setLoadingCameras(false) },
      (err) => { pushLog(`Gagal ambil kamera: ${err}`, 'error'); setLoadingCameras(false) },
    )
  }, [callService, pushLog])

  /* ── Add gate ── */
  const handleAddGate = useCallback(async (form: AddGateForm) => {
    return new Promise<{ success: boolean; message: string }>((resolve) => {
      callService<object, { success: boolean; message: string }>(
        '/smartport/gates/add', 'vehicle_dimension_msgs/AddGate',
        {
          gate_id: form.gate_id, gate_name: form.gate_name,
          location: form.location, camera_serial: form.camera_serial,
          confidence_threshold: form.confidence_threshold,
          min_valid_points: form.min_valid_points,
          max_detections: form.max_detections,
          start_immediately: form.start_immediately,
        },
        (res) => {
          if (res.success) pushLog(`Gate '${form.gate_id}' ditambahkan.`, 'info')
          resolve(res)
        },
        (err) => resolve({ success: false, message: err ?? 'Service error.' }),
      )
    })
  }, [callService, pushLog])

  /* ── Remove gate ── */
  const handleRemoveGate = useCallback((gateId: string) => {
    callService<object, { success: boolean; message: string }>(
      '/smartport/gates/remove', 'vehicle_dimension_msgs/RemoveGate',
      { gate_id: gateId, force: true },
      (res) => {
        if (res.success) {
          gatesMap.current.delete(gateId)
          setGates(Array.from(gatesMap.current.values()))
          if (activeGate === gateId) setActiveGate(null)
          pushLog(`Gate '${gateId}' dihapus.`, 'info')
        } else {
          pushLog(`Gagal hapus gate: ${res.message}`, 'error')
        }
      },
      (err) => pushLog(`Error hapus gate: ${err}`, 'error'),
    )
  }, [callService, activeGate, pushLog])

  /* ── Toggle gate active ── */
  const handleToggleGate = useCallback((gateId: string, active: boolean) => {
    callService<object, { success: boolean; message: string }>(
      '/smartport/gates/set_active', 'vehicle_dimension_msgs/SetGateActive',
      { gate_id: gateId, active },
      (res) => { if (!res.success) pushLog(`Gagal toggle gate: ${res.message}`, 'error') },
      (err) => pushLog(`Error toggle gate: ${err}`, 'error'),
    )
  }, [callService, pushLog])

  /* ── Dimension conversion ── */
  const cvt = useCallback((mm?: number) => {
    if (mm == null) return '--'
    return unit==='cm' ? (mm/10).toFixed(1) : unit==='m' ? (mm/1000).toFixed(2) : mm.toFixed(0)
  }, [unit])
  const dims = useMemo(() => ({ L:cvt(dimension?.length), W:cvt(dimension?.width), H:cvt(dimension?.height) }), [dimension, cvt])
  const tariffVal = tariff?.tariff ?? 0

  const pageTitles: Record<Page,string> = {
    camera:'SmartPortCam Live View', dashboard:'Dashboard',
    settings:'Pengaturan', log:'Log Aktivitas',
  }
  const navItems = [
    { key:'camera' as Page, icon:I.camera, label:'Live View' },
    { key:'dashboard' as Page, icon:I.chart, label:'Dashboard' },
    { key:'settings' as Page, icon:I.gear, label:'Pengaturan' },
    { key:'log' as Page, icon:I.log, label:'Log' },
  ]

  return (
    <>
      {/* Add Gate Modal */}
      {showAddGate && (
        <AddGateModal
          onClose={() => setShowAddGate(false)}
          onAdd={handleAddGate}
          cameras={cameras}
          onRefreshCameras={fetchCameras}
          loadingCameras={loadingCameras}
        />
      )}

      <div className="sp-shell">
        {/* Sidebar */}
        <aside className="sp-sidebar">
          <div className="sp-logo">SPC</div>
          <nav className="sp-nav">
            {navItems.map(n => (
              <button key={n.key} className={`sp-nav-btn${page===n.key?' active':''}`}
                title={n.label} onClick={() => setPage(n.key)}>
                {n.icon}
              </button>
            ))}
          </nav>
          <button className="sp-nav-more">{I.more}</button>
        </aside>

        {/* Main */}
        <div className="sp-main">
          <header className="sp-header">
            <div className="sp-header-left">
              <div className="sp-header-toolbar">
                <button className="sp-icon-btn" onClick={() => setPage('camera')}>{I.camera}</button>
                <button className="sp-icon-btn" onClick={() => setPage('settings')}>{I.pencil}</button>
              </div>
              <div style={{ display: 'flex', alignItems: 'center', gap: 10 }}>
                <h1 className="sp-header-title">{pageTitles[page]}</h1>
                {DEMO_MODE && <span className="sp-demo-badge">DEMO</span>}
              </div>
            </div>
            <div className="sp-header-right" />
          </header>

          {page==='camera' && (
            <CameraPage
              status={status} gates={gates} activeGate={activeGate}
              setActiveGate={setActiveGate}
              onAddGate={() => { fetchCameras(); setShowAddGate(true) }}
              dimension={dimension} tariff={tariff}
              videoBase={videoBase} unit={unit} setUnit={setUnit}
              dims={dims} tariffVal={tariffVal}
            />
          )}
          {page==='dashboard' && <DashboardPage history={history} status={status} gates={gates} />}
          {page==='settings' && (
            <SettingsPage
              wsUrl={wsUrl} urlInput={urlInput} setUrlInput={setUrlInput}
              onReconnect={() => setWsUrl(urlInput.trim()||DEFAULT_WS_URL)}
              videoInput={videoInput} setVideoInput={setVideoInput}
              onUpdateVideo={() => setVideoBase(videoInput.trim()||DEFAULT_VIDEO_BASE)}
              status={status} gates={gates}
              onAddGate={() => { fetchCameras(); setShowAddGate(true) }}
              onRemoveGate={handleRemoveGate} onToggleGate={handleToggleGate}
            />
          )}
          {page==='log' && <LogPage logs={logs} onClear={() => setLogs([])} />}
        </div>
      </div>
    </>
  )
}
