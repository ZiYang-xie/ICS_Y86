cmake_minimum_required(VERSION 3.17)
project(test)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(Google_Tests_run test_util.h test_util.cpp main_test.cpp ../src/Controller.cpp ../src/Controller.h
        ../src/Device.cpp ../src/Device.h ../src/instr.h ../src/pipeline_registers.cpp ../src/pipeline_registers.h ../src/util.cpp ../src/util.h)
target_link_libraries(Google_Tests_run gtest gtest_main)