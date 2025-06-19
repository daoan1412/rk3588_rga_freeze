cmake_minimum_required(VERSION 3.28)
project(rk3588_rga_freeze)

set(CMAKE_CXX_STANDARD 17)

file(COPY assets/images DESTINATION ${PROJECT_BINARY_DIR})

set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/3rdparty/opencv-4.9.0/build_linux/install/lib/cmake/opencv4")
find_package(OpenCV REQUIRED core imgproc video videoio highgui)


add_executable(rk3588_rga_freeze  main.cpp
        resize/src/resize_function.cpp)

target_include_directories(rk3588_rga_freeze PUBLIC rga/include resize/include)
target_link_directories(rk3588_rga_freeze PUBLIC rga/libs)
target_compile_definitions(rk3588_rga_freeze PRIVATE ENABLE_RGA)
target_link_libraries(rk3588_rga_freeze ${OpenCV_LIBS} rga ${CMAKE_THREAD_LIBS_INIT})
