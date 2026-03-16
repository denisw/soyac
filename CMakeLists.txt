cmake_minimum_required(VERSION 3.31)
cmake_policy(VERSION 3.31)

project(Soya VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(BISON 3.6 REQUIRED)
find_package(BDWgc REQUIRED)
find_package(Boost CONFIG COMPONENTS filesystem program_options regex REQUIRED)
find_package(LLVM 19.1 CONFIG REQUIRED)

add_subdirectory(runtime)
add_subdirectory(src)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()
