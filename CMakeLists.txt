cmake_minimum_required(VERSION 3.16)
project(EpiKodi LANGUAGES CXX)

# Version du standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options de build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Dossiers include
include_directories(${CMAKE_SOURCE_DIR}/include)

# Fichiers sources
file(GLOB_RECURSE SOURCES src/*.cpp)

# Exécutable principal
add_executable(epikodi ${SOURCES})

# Tests (placeholder pour plus tard)
enable_testing()
