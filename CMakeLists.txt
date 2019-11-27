cmake_minimum_required(VERSION 2.8)

PROJECT(demo1)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(./src)
add_subdirectory(./src)

aux_source_directory(. DIR_SRCS)
add_executable(demo1 ${DIR_SRCS})

target_link_libraries(demo1 flight_base)
