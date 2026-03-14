#!/bin/bash
BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Dependencies
sudo apt update
sudo apt install -y \
git \
build-essential \
libglib2.0-dev \
libfdt-dev \
libpixman-1-dev \
zlib1g-dev \
ninja-build \
bear

sudo apt remove -y \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi
sudo apt autopurge -y

git clone https://github.com/kmhallen/gcc-arm-none-eabi.git "${BASE_DIR}/arm-gcc-install"
chmod +x "${BASE_DIR}/arm-gcc-install/gcc-arm-none-eabi-install.bash"
"${BASE_DIR}/arm-gcc-install/gcc-arm-none-eabi-install.bash"