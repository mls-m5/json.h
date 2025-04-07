# CMakeLists for json.h

# to use this in your library
# assuming you have the lib installed in "lib/json.h"
#
# add_subdirectory(lib/json.h)
# target_link_libraries(your-application-name PUBLIC json)

cmake_minimum_required(VERSION 3.23)
project(json.h)

add_library(json INTERFACE)

target_include_directories(json INTERFACE include/)

enable_testing()

# Test
add_executable(
    json_test
    tests/json_test.cpp
    tests/mls-unit-test/test_main.cpp
    )
target_include_directories(json_test PRIVATE include)
target_compile_features(json_test PRIVATE cxx_std_17)
add_test(NAME json_test COMMAND json_test)
