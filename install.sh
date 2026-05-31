#!/usr/bin/env bash
# =============================================================================
# SmartPortCam — Install Script
# Jalankan sekali di mesin baru Ubuntu 22.04 setelah git clone.
# Usage: bash install.sh
# =============================================================================

set -euo pipefail

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'; NC='\033[0m'
info()  { echo -e "${GREEN}[INFO]${NC} $*"; }
warn()  { echo -e "${YELLOW}[WARN]${NC} $*"; }
error() { echo -e "${RED}[ERROR]${NC} $*"; exit 1; }

WORKSPACE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$WORKSPACE_DIR"

# ---------------------------------------------------------------------------
# 1. Cek OS
# ---------------------------------------------------------------------------
info "Memeriksa OS…"
if [[ "$(uname -s)" != "Linux" ]]; then
  error "Script ini hanya untuk Linux (Ubuntu 22.04)."
fi
UBUNTU_VER=$(lsb_release -rs 2>/dev/null || echo "unknown")
if [[ "$UBUNTU_VER" != "22.04" ]]; then
  warn "Ubuntu $UBUNTU_VER terdeteksi. Script diuji di Ubuntu 22.04 — mungkin ada perbedaan."
fi

# ---------------------------------------------------------------------------
# 2. Cek ROS 2 Humble
# ---------------------------------------------------------------------------
info "Memeriksa ROS 2 Humble…"
if ! command -v ros2 &>/dev/null; then
  error "ROS 2 tidak ditemukan. Install dulu:\n  https://docs.ros.org/en/humble/Installation.html"
fi
ROS_DISTRO="${ROS_DISTRO:-}"
if [[ "$ROS_DISTRO" != "humble" ]]; then
  warn "ROS_DISTRO='$ROS_DISTRO'. Script diuji dengan ROS 2 Humble."
fi
info "ROS 2 OK: $ROS_DISTRO"

# ---------------------------------------------------------------------------
# 3. Install Python dependencies
# ---------------------------------------------------------------------------
info "Menginstal Python dependencies…"
pip3 install --upgrade pip --quiet
pip3 install \
  ultralytics \
  open3d \
  pyrealsense2 \
  psycopg2-binary \
  opencv-python \
  --quiet
info "Python deps OK."

# ---------------------------------------------------------------------------
# 4. Install ROS 2 tambahan (rosbridge, web_video_server)
# ---------------------------------------------------------------------------
info "Menginstal paket ROS 2 tambahan…"
sudo apt-get update -qq
sudo apt-get install -y -qq \
  ros-humble-rosbridge-server \
  ros-humble-web-video-server \
  ros-humble-realsense2-camera \
  ros-humble-cv-bridge \
  python3-colcon-common-extensions
info "Paket ROS 2 tambahan OK."

# ---------------------------------------------------------------------------
# 5. Install Docker (untuk PostgreSQL)
# ---------------------------------------------------------------------------
info "Memeriksa Docker…"
if ! command -v docker &>/dev/null; then
  info "Docker tidak ditemukan — menginstal…"
  curl -fsSL https://get.docker.com | sh
  sudo usermod -aG docker "$USER"
  warn "Docker terinstal. Logout & login kembali agar grup 'docker' aktif, lalu jalankan install.sh lagi."
  exit 0
fi
info "Docker OK: $(docker --version)"

if ! command -v docker &>/dev/null || ! docker compose version &>/dev/null 2>&1; then
  # Coba plugin lama
  if ! command -v docker-compose &>/dev/null; then
    info "Menginstal Docker Compose plugin…"
    sudo apt-get install -y -qq docker-compose-plugin
  fi
fi

# ---------------------------------------------------------------------------
# 6. Setup .env
# ---------------------------------------------------------------------------
info "Memeriksa file .env…"
if [[ ! -f ".env" ]]; then
  cp .env.example .env
  info "File .env dibuat dari .env.example. Edit nilainya sesuai kebutuhan."
else
  info ".env sudah ada — tidak ditimpa."
fi

if [[ ! -f "web_dashboard/.env" ]]; then
  cp web_dashboard/.env.example web_dashboard/.env
  info "File web_dashboard/.env dibuat."
fi

# ---------------------------------------------------------------------------
# 7. Jalankan PostgreSQL via Docker Compose
# ---------------------------------------------------------------------------
info "Menjalankan PostgreSQL…"
if docker compose version &>/dev/null 2>&1; then
  docker compose up -d
else
  docker-compose up -d
fi
info "PostgreSQL berjalan di port 5432. Adminer: http://localhost:8081"

# ---------------------------------------------------------------------------
# 8. Install Node.js / npm
# ---------------------------------------------------------------------------
info "Memeriksa Node.js…"
if ! command -v node &>/dev/null; then
  info "Node.js tidak ditemukan — menginstal via NodeSource (LTS)…"
  curl -fsSL https://deb.nodesource.com/setup_lts.x | sudo -E bash -
  sudo apt-get install -y -qq nodejs
fi
info "Node.js OK: $(node --version)"

# ---------------------------------------------------------------------------
# 9. Install web dashboard dependencies
# ---------------------------------------------------------------------------
info "Menginstal web dashboard dependencies…"
cd "$WORKSPACE_DIR/web_dashboard"
npm install --silent
cd "$WORKSPACE_DIR"
info "Web dashboard deps OK."

# ---------------------------------------------------------------------------
# 10. Build ROS 2 workspace
# ---------------------------------------------------------------------------
info "Membangun ROS 2 workspace (colcon build)…"
# Pastikan setup.bash di-source dulu jika ada
if [[ -f "/opt/ros/humble/setup.bash" ]]; then
  # shellcheck disable=SC1091
  source /opt/ros/humble/setup.bash
fi
colcon build --symlink-install
info "Build selesai."

# ---------------------------------------------------------------------------
# Selesai
# ---------------------------------------------------------------------------
echo ""
echo -e "${GREEN}============================================================${NC}"
echo -e "${GREEN} SmartPortCam berhasil diinstal!${NC}"
echo -e "${GREEN}============================================================${NC}"
echo ""
echo "Langkah selanjutnya:"
echo "  1. Source workspace : source install/setup.bash"
echo "  2. Jalankan sistem  : python3 scripts/smartport_mode.py deploy --with-db"
echo "  3. Buka dashboard   : cd web_dashboard && npm run dev"
echo "  4. Kelola database  : http://localhost:8081 (Adminer)"
echo ""
echo "Lihat README.md untuk panduan lengkap."
