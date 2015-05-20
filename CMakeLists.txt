cmake_minimum_required (VERSION 2.4.7)

project (soyac)

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeScripts")
include (DefineDependenciesAndFlags)

#set(${soyac_BINARY_DIR} build)

add_subdirectory (src build)
add_subdirectory (runtime)
add_subdirectory (test EXCLUDE_FROM_ALL)
