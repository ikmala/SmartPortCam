# SmartPortCam — Web Dashboard

Dashboard web real-time untuk sistem SmartPortCam. Menampilkan live feed kamera per gate, dimensi kendaraan, tarif otomatis, statistik harian, dan manajemen gate — tanpa reload halaman.

Tersedia dalam **2 mode**: **Demo** (data simulasi, tanpa ROS) dan **Production** (terhubung ke ROS 2 bridge).

---

## Tech Stack

| Komponen | Teknologi |
|---|---|
| Framework | React 19 + TypeScript |
| Build tool | Vite 7 |
| Styling | Tailwind CSS 4 + Custom CSS |
| ROS bridge | roslib (WebSocket) |
| Video stream | MJPEG via `web_video_server` |

---

## Prasyarat

- Node.js 18+
- npm 9+
- *(Production only)* ROS 2 Humble + `rosbridge_server` + `web_video_server` berjalan

---

## Quick Start

```bash
cd web_dashboard

# Install dependencies (sekali saja)
npm install

# ── Mode Demo (tanpa ROS, data simulasi) ──
npm run dev:demo
# Buka: http://localhost:5173

# ── Mode Production (konek ke ROS bridge) ──
npm run dev
# Buka: http://localhost:5173
```

---

## Demo Mode vs Production Mode

| | Demo | Production |
|---|---|---|
| **Koneksi ROS** | Tidak perlu | Wajib (`rosbridge_server`) |
| **Data** | Simulasi otomatis setiap 5 detik | Real-time dari node ROS 2 |
| **Video** | Gambar statis (`public/demo-gate.jpg`) | MJPEG stream dari `web_video_server` |
| **Gate** | 2 gate bawaan (Gate Masuk, Gate Keluar) | Dikelola via `gate_manager` node |
| **Badge** | Kuning "DEMO" di header | Tidak ada |
| **Database** | Tidak ada | PostgreSQL via `data_logger` node |

### Menyiapkan Demo

1. Salin gambar gate ke folder `public/`:
   ```bash
   cp /path/ke/foto-gate.jpg public/demo-gate.jpg
   ```
2. Jalankan:
   ```bash
   npm run dev:demo
   ```

Tanpa `demo-gate.jpg`, live view menampilkan placeholder. Dashboard, statistik, dan log tetap berjalan normal dengan data simulasi.

---

## Scripts

```bash
npm run dev           # Development — Production mode
npm run dev:demo      # Development — Demo mode
npm run build         # Build production
npm run build:demo    # Build demo
npm run preview       # Preview production build
npm run preview:demo  # Preview demo build
npm run lint          # ESLint check
```

---

## Konfigurasi

### `.env` (Production)
```env
VITE_ROSBRIDGE_URL=ws://localhost:9090
VITE_VIDEO_BASE=http://localhost:8080
```

### `.env.demo` (Demo — sudah ada, tidak perlu diubah)
```env
VITE_DEMO_MODE=true
VITE_ROSBRIDGE_URL=ws://localhost:9090
VITE_VIDEO_BASE=http://localhost:8080
```

Vite otomatis memuat `.env.demo` saat `--mode demo` digunakan.

| Variabel | Default | Keterangan |
|---|---|---|
| `VITE_DEMO_MODE` | `false` | `true` = aktifkan demo mode |
| `VITE_ROSBRIDGE_URL` | `ws://localhost:9090` | URL rosbridge WebSocket |
| `VITE_VIDEO_BASE` | `http://localhost:8080` | Base URL `web_video_server` |

URL video per gate dibangun otomatis:
```
{VITE_VIDEO_BASE}/stream?topic=/{gate_id}/vehicle/detections/image_raw&type=mjpeg
```

---

## Halaman Dashboard

### Live View (`/camera`)
- **Tab per gate** — switch antar gate secara real-time
- **Video feed** — MJPEG stream dari `web_video_server` (atau gambar statis di demo)
- **Overlay animasi** — bounding box + scan line di demo mode
- **Camera panel** (kiri atas) — nama gate, serial kamera, status live
- **ROS Bridge card** (kiri bawah) — status koneksi WebSocket
- **Panel kanan** — Deteksi Kendaraan · Dimensi · Tarif Otomatis

### Dashboard (`/dashboard`)
- Total kendaraan & total pendapatan (sesi aktif)
- Rata-rata dimensi (P × L × T dalam meter)
- Status semua gate (online/offline + jumlah kendaraan terdeteksi)
- Bar chart distribusi jenis kendaraan
- Tabel riwayat 10 kendaraan terakhir (dengan kolom gate)

### Pengaturan (`/settings`)
- **Manajemen Gate** — daftar gate, toggle aktif/nonaktif, hapus (konfirmasi 2-klik)
- **Tambah Gate** — modal form: nama, ID namespace, lokasi, pilih kamera USB (dari service ROS)
- **Koneksi ROS** — ubah URL rosbridge + reconnect
- **Video Stream** — ubah base URL `web_video_server`
- **Topik ROS** — referensi topik yang digunakan (read-only)

### Log Aktivitas (`/log`)
- Semua event dengan timestamp, tipe (info / data / error)
- Tombol hapus log
- Di demo mode: event bertanda `[DEMO]`

---

## Struktur Folder

```
web_dashboard/
├── public/
│   └── demo-gate.jpg       ← gambar demo (taruh sendiri, tidak di-commit)
├── src/
│   ├── demo/
│   │   └── demoData.ts     ← data dummy, skenario kendaraan, simulasi
│   ├── types.ts            ← shared TypeScript interfaces
│   ├── App.tsx             ← komponen utama (semua halaman)
│   ├── App.css             ← semua styling custom
│   ├── main.tsx            ← entry point
│   └── index.css           ← global reset
├── .env.example            ← template env production
├── .env.demo               ← env demo (sudah dikonfigurasi)
├── package.json
├── vite.config.ts
└── tsconfig.app.json
```

---

## ROS Topics yang Disubscribe

| Topic | Tipe ROS | Keterangan |
|---|---|---|
| `/{gate_id}/vehicle/dimension_info` | `VehicleDimension` | Dimensi kendaraan (mm) |
| `/{gate_id}/vehicle/tariff_info` | `VehicleTariff` | Tarif (IDR) |
| `/smartport/gates/status` | `GateStatus` | Status tiap gate (5 detik sekali) |
| `/smartport/cameras` | `CameraDeviceArray` | Kamera USB terdeteksi |

## ROS Services yang Dipanggil

| Service | Tipe | Trigger |
|---|---|---|
| `/smartport/gates/add` | `AddGate` | Form "Tambah Gate" |
| `/smartport/gates/remove` | `RemoveGate` | Tombol hapus di Settings |
| `/smartport/gates/set_active` | `SetGateActive` | Toggle on/off gate |
| `/smartport/cameras/list` | `ListCameras` | Buka form / klik Refresh |

---

## Demo Simulation

Data simulasi dihasilkan oleh `src/demo/demoData.ts`:

- **Interval**: deteksi baru setiap **5 detik**, bergantian antara gate
- **Distribusi kendaraan** (realistik untuk pelabuhan):

| Jenis | Probabilitas | Panjang | Tarif |
|---|---|---|---|
| Truk Berat | 45% | 8–14 m | Rp 150.000 |
| Bus | 20% | 9–12.5 m | Rp 120.000 |
| Mobil | 25% | 3.8–5.2 m | Rp 50.000 |
| Sepeda Motor | 10% | 1.8–2.3 m | Rp 20.000 |

- **Histori awal**: 20 entri pre-populated agar dashboard langsung terisi
- **Gate**: 2 gate bawaan dengan total_detected awal (47 dan 23)

---

## Build & Deploy Production

```bash
# Build
npm run build
# Output: dist/

# Serve dengan nginx (contoh)
server {
    listen 80;
    root /var/www/smartportcam/dist;
    index index.html;
    location / { try_files $uri $uri/ /index.html; }
}
```

Untuk deploy demo (misalnya di laptop presentasi):
```bash
npm run build:demo
# Serve dist/ dengan nginx atau: npx serve dist
```

---

## Troubleshooting

**Dashboard tidak menerima data**
- Pastikan `rosbridge_server` berjalan: `ros2 launch rosbridge_server rosbridge_websocket_launch.xml`
- Cek URL di Settings → sama dengan IP server ROS

**Video tidak muncul**
- Pastikan `web_video_server` berjalan
- Pastikan `publish_annotated_image: true` di `config/smartport.yaml`
- Cek Base URL video di Settings

**Demo image tidak muncul**
- Pastikan file ada di `web_dashboard/public/demo-gate.jpg`
- Jalankan ulang `npm run dev:demo`
