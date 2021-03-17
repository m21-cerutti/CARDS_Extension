project(rv_calibration)

cmake_minimum_required(VERSION 2.8)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV 4.1 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(SRCS camera_calibration.cpp)

add_executable(camera_calibration ${SRCS})

target_link_libraries(camera_calibration ${OpenCV_LIBS})

if(APPLE)
  target_link_libraries(camera_calibration "-framework Foundation" "-framework Cocoa")
endif()
