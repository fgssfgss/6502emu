cmake_minimum_required(VERSION 3.3)
project(6502emu)
include_directories("include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp src/CPU.cpp src/Emulator.cpp src/Memory.cpp src/ROM.cpp)
add_executable(6502emu ${SOURCE_FILES})