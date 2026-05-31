# Cara Menjalankan Sistem (Manual — tanpa script)

> Untuk cara tercepat, gunakan: `python3 scripts/smartport_mode.py deploy --with-db`
> Panduan di bawah ini untuk debugging atau menjalankan node secara individual.

## Mode Otomatis (Direkomendasikan)

```bash
source ~/smartport_ws/install/setup.bash
python3 scripts/smartport_mode.py deploy \
  --model-path models/yolov8_smartport.pt \
  --with-db \
  --publish-annotated-image
```

## Mode Manual (per terminal)

### Terminal 1 — Kamera RealSense
```bash
source ~/smartport_ws/install/setup.bash
ros2 launch realsense2_camera rs_launch.py \
  align_depth.enable:=true \
  enable_sync:=true \
  enable_infra1:=false \
  enable_infra2:=false \
  pointcloud.enable:=false
```

### Terminal 2 — YOLOv8 Detector
```bash
source ~/smartport_ws/install/setup.bash
ros2 run yolov8_detector yolov8_detector --ros-args \
  --params-file config/smartport.yaml \
  -p model_path:=models/yolov8_smartport.pt
```

### Terminal 3 — Dimension Estimator
```bash
source ~/smartport_ws/install/setup.bash
ros2 run dimension_estimator dimension_estimator --ros-args \
  --params-file config/smartport.yaml
```

### Terminal 4 — Tariff Calculator
```bash
source ~/smartport_ws/install/setup.bash
ros2 run tariff_calculator tariff_calculator --ros-args \
  --params-file config/smartport.yaml
```

### Terminal 5 — Data Logger (PostgreSQL)
```bash
source ~/smartport_ws/install/setup.bash
ros2 run data_logger data_logger --ros-args \
  --params-file config/smartport.yaml
```

### Terminal 6 — Dimension GUI (Tkinter)
```bash
source ~/smartport_ws/install/setup.bash
ros2 run dimension_gui dimension_gui --ros-args \
  --params-file config/smartport.yaml
```

### Terminal 7 — rosbridge + web_video_server
```bash
source ~/smartport_ws/install/setup.bash
ros2 launch rosbridge_server rosbridge_websocket_launch.xml &
sleep 3
ros2 run web_video_server web_video_server --ros-args \
  -r image:=/vehicle/detections/image_raw
```

### Terminal 8 — Web Dashboard
```bash
cd web_dashboard
npm run dev
# Buka browser: http://localhost:5173
```
