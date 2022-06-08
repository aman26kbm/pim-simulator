cmake_minimum_required(VERSION 3.10)
project(PIM_SIM)
set(CMAKE_CXX_STANDARD 11)
set(BUILD_SHARED_LIBS ON)

include_directories(include)
include_directories(include/frontend)
include_directories(include/backend)
include_directories(include/tclap)
include_directories(lib/backend)

message(WARN ${CMAKE_SOURCE_DIR})
file(GLOB_RECURSE LIB_SRCS ${CMAKE_SOURCE_DIR}/lib/backend/*.cpp)

add_library(PimBackend ${LIB_SRCS})

add_executable(PIM_simulator main.cpp)

target_link_libraries(PIM_simulator PimBackend)
