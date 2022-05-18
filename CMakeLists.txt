cmake_minimum_required(VERSION 3.10)
project(PIM_SIM)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)
include_directories(include/frontend)
include_directories(include/backend)
include_directories(include/tclap)
include_directories(lib/backend)

add_library(PimBackend lib/backend/Config.cpp
        lib/backend/Controller.cpp
        lib/backend/MemoryBlock.cpp
        lib/backend/RegisterFile.cpp
        lib/backend/MemoryTile.cpp
        lib/backend/MemoryChip.cpp
        lib/backend/MemoryComponent.cpp
        lib/backend/Tool.cpp
        lib/backend/System.cpp
        lib/backend/Memory_characteristics.cpp
        lib/backend/hTree.cpp
        )

add_executable(PIM_simulator main.cpp)

target_link_libraries(PIM_simulator PimBackend)
