#!/bin/bash

# Debug build script for RK3588 RGA Freeze application

set -e

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building RK3588 RGA Freeze application in Debug mode...${NC}"

# Call main build script with Debug parameter
./build.sh Debug

echo -e "${YELLOW}Debug build completed!${NC}"
echo -e "${YELLOW}You can now debug the application with gdb:${NC}"
echo -e "${GREEN}  gdb ./build/rk3588_rga_freeze${NC}"