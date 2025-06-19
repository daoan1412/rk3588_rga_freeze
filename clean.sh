#!/bin/bash

# Clean script for RK3588 RGA Freeze application

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Cleaning build artifacts...${NC}"

# Remove build directory
if [ -d "build" ]; then
    echo -e "${GREEN}Removing build directory...${NC}"
    rm -rf build
fi

# Remove CMake cache files in root (if any)
rm -f CMakeCache.txt
rm -rf CMakeFiles

echo -e "${GREEN}Clean completed!${NC}"