#!/usr/bin/env bash
set -e

BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Detect distro
if [[ -f /etc/os-release ]]; then
    . /etc/os-release
else
    echo "Cannot detect OS (missing /etc/os-release)"
    exit 1
fi

install_deps_ubuntu() {
    sudo apt update
    sudo apt install -y \
        git \
        build-essential \
        libglib2.0-dev \
        libfdt-dev \
        libpixman-1-dev \
        zlib1g-dev \
        ninja-build \
        bear \
        python3 \
        python3-pip \
        python3-serial \
        python3-pandas \
        python3-seaborn

    sudo apt remove -y \
        gcc-arm-none-eabi \
        binutils-arm-none-eabi || true

    sudo apt autopurge -y
}

install_toolchain_ubuntu() {
    git clone https://github.com/kmhallen/gcc-arm-none-eabi.git "${BASE_DIR}/arm-gcc-install"
    chmod +x "${BASE_DIR}/arm-gcc-install/gcc-arm-none-eabi-install.bash"
    "${BASE_DIR}/arm-gcc-install/gcc-arm-none-eabi-install.bash"
    rm -rf "${BASE_DIR}/arm-gcc-install"
}

install_flasher_ubuntu() {
    sudo apt install -y \
        dfu-util
}

install_deps_arch() {
    sudo pacman -Sy --needed \
        git \
        base-devel \
        glib2 \
        dtc \
        pixman \
        zlib \
        ninja \
        bear \
        python \
        python-pip \
        python-pyserial \
        python-pandas \
        python-seaborn
}

install_toolchain_arch() {
    sudo pacman -S --needed \
        arm-none-eabi-gcc \
        arm-none-eabi-binutils \
        arm-none-eabi-gdb \
        arm-none-eabi-newlib
}

install_flasher_arch() {
    sudo pacman -S --needed \
        dfu-util
}

# Branch based on distro
case "$ID" in
    ubuntu|debian)
        install_deps_ubuntu
        install_toolchain_ubuntu
        install_flasher_ubuntu
        ;;
    arch)
        install_deps_arch
        install_toolchain_arch
        install_flasher_arch
        ;;
    *)
        echo "Unsupported distro: $ID"
        exit 1
        ;;
esac

# Run emulator setup
EMUPATH="${BASE_DIR}/emu"
EMUSETUP="${EMUPATH}/setup.sh"

chmod +x "$EMUSETUP"
"$EMUSETUP"
