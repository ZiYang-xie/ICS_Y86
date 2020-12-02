cmake_minimum_required(VERSION 3.17)
project(ICS_Y86)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./build)
set(CMAKE_CXX_STANDARD 17)
include_directories(src)
add_subdirectory(test)

add_executable(ICS_Y86
        src/main.cpp
        src/Device.cpp
        src/pipeline_registers.h src/util.h src/util.cpp src/instr.h src/Controller.cpp src/Controller.h src/pipeline_registers.cpp src/infoList.h)
