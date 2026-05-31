# Panduan Training Model Kustom

## Opsi 1 — Rekam Dataset Sendiri dengan RealSense

### Langkah 1: Rekam data (2 terminal)

**Terminal 1 — Kamera:**
```bash
source ~/smartport_ws/install/setup.bash
ros2 launch realsense2_camera rs_launch.py \
  align_depth.enable:=true \
  enable_sync:=true \
  enable_infra1:=false \
  enable_infra2:=false \
  pointcloud.enable:=false
```

**Terminal 2 — Rekam rosbag:**
```bash
source ~/smartport_ws/install/setup.bash
ros2 bag record -o ~/smartport_ws/datasets/sesi1 \
  /camera/camera/color/image_raw \
  /camera/camera/aligned_depth_to_color/image_raw \
  /camera/camera/aligned_depth_to_color/camera_info
```

### Langkah 2: Ekstrak frame dari rosbag

**Terminal 3 — Putar rosbag:**
```bash
ros2 bag play ~/smartport_ws/datasets/sesi1/sesi1_0.db3 --clock --loop
```

**Terminal 4 — Simpan frame RGB:**
```bash
mkdir -p ~/smartport_ws/datasets/sesi1_frames/rgb
ros2 run image_view image_saver --ros-args \
  -r image:=/camera/camera/color/image_raw \
  -p save_all_image:=true \
  -p filename_format:=/home/ais/smartport_ws/datasets/sesi1_frames/rgb/frame%06i.jpg
```

### Langkah 3: Auto-label & Training

```bash
# Auto-label pakai model umum
yolo task=detect mode=predict model=yolov8n.pt \
  source=~/smartport_ws/datasets/sesi1_frames/rgb \
  save_txt=True save_conf=True classes=2 3 5 7 \
  project=~/smartport_ws/datasets/auto_label name=session1

# Susun struktur custom_yolo/train dan custom_yolo/val, lalu training:
yolo task=detect mode=train model=yolov8n.pt \
  data=~/smartport_ws/datasets/custom_yolo/data.yaml \
  classes=2 3 5 7 \
  epochs=100 imgsz=640 batch=16 device=0 \
  project=~/smartport_ws/datasets/train_runs \
  name=smartport_vehicle_v1

# Salin hasil terbaik ke models/
cp ~/smartport_ws/datasets/train_runs/smartport_vehicle_v1/weights/best.pt \
   ~/smartport_ws/models/yolov8_smartport.pt
```

---

## Opsi 2 — Pakai Dataset Roboflow

1. Buka [Roboflow Universe](https://universe.roboflow.com/) → cari dataset kendaraan
2. Download format **YOLOv8 PyTorch**, ukuran 640

```bash
mkdir -p ~/smartport_ws/datasets/roboflow_vehicle
cd ~/smartport_ws/datasets/roboflow_vehicle
curl -L "https://universe.roboflow.com/.../download?api_key=API_KEY" -o dataset.zip
unzip dataset.zip
```

3. Edit `data.yaml`:
   - `path: /home/<user>/smartport_ws/datasets/roboflow_vehicle`
   - `names: [mobil, sepeda_motor, bus, truk]`

4. Training:
```bash
yolo task=detect mode=train model=yolov8n.pt \
  data=~/smartport_ws/datasets/roboflow_vehicle/data.yaml \
  classes=2 3 5 7 epochs=100 imgsz=640 batch=16 device=0 \
  project=~/smartport_ws/datasets/train_runs name=smartport_vehicle_v1
```

5. Deploy:
```bash
cp .../weights/best.pt ~/smartport_ws/models/yolov8_smartport.pt
```

---

## Tips Training

| Parameter | Nilai | Keterangan |
|---|---|---|
| `epochs` | 100–300 | Lebih banyak = lebih akurat, lebih lama |
| `batch` | 16 | Turunkan ke 8 jika VRAM tidak cukup |
| `imgsz` | 640 | Standar YOLOv8, cukup untuk kendaraan |
| `device` | 0 | GPU pertama; `cpu` untuk tanpa GPU |
| `conf` | 0.70 | Threshold deteksi di `config/smartport.yaml` |
