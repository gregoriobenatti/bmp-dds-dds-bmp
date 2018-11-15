cmake_minimum_required(VERSION 3.12)
project(bmp_dds_dds_bmp)

set(CMAKE_CXX_STANDARD 14)

add_executable(bmp_dds_dds_bmp src/main.cpp src/BMPFile.cpp src/BMPFile.h src/DDSFile.cpp src/DDSFile.h src/Converter.cpp src/Converter.h)
