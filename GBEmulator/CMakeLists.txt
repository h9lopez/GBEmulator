cmake_minimum_required (VERSION 3.0)
set (CMAKE_CXX_STANDARD 14)

project (GBEmulator)

include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB SOURCES "*.cpp")
file(GLOB SOURCES "*/*.cpp")

add_executable(GBEmulator ${SOURCES})