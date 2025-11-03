# SmartPortCam

## Menjalankan Deteksi YOLOv8 dengan Visualisasi dan Estimasi Dimensi

Gunakan launch file baru berikut agar node detektor dan estimator dimensi otomatis aktif. Opsi tambahan memungkinkan memulai driver RealSense dan menampilkan jendela OpenCV.

```bash
ros2 launch yolov8_detector yolov8_with_dimensions.launch.py \
  display_results:=true \
  start_realsense:=true \
  model_path:=/path/ke/model.pt
```

- `display_results` (`true`/`false`): tampilkan jendela OpenCV berisi gambar dengan bounding box dan skor YOLOv8.
- `start_realsense` (`true`/`false`): jalankan node `realsense2_camera` dengan depth ter-align otomatis.
- `model_path`/`engine_path`: set jika model tidak berada di direktori workspace.
- `image_topic`: topik kamera RGB yang akan dipakai detektor (default `/camera/color/image_raw`).
- `log_detection_summary` (`true`/`false`): cetak jumlah deteksi pada log setiap frame.
- `publish_annotated_image` (`true`/`false`): terbitkan citra hasil anotasi pada `/vehicle/detections/image_raw`.
- `max_detections`: batasi jumlah bounding box yang dipublikasikan per frame (0 = tanpa batas, default).
- `bbox_topic`: (dimension_estimator) topik bounding box yang akan dikonsumsi (default `/vehicle/bounding_boxes`).
- `depth_image_topic` / `depth_camera_info_topic`: (dimension_estimator) topik depth aligned dan camera info; sesuaikan jika namespace kamera berbeda, misalnya `/camera/camera/aligned_depth_to_color/image_raw`.
- `dimension_gui.depth_camera_info_topic`: topik kamera info yang dipakai dashboard untuk status kamera.

Jika RealSense dijalankan terpisah, set `start_realsense:=false`. Node `dimension_estimator` akan menerbitkan ukuran kendaraan dalam milimeter pada topik `/vehicle/dimension_info`.

Dashboard GUI menampilkan panjang/lebar/tinggi dalam sentimeter (cm).

### Visualisasi hasil YOLO di RViz atau rqt_image_view

Dengan parameter `publish_annotated_image:=true`, kamu bisa menampilkan citra yang sudah digambar bounding box lewat:

```bash
ros2 run rqt_image_view rqt_image_view --ros-args -r /image:=/vehicle/detections/image_raw
```

Atau tambahkan display `Image` di RViz dan pilih topik yang sama.



Cara menjalankannya

* terminal 1 (menjalankan kameranya)
 - source ~/smartport_ws/install/setup.bash
 - ros2 launch realsense2_camera rs_launch.py   align_depth.enable:=true   enable_sync:=true   enable_infra1:=false   enable_infra2:=false   pointcloud.enable:=false

* terminal 2 (menjalankan yolonya)
 - source ~/smartport_ws/install/setup.bash
 - ros2 run yolov8_detector yolov8_detector --ros-args   -p model_path:=/home/ais/smartport_ws/yolov8n.pt   -p image_topic:=/camera/camera/color/image_raw   -p class_ids:="[2,3,5,7]"   -p max_detections:=1   -p display_results:=true   -p publish_annotated_image:=true

* terminal 3 (menjalankan perhitungan ukuran)
 - source ~/smartport_ws/install/setup.bash
 - ros2 run dimension_estimator dimension_estimator --ros-args   -p bbox_topic:=/vehicle/bounding_boxes   -p depth_image_topic:=/camera/camera/aligned_depth_to_color/image_raw   -p depth_camera_info_topic:=/camera/camera/aligned_depth_to_color/camera_info   -p min_valid_points:=200

* temrinal 4 (menjalankan GUInya)
 - source ~/smartport_ws/install/setup.bash
 - ros2 run dimension_gui dimension_gui --ros-args   -p depth_camera_info_topic:=/camera/camera/aligned_depth_to_color/camera_info



kurang menampilkan harga saja dan kalkulasi harga nya per meter