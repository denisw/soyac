cmake_minimum_required(VERSION 3.17)
cmake_policy(VERSION 3.17)
include(FindPkgConfig)

project(Soyac VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

# Bison
if (EXISTS /usr/local/opt/bison/bin/bison)
    set(BISON_EXECUTABLE /usr/local/opt/bison/bin/bison)
endif()
find_package(BISON 3.6 REQUIRED)

# Boost
find_package(Boost COMPONENTS program_options regex REQUIRED)

# LLVM
set(LLVM_ROOT /usr/local/Cellar/llvm/10.0.0_3)
find_package(LLVM REQUIRED)

# SigC++
pkg_check_modules(SigCPP REQUIRED IMPORTED_TARGET sigc++-3.0)

add_subdirectory(runtime)
add_subdirectory(src)
