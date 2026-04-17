#!/bin/bash
# build-arm64-ubuntu.sh — install dependencies and compile reNut on Ubuntu ARM64
#
# Run this script on an Ubuntu 22.04+ ARM64 (aarch64) machine.
# Usage: bash build-arm64-ubuntu.sh

set -eu

RENUT_REPO="https://github.com/etonedemid/reNut.git"
SDK_REPO="https://github.com/etonedemid/rexglue-sdk.git"
INSTALL_DIR="$HOME/reNut"
BUILD_PRESET="linux-arm64-relwithdebinfo"

# ── Colours ───────────────────────────────────────────────────────────────────
RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'; NC='\033[0m'
info()  { echo -e "${GREEN}[reNut]${NC} $*"; }
warn()  { echo -e "${YELLOW}[reNut]${NC} $*"; }
error() { echo -e "${RED}[reNut]${NC} $*" >&2; exit 1; }

# ── Sanity checks ─────────────────────────────────────────────────────────────
if [ "$(uname -m)" != "aarch64" ]; then
    error "This script must be run on an aarch64 machine. For cross-compilation from x86_64, use the linux-arm64-relwithdebinfo CMake preset directly with cmake/toolchain-aarch64-linux-gnu.cmake."
fi

# ── Install dependencies ───────────────────────────────────────────────────────
install_deps() {
    info "Installing build dependencies…"
    sudo apt-get update -qq
    sudo apt-get install -y \
        build-essential clang lld ninja-build cmake pkg-config python3 git \
        libgtk-3-dev libpango1.0-dev libharfbuzz-dev libfontconfig-dev \
        libfreetype-dev libcairo2-dev libgdk-pixbuf-2.0-dev libglib2.0-dev \
        libatspi2.0-dev libx11-dev libx11-xcb-dev libxcb1-dev libxext-dev libxrender-dev \
        libpipewire-0.3-dev vulkan-headers libvulkan-dev \
        libasound2-dev libpulse-dev libusb-1.0-0-dev libunwind-dev \
        libdbus-1-dev libsystemd-dev libudev-dev \
        libpixman-1-dev zlib1g-dev libpng-dev libjpeg-dev libtiff-dev \
        libbz2-dev libbrotli-dev libzstd-dev liblzma-dev \
        libpcre2-dev shared-mime-info
}

# Check if already installed (rough check)
if ! command -v ninja &>/dev/null || ! pkg-config --exists gtk+-3.0 2>/dev/null; then
    install_deps
else
    info "Dependencies appear to be installed — skipping."
fi

# ── Clone / update repos ───────────────────────────────────────────────────────
if [ ! -d "$INSTALL_DIR" ]; then
    info "Cloning reNut to $INSTALL_DIR…"
    git clone --recurse-submodules "$RENUT_REPO" "$INSTALL_DIR"
else
    info "Updating reNut…"
    git -C "$INSTALL_DIR" pull
fi

SDK_DIR="$INSTALL_DIR/../rexglue-sdk"
if [ ! -d "$SDK_DIR" ]; then
    info "Cloning rexglue-sdk…"
    git clone "$SDK_REPO" "$SDK_DIR"
else
    info "Updating rexglue-sdk…"
    git -C "$SDK_DIR" pull
fi

# ── Build ──────────────────────────────────────────────────────────────────────
cd "$INSTALL_DIR"

# The linux-arm64-relwithdebinfo preset uses the cross-compilation toolchain file.
# On a native aarch64 machine, override the toolchain to use the system clang directly.
info "Configuring (native aarch64 build)…"
cmake --preset linux-arm64-relwithdebinfo \
    -DCMAKE_TOOLCHAIN_FILE="" \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++

info "Building…"
cmake --build out/build/linux-arm64-relwithdebinfo --target renut -j"$(nproc)"

info "Build complete: $INSTALL_DIR/out/build/linux-arm64-relwithdebinfo/renut"
