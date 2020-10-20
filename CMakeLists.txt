cmake_minimum_required (VERSION 3.17)
set (CMAKE_CXX_STANDARD 20)

project(GBEmulator)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

FIND_PACKAGE(Boost 1.73.0 COMPONENTS log thread program_options REQUIRED)
FIND_PACKAGE(SDL2 REQUIRED)
SET(Boost_USE_STATIC_LIBS ON)


# Set up the core library itself for use by both the main exec and the testing project
file(GLOB CPP_SRC "GBEmulator/*/*/*.cpp" "GBEmulator/*/*.cpp" "GBEmulator/*.cpp")
include_directories(GBEmulator)
include_directories(GBEmulator/cpu/opcodes GBEmulator/cpu GBEmulator/display GBEmulator/ram
    GBEmulator/registers GBEmulator/romloader GBEmulator/utils)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})
add_library(gbemucore ${CPP_SRC})
set_property(TARGET gbemucore PROPERTY CXX_STANDARD 20)
target_link_libraries(gbemucore PRIVATE ${Boost_LIBRARIES})
target_include_directories(gbemucore PRIVATE ${Boost_INCLUDE_DIRS})
#target_compile_definitions(gbemucore PRIVATE BOOST_LOG_DYN_LINK)
#target_link_libraries(gbemucore boost_log pthread)


# # thehandler tool
# include_directories(thehandler)
# file(GLOB HANDLER_CPP_SRC "thehandler/*.cpp")
# add_executable(thehandler ${HANDLER_CPP_SRC})
# target_include_directories(thehandler PRIVATE thehandler)
# target_link_libraries(thehandler
#     gbemucore
#     ${Boost_LIBRARIES}
#     ${Boost_LOG_LIBRARY}
#     ${Boost_PROGRAM_OPTIONS_LIBRARY}
#     ${Boost_THREAD_LIBRARY}
# )

# Configure the testing target
# enable_testing()
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

# set(TEST_CPP_SRC GBEmulator_Test/GBEmulator_Test.gen.cpp)
# add_executable(gbemutest ${TEST_CPP_SRC})
# target_include_directories(gbemutest PRIVATE GBEmulator_Test)
# target_link_libraries(gbemutest 
#     gbemucore
#     ${GTEST_BOTH_LIBRARIES}
#     ${Boost_LIBRARIES}
#     ${Boost_LOG_LIBRARY}
#     ${Boost_PROGRAM_OPTIONS_LIBRARY}
#     ${Boost_THREAD_LIBRARY}
#     )

# TODO: Add workaround that has each GTest as its own test within ctest
# add_test(AllTests gbemutest)

# Create the actual core runnable
add_executable(gbemu GBEmulator/primary_loop.cpp)
target_include_directories(gbemu PRIVATE ${Boost_INCLUDE_DIRS})
#set_property(TARGET gbemu PROPERTY CXX_STANDARD 20)
target_link_libraries(gbemu PRIVATE 
                            gbemucore
                            ${Boost_LIBRARIES}
                            SDL2::SDL2main SDL2::SDL2-static)
#target_compile_definitions(gbemu PRIVATE BOOST_LOG_DYN_LINK)