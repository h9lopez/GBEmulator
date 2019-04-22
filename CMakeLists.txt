cmake_minimum_required (VERSION 3.0)
set (CMAKE_CXX_STANDARD 14)

project(GBEmulator)

SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost COMPONENTS log REQUIRED)

# Set up the core library itself for use by both the main exec and the testing project
file(GLOB CPP_SRC "GBEmulator/display/*.cpp")
file(GLOB CPP_SRC "GBEmulator/*.cpp")
include_directories(GBEmulator)
add_library(gbemucore ${CPP_SRC})
#target_compile_definitions(gbemucore PRIVATE BOOST_LOG_DYN_LINK)
#target_link_libraries(gbemucore boost_log pthread)


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
    ${Boost_LIBRARIES}
    )

# TODO: Add workaround that has each GTest as its own test within ctest
add_test(AllTests gbemutest)

# Create the actual core runnable
add_executable(gbemu GBEmulator/primary_loop.cpp)
target_link_libraries(gbemu gbemucore
                            ${Boost_LIBRARIES})
#target_compile_definitions(gbemu PRIVATE BOOST_LOG_DYN_LINK)