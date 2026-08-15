cmake_minimum_required(VERSION 3.17)

project(GBEmulator)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# If compiling with Clang on a Linux system, linking libc++ might require specific flags
# or cause issues if the dependencies (like Boost/SDL) were built with libstdc++.
# In vcpkg, packages are built with the default system library (libstdc++ on Linux).
# Therefore, forcing libc++ will lead to linker errors. Let's not force it.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

find_package(Boost REQUIRED COMPONENTS log thread program_options)
find_package(SDL2 CONFIG REQUIRED)

# Set up the core library itself for use by both the main exec and the testing project
set(CPP_SRC
    GBEmulator/cpu/CPUCore.cpp
    GBEmulator/cpu/opcodes/OpcodeResultContext.cpp
    GBEmulator/cpu/opcodes/gb_cpucore_cb_opcodes.cpp
    GBEmulator/cpu/opcodes/gb_cpucore_opcode_utils.cpp
    GBEmulator/cpu/opcodes/gb_cpucore_opcodes.cpp
    GBEmulator/display/gb_screen_api.cpp
    GBEmulator/display/gb_sdl_screen.cpp
    GBEmulator/ram/gb_ram.cpp
    GBEmulator/registers/gb_regs.cpp
    GBEmulator/romloader/gb_rom.cpp
)

add_library(gbemucore STATIC ${CPP_SRC})
target_include_directories(gbemucore PUBLIC
    GBEmulator
    GBEmulator/cpu/opcodes
    GBEmulator/cpu
    GBEmulator/display
    GBEmulator/ram
    GBEmulator/registers
    GBEmulator/romloader
    GBEmulator/utils
)

target_link_libraries(gbemucore PUBLIC Boost::log Boost::thread Boost::program_options SDL2::SDL2)

# Create the actual core runnable
add_executable(gbemu GBEmulator/primary_loop.cpp)
target_link_libraries(gbemu PRIVATE gbemucore)
