cmake_minimum_required (VERSION 3.0)
set (CMAKE_CXX_STANDARD 14)

project (GBEmulator)

include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB SOURCES "*.cpp")
file(GLOB SOURCES "GBEmulator/*.cpp")
file(GLOB SOURCES "GBEmulator/cpu/*/*.cpp")
file(GLOB SOURCES "GBEmulator/ram/*/*.cpp")
file(GLOB SOURCES "GBEmulator/registers/*/*.cpp")
file(GLOB SOURCES "GBEmulator/romloader/*/*.cpp")
file(GLOB SOURCES "GBEmulator/utils/*/*.cpp")
file(GLOB SOURCES "GBEmulator/display/sdl/*.cpp")
file(GLOB SOURCES "GBEmulator/display/displaycore/*.cpp")

add_executable(GBEmulator ${SOURCES})