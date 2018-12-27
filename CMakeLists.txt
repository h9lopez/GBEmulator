cmake_minimum_required (VERSION 3.0)
set (CMAKE_CXX_STANDARD 14)

project(GBEmulator)

# Set up the core library itself for use by both the main exec and the testing project
file(GLOB CPP_SRC "GBEmulator/*.cpp")
include_directories(GBEmulator)
add_library(gbemucore ${CPP_SRC})


# Configure the testing target
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_CPP_SRC GBEmulator_Test/GBEmulator_Test.gen.cpp)
add_executable(gbemutest ${TEST_CPP_SRC})
target_include_directories(gbemutest PRIVATE GBEmulator_Test)
target_link_libraries(gbemutest 
    gbemucore
    ${GTEST_BOTH_LIBRARIES}
    )

# TODO: Add workaround that has each GTest as its own test within ctest
add_test(AllTests gbemutest)