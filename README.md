# SmartPortCam

Sistem deteksi dan pengukuran kendaraan otomatis berbasis **ROS 2 + YOLOv8 + Intel RealSense** untuk keperluan pelabuhan. Mendukung **banyak gate sekaligus** dari satu server, menghitung tarif otomatis berdasarkan dimensi kendaraan, dan menyimpan seluruh riwayat ke **PostgreSQL**.

---

## Daftar Isi

- [Fitur](#fitur)
- [Arsitektur](#arsitektur)
- [Prasyarat](#prasyarat)
- [Instalasi Cepat](#instalasi-cepat)
- [Konfigurasi](#konfigurasi)
- [Menjalankan Sistem](#menjalankan-sistem)
- [Manajemen Gate](#manajemen-gate)
- [Web Dashboard](#web-dashboard)
- [Mode Demo](#mode-demo)
- [Database & Riwayat Data](#database--riwayat-data)
- [Training Model Kustom](#training-model-kustom)
- [Struktur Folder](#struktur-folder)
- [Spesifikasi Hardware](#spesifikasi-hardware)
- [Security](#security)
- [Troubleshooting](#troubleshooting)

---

## Fitur

- Deteksi kendaraan real-time pakai **YOLOv8** (mobil, motor, bus, truk)
- Estimasi dimensi 3D (panjang × lebar × tinggi) dari Intel RealSense depth camera
- Kalkulasi tarif otomatis berdasarkan dimensi dan jenis kendaraan
- **Web dashboard tersedia dalam 2 versi** — Demo (tanpa hardware) dan Production (ROS 2 penuh)
- **Multi-gate** — satu server mengelola banyak gate/kamera sekaligus
- **Gate Manager** — tambah, hapus, aktifkan/nonaktifkan gate secara dinamis tanpa restart sistem
- **Camera Manager** — deteksi otomatis kamera RealSense yang terhubung via USB
- Penyimpanan histori ke **PostgreSQL** (setiap kendaraan tercatat dengan gate_id)
- **Web Dashboard** (React) — live feed per gate, dimensi, tarif, statistik, log
- Namespace ROS 2 terisolasi per gate (`/gate_1/`, `/gate_2/`, …)
- Konfigurasi terpusat via `config/smartport.yaml` dan `config/gates.json`
- Script deploy satu perintah untuk semua gate

---

## Arsitektur

### Alur per Gate

```
Kamera RealSense D435/D455 (USB)
  │  /{gate_id}/cam/color/image_raw
  │  /{gate_id}/cam/aligned_depth_to_color/image_raw
  ▼
[yolov8_detector]          /{gate_id}/vehicle/bounding_boxes
        │
        ▼
[dimension_estimator]      /{gate_id}/vehicle/dimension_info  (mm)
        │
        ▼
[tariff_calculator]        /{gate_id}/vehicle/tariff_info  (IDR)
        │
        ├──► [data_logger]     → PostgreSQL  (gate_id, dimensi, tarif)
        └──► Web Dashboard     ← rosbridge WebSocket
```

### Sistem Multi-Gate (1 Server)

```
SERVER
│
├── gate_manager        ← baca gates.json, spawn & monitor semua gate
├── camera_manager      ← deteksi kamera RealSense tiap 5 detik
├── rosbridge_server    ← WebSocket untuk web dashboard
├── web_video_server    ← MJPEG stream per gate
├── PostgreSQL (Docker) ← histori semua gate
│
├── /gate_1/ ─── RealSense SN:AAAA ─── YOLOv8 ─── Estimator ─── Tarif ─── Logger
├── /gate_2/ ─── RealSense SN:BBBB ─── YOLOv8 ─── Estimator ─── Tarif ─── Logger
└── /gate_N/ ─── ...

Web Dashboard (browser)
├── Tab Gate 1 → subscribe /{gate_1}/vehicle/...
├── Tab Gate 2 → subscribe /{gate_2}/vehicle/...
└── Dashboard  → ringkasan semua gate
```

### ROS 2 Topics & Services

| Topic / Service | Tipe | Keterangan |
|---|---|---|
| `/{gate_id}/cam/color/image_raw` | `sensor_msgs/Image` | RGB dari kamera |
| `/{gate_id}/vehicle/bounding_boxes` | `VehicleBoundingBoxArray` | Hasil deteksi YOLO |
| `/{gate_id}/vehicle/dimension_info` | `VehicleDimension` | Dimensi (mm) |
| `/{gate_id}/vehicle/tariff_info` | `VehicleTariff` | Tarif (IDR) |
| `/{gate_id}/vehicle/detections/image_raw` | `sensor_msgs/Image` | Video anotasi |
| `/smartport/gates/status` | `GateStatus` | Status tiap gate (publish per 5 detik) |
| `/smartport/cameras` | `CameraDeviceArray` | Kamera USB terdeteksi |
| `/smartport/gates/add` | `AddGate` (srv) | Tambah gate baru |
| `/smartport/gates/remove` | `RemoveGate` (srv) | Hapus gate |
| `/smartport/gates/set_active` | `SetGateActive` (srv) | On/off gate |
| `/smartport/gates/list` | `ListGates` (srv) | Daftar semua gate |
| `/smartport/cameras/list` | `ListCameras` (srv) | Daftar kamera USB |

---

## Prasyarat

| Komponen | Versi | Keterangan |
|---|---|---|
| Ubuntu | 22.04 LTS | Wajib |
| ROS 2 | Humble Hawksbill | [Panduan install](https://docs.ros.org/en/humble/Installation.html) |
| Intel RealSense SDK | 2.x | [Panduan install](https://github.com/IntelRealSense/librealsense) |
| Python | 3.10+ | Sudah bawaan Ubuntu 22.04 |
| Docker + Compose | 24+ | Untuk PostgreSQL |
| Node.js | 18+ | Untuk web dashboard |
| NVIDIA GPU | CUDA 11.8+ (opsional) | Untuk inferensi lebih cepat |

---

## Instalasi Cepat

```bash
# 1. Clone repository
git clone https://github.com/<user>/SmartPortCam.git ~/smartport_ws
cd ~/smartport_ws

# 2. Jalankan install script (satu kali saja di mesin baru)
bash install.sh

# 3. Source workspace
source install/setup.bash

# 4. Letakkan model YOLOv8 di folder models/
cp /path/ke/best.pt models/yolov8_smartport.pt

# 5. Jalankan sistem (akan membaca gates.json — kosong di awal)
python3 scripts/smartport_mode.py deploy --with-rosbridge

# 6. Buka web dashboard → Pengaturan → Tambah Gate
cd web_dashboard && npm run dev
# Buka browser: http://localhost:5173
```

> **File `.pt` tidak di-commit ke Git.** Download dari halaman Releases atau lihat [Training Model Kustom](#training-model-kustom).

---

## Konfigurasi

### `config/smartport.yaml` — Parameter ROS (semua node)

```yaml
yolov8_detector:
  ros__parameters:
    model_path: "models/yolov8_smartport.pt"
    confidence_threshold: 0.70
    class_ids: [2, 3, 5, 7]   # COCO: car, motorcycle, bus, truck
    max_detections: 1

tariff_calculator:
  ros__parameters:
    currency: "IDR"
    dimension_rules: >
      [{"name": "motorcycle", "max_length_mm": 2500, "tariff": 20000}, ...]
```

### `config/gates.json` — Konfigurasi Gate

Dibuat otomatis saat pertama kali deploy. Diisi lewat web dashboard atau manual:

```json
{
  "version": "1.0",
  "gates": [
    {
      "id": "gate_1",
      "name": "Gate Masuk Utara",
      "location": "Dermaga 3",
      "camera_serial": "123456789012",
      "active": true,
      "params": {
        "confidence_threshold": 0.70,
        "min_valid_points": 200,
        "max_detections": 1
      }
    }
  ]
}
```

### `.env` — Kredensial & URL

```bash
cp .env.example .env
# Wajib diganti sebelum production:
POSTGRES_PASSWORD=ganti_password_ini
```

| Variabel | Default | Keterangan |
|---|---|---|
| `POSTGRES_HOST` | `localhost` | Host PostgreSQL |
| `POSTGRES_DB` | `smartportcam` | Nama database |
| `POSTGRES_USER` | `smartport` | Username |
| `POSTGRES_PASSWORD` | *(wajib diisi)* | Password — **ganti!** |
| `VITE_ROSBRIDGE_URL` | `ws://localhost:9090` | URL rosbridge untuk dashboard |
| `ROSBRIDGE_ADDRESS` | `0.0.0.0` | Interface binding rosbridge |

---

## Menjalankan Sistem

### Mode Deploy — Multi-Gate (Direkomendasikan)

Membaca `config/gates.json` dan menjalankan semua gate aktif secara otomatis.

```bash
source install/setup.bash

# Tanpa rosbridge (jalankan rosbridge terpisah)
python3 scripts/smartport_mode.py deploy

# Dengan rosbridge + web_video_server sekaligus
python3 scripts/smartport_mode.py deploy --with-rosbridge
```

### Mode Single Gate — Satu Gate Langsung (Testing)

```bash
python3 scripts/smartport_mode.py single \
  --gate-id gate_1 \
  --camera-serial 123456789012 \
  --with-db
```

Flag tersedia:

| Flag | Default | Keterangan |
|---|---|---|
| `--gate-id` | (wajib) | ID gate, contoh: `gate_1` |
| `--camera-serial` | (wajib) | Serial number RealSense |
| `--model-path` | `models/yolov8_smartport.pt` | Path model YOLO |
| `--confidence` | `0.70` | Threshold deteksi |
| `--with-db` | off | Aktifkan data_logger → PostgreSQL |
| `--no-gui` | off | Nonaktifkan jendela Tkinter |

### Mode Train — Rekam Dataset

```bash
python3 scripts/smartport_mode.py train --output-dir ~/datasets/sesi1
```

### Rosbridge Aman (Production)

```bash
source install/setup.bash
ros2 launch smartport rosbridge_secure.launch.py

# Atau sekaligus semua:
ros2 launch smartport smartport.launch.py with_bridge:=true
```

### Web Dashboard

```bash
cd web_dashboard
npm run dev       # Development
npm run build     # Production build → serve dengan nginx
```

Buka browser: `http://localhost:5173`

---

## Manajemen Gate

### Via Web Dashboard (Direkomendasikan)

1. Buka **Pengaturan** (ikon gear di sidebar)
2. Klik **Tambah Gate**
3. Isi form:
   - **Nama Gate** — contoh: "Gate Masuk Utara"
   - **ID Gate** — auto-generate dari nama (bisa diedit)
   - **Lokasi** — opsional
   - **Kamera** — pilih dari dropdown (tombol 🔄 untuk refresh deteksi USB)
   - **Pengaturan Lanjutan** — confidence, min points, max detections
4. Centang **Langsung aktifkan**, klik **Simpan Gate**

Gate langsung muncul sebagai tab di halaman Live View.

### Via ROS 2 Service (Terminal)

```bash
# Tambah gate
ros2 service call /smartport/gates/add vehicle_dimension_msgs/srv/AddGate \
  "{gate_id: 'gate_2', gate_name: 'Gate Keluar Selatan',
    camera_serial: '987654321098', confidence_threshold: 0.70,
    min_valid_points: 200, max_detections: 1, start_immediately: true}"

# Nonaktifkan gate
ros2 service call /smartport/gates/set_active vehicle_dimension_msgs/srv/SetGateActive \
  "{gate_id: 'gate_2', active: false}"

# Hapus gate
ros2 service call /smartport/gates/remove vehicle_dimension_msgs/srv/RemoveGate \
  "{gate_id: 'gate_2', force: false}"

# Lihat semua gate
ros2 service call /smartport/gates/list vehicle_dimension_msgs/srv/ListGates

# Lihat kamera USB yang terhubung
ros2 service call /smartport/cameras/list vehicle_dimension_msgs/srv/ListCameras

# Cek serial number kamera RealSense
rs-enumerate-devices | grep "Serial Number"
```

---

## Web Dashboard

Dashboard berbasis React, terhubung ke ROS 2 via rosbridge WebSocket.
Tersedia dalam **2 versi**: Demo dan Production.

```bash
cd web_dashboard

npm run dev           # Production — konek ke ROS bridge
npm run dev:demo      # Demo — data simulasi, tanpa hardware
```

Lihat dokumentasi lengkap: **[web_dashboard/README.md](web_dashboard/README.md)**

### Halaman yang Tersedia

| Halaman | Isi |
|---|---|
| **Live View** | Video feed per gate, deteksi kendaraan, dimensi, tarif real-time |
| **Dashboard** | Statistik sesi: total kendaraan, pendapatan, distribusi jenis, status gate |
| **Pengaturan** | Manajemen gate, koneksi ROS bridge, video stream URL |
| **Log Aktivitas** | Semua event dengan timestamp dan tipe (info/data/error) |

### Live View

- **Tab per gate** — klik untuk switch; indikator hijau/kuning/abu menunjukkan status kamera
- **Camera panel** (kiri atas) — info gate aktif dan deteksi terkini
- **ROS Bridge card** (kiri bawah) — status koneksi
- **Panel kanan** — Deteksi Kendaraan, Dimensi (mm/cm/m), Tarif Otomatis

### Pengaturan Gate

- Toggle on/off gate tanpa restart sistem
- Hapus gate dengan konfirmasi 2-klik
- Indikator 5 titik per gate: Kamera · Detektor · Estimator · Tarif · Logger

---

## Mode Demo

Versi demo memungkinkan presentasi dan pengujian dashboard **tanpa hardware ROS apapun**.

### Menyiapkan Demo

```bash
# 1. Salin foto gate ke public/
cp /path/ke/foto_gate.jpg web_dashboard/public/demo-gate.jpg

# 2. Jalankan dashboard demo
cd web_dashboard
npm run dev:demo

# 3. Buka browser
# http://localhost:5173
```

### Yang Berjalan Otomatis di Demo

| Fitur | Keterangan |
|---|---|
| **2 gate** | "Gate Masuk" dan "Gate Keluar" sudah terkonfigurasi |
| **Live View** | Gambar `demo-gate.jpg` + bounding box animasi + scan line |
| **Deteksi tiap 5 detik** | Kendaraan acak bergantian di kedua gate |
| **Distribusi realistik** | Truk 45%, Bus 20%, Mobil 25%, Motor 10% |
| **Histori awal** | 20 entri pre-populated agar dashboard langsung terisi |
| **Badge DEMO** | Label kuning di header agar jelas ini versi demo |
| **Log** | Semua event bertanda `[DEMO]` |

### Build Demo untuk Deployment

```bash
cd web_dashboard
npm run build:demo
# Output: web_dashboard/dist/ — bisa di-serve dengan nginx atau npx serve dist
```

---

## Database & Riwayat Data

### Start PostgreSQL

```bash
docker compose up -d
docker compose logs -f postgres    # pantau log
```

### Adminer (UI Database)

Buka: **http://localhost:8081**

| Field | Nilai |
|---|---|
| System | PostgreSQL |
| Server | `postgres` |
| Username | `smartport` |
| Password | *(sesuai .env)* |
| Database | `smartportcam` |

### Skema Tabel

```sql
-- Setiap baris = satu kendaraan yang melewati gate
SELECT * FROM vehicle_records ORDER BY recorded_at DESC LIMIT 20;
```

| Kolom | Tipe | Keterangan |
|---|---|---|
| `id` | SERIAL | Primary key |
| `recorded_at` | TIMESTAMPTZ | Waktu server |
| `ros_timestamp` | TIMESTAMPTZ | Timestamp dari ROS header |
| `gate_id` | VARCHAR(32) | ID gate (contoh: `gate_1`) |
| `class_id` | INTEGER | COCO class (2=mobil, 3=motor, 5=bus, 7=truk) |
| `category` | VARCHAR(50) | Nama kategori tarif |
| `length_mm` | DOUBLE | Panjang kendaraan (mm) |
| `width_mm` | DOUBLE | Lebar kendaraan (mm) |
| `height_mm` | DOUBLE | Tinggi kendaraan (mm) |
| `tariff` | DOUBLE | Tarif (IDR) |
| `currency` | VARCHAR(10) | Mata uang |

### Query Berguna

```sql
-- Rekap harian per gate
SELECT gate_id, COUNT(*) AS kendaraan, SUM(tariff) AS pendapatan
FROM vehicle_records
WHERE recorded_at >= CURRENT_DATE
GROUP BY gate_id ORDER BY pendapatan DESC;

-- Total pendapatan hari ini
SELECT SUM(tariff) FROM vehicle_records WHERE recorded_at >= CURRENT_DATE;

-- Distribusi jenis kendaraan minggu ini
SELECT category, COUNT(*) AS jumlah, SUM(tariff) AS total
FROM vehicle_records
WHERE recorded_at >= CURRENT_DATE - INTERVAL '7 days'
GROUP BY category ORDER BY total DESC;

-- Export ke CSV
\COPY vehicle_records TO '/tmp/export.csv' CSV HEADER;
```

---

## Training Model Kustom

Lihat panduan lengkap: **[docs/training_guide.md](docs/training_guide.md)**

```bash
# 1. Rekam dataset
python3 scripts/smartport_mode.py train --output-dir ~/datasets/sesi1

# 2. Training YOLOv8 setelah dataset siap
yolo task=detect mode=train model=yolov8n.pt \
  data=~/datasets/custom_yolo/data.yaml \
  classes=2 3 5 7 epochs=100 imgsz=640 batch=16 device=0

# 3. Deploy model baru
cp runs/detect/train/weights/best.pt models/yolov8_smartport.pt
```

---

## Struktur Folder

```
SmartPortCam/
├── config/
│   ├── smartport.yaml        ← parameter ROS semua node
│   └── gates.json            ← konfigurasi gate (dikelola otomatis)
├── docs/
│   ├── running_system.md     ← cara jalankan per-node (manual)
│   └── training_guide.md     ← panduan training model
├── launch/
│   ├── smartport.launch.py         ← launch multi-gate penuh
│   └── rosbridge_secure.launch.py  ← rosbridge + topic allowlist
├── models/
│   └── yolov8_smartport.pt   ← model weights (tidak di-commit)
├── scripts/
│   └── smartport_mode.py     ← CLI: deploy / single / train
├── src/
│   ├── yolov8_detector/      ← Node: deteksi kendaraan (YOLOv8)
│   ├── dimension_estimator/  ← Node: estimasi dimensi 3D
│   ├── tariff_calculator/    ← Node: kalkulasi tarif otomatis
│   ├── dimension_gui/        ← Node: dashboard Tkinter lokal
│   ├── data_logger/          ← Node: simpan ke PostgreSQL (+ gate_id)
│   ├── gate_manager/         ← Node: lifecycle semua gate
│   │   ├── gate_config.py    ←   validasi & load/save gates.json
│   │   ├── gate_process.py   ←   spawn & monitor subprocess per gate
│   │   └── gate_manager.py   ←   ROS 2 node + services
│   ├── camera_manager/       ← Node: deteksi kamera RealSense USB
│   └── vehicle_dimension_msgs/  ← Custom ROS 2 messages & services
│       ├── msg/  (GateStatus, CameraDevice, CameraDeviceArray, …)
│       └── srv/  (AddGate, RemoveGate, SetGateActive, ListCameras, ListGates)
├── web_dashboard/            ← Dashboard React (Vite + TypeScript)
│   ├── public/
│   │   └── demo-gate.jpg     ←   foto gate untuk demo (taruh sendiri, tidak di-commit)
│   ├── src/
│   │   ├── demo/
│   │   │   └── demoData.ts   ←   simulasi data dummy (skenario kendaraan, histori awal)
│   │   ├── types.ts          ←   shared TypeScript interfaces
│   │   └── App.tsx           ←   semua halaman: Live View, Dashboard, Settings, Log
│   ├── .env.example          ←   template env production
│   ├── .env.demo             ←   env demo (VITE_DEMO_MODE=true)
│   └── README.md             ←   dokumentasi lengkap web dashboard
├── .env.example              ← template environment variables
├── .gitignore
├── docker-compose.yml        ← PostgreSQL 15 + Adminer
├── install.sh                ← setup script mesin baru
└── README.md
```

---

## Spesifikasi Hardware

### Minimum (fungsional, latency ~200ms/kendaraan)

| Komponen | Spek |
|---|---|
| CPU | Intel Core i5 Gen 11 / AMD Ryzen 5 5600 |
| RAM | 16 GB DDR4 |
| GPU | NVIDIA GTX 1660 Super 4 GB |
| Storage | 256 GB SSD |
| Kamera | Intel RealSense D435 / D435i |
| OS | Ubuntu 22.04 LTS |

### Rekomendasi Production (latency <50ms)

| Komponen | Spek |
|---|---|
| CPU | Intel Core i7-12700 / AMD Ryzen 7 5800X |
| RAM | 32 GB DDR4 |
| GPU | NVIDIA RTX 3060 12 GB (TensorRT support) |
| Storage | 512 GB NVMe SSD |
| Kamera | Intel RealSense D455 (jangkauan lebih jauh) |
| Network | Gigabit LAN |

### Estimasi Kapasitas per Server

| Spek GPU | Maks Gate Stabil | VRAM/Gate | Catatan |
|---|---|---|---|
| RTX 3060 12 GB | 4–6 gate | ~1.5 GB | Cukup untuk pelabuhan kecil–menengah |
| RTX 4080 16 GB | 8–10 gate | ~1.5 GB | Pelabuhan menengah–besar |
| Jetson AGX Orin | 1 gate | — | Embedded per gate, data ke server pusat |

> **Bottleneck:** GPU VRAM (YOLOv8 ~1.5 GB/instance) dan USB bandwidth (RealSense ~350 MB/s/kamera). Gunakan USB controller terpisah untuk banyak kamera.

---

## Security

| Layer | Mekanisme |
|---|---|
| **Input validation** | `gate_id` dan `camera_serial` divalidasi regex sebelum diproses — cegah path traversal & injection |
| **Subprocess** | `shell=False` pada semua `subprocess.Popen` — tidak ada shell injection |
| **ROS namespace** | Tiap gate terisolasi di namespace-nya sendiri (`/gate_1/`, `/gate_2/`) |
| **rosbridge allowlist** | `rosbridge_secure.launch.py` — hanya topik SmartPortCam yang bisa diakses dari browser |
| **DB credentials** | Dari `.env`, tidak pernah hardcoded di kode |
| **Config file** | Atomic write (write-then-rename) di `save_gates()` — cegah config corrupt |
| **Web read-only** | Dashboard hanya bisa subscribe topik + call service yang terdaftar; tidak bisa publish ke topik arbitrary |

---

## Troubleshooting

### Gate tidak muncul di dashboard

```bash
# Cek gate_manager berjalan
ros2 node list | grep gate_manager

# Cek status semua gate
ros2 topic echo /smartport/gates/status --once

# Lihat log gate_manager
ros2 node info /gate_manager
```

### Kamera RealSense tidak terdeteksi

```bash
# Cek USB
lsusb | grep Intel

# Cek serial number
rs-enumerate-devices | grep "Serial Number"

# Cek camera_manager
ros2 topic echo /smartport/cameras --once
```

### Data logger gagal konek ke PostgreSQL

```bash
# Cek container berjalan
docker compose ps

# Restart
docker compose restart postgres

# Test koneksi manual
psql -h localhost -U smartport -d smartportcam
```

### Video stream tidak muncul di dashboard

1. Pastikan `--with-rosbridge` diaktifkan atau `web_video_server` berjalan terpisah
2. Cek URL base video di Pengaturan dashboard (default: `http://localhost:8080`)
3. Pastikan parameter `publish_annotated_image: true` di `config/smartport.yaml`

### YOLOv8 lambat (inferensi >100ms)

```bash
# Cek GPU terdeteksi
python3 -c "import torch; print(torch.cuda.is_available(), torch.cuda.get_device_name())"

# Export ke TensorRT untuk performa maksimal
yolo export model=models/yolov8_smartport.pt format=engine device=0
# Set engine_path di config/smartport.yaml
```

### colcon build gagal

```bash
# Install semua ROS dependencies
rosdep install --from-paths src --ignore-src -r -y

# Build ulang bersih
rm -rf build install log
colcon build --symlink-install
```
