#!/bin/bash

# Build script for RK3588 RGA Freeze application

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Build type (default: Release)
BUILD_TYPE=${1:-Release}

echo -e "${GREEN}Building RK3588 RGA Freeze application...${NC}"
echo -e "${YELLOW}Build type: ${BUILD_TYPE}${NC}"

# Check if OpenCV is built
OPENCV_DIR="${PWD}/3rdparty/opencv-4.9.0/build_linux/install"
if [ ! -d "$OPENCV_DIR" ]; then
    echo -e "${RED}Error: OpenCV not found at $OPENCV_DIR${NC}"
    echo -e "${YELLOW}Please run ./scripts/opencv_build.sh first${NC}"
    exit 1
fi

# Check if RGA library exists
RGA_LIB="${PWD}/rga/libs/librga.so"
if [ ! -f "$RGA_LIB" ]; then
    echo -e "${RED}Error: RGA library not found at $RGA_LIB${NC}"
    exit 1
fi

# Create build directory
BUILD_DIR="build"
mkdir -p $BUILD_DIR

# Configure with CMake
echo -e "${GREEN}Configuring with CMake...${NC}"
cd $BUILD_DIR
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..

# Build
echo -e "${GREEN}Building...${NC}"
make -j$(nproc)

echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "${YELLOW}Executable: ${BUILD_DIR}/rk3588_rga_freeze${NC}"

# Copy RGA library to build directory for runtime
cp ../rga/libs/librga.so . 2>/dev/null || true

echo -e "${GREEN}Done!${NC}"