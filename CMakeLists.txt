cmake_minimum_required(VERSION 3.23)
project(startorch LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(crossing
  sources/crossing.cpp
)

target_include_directories(crossing PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

add_executable(crossing_test
  tests/test_animal.cpp
  tests/test_people.cpp
)

target_include_directories(crossing_test PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

target_link_libraries(crossing_test PRIVATE
  GTest::gtest_main
)

enable_testing()

include(GoogleTest)
gtest_discover_tests(crossing_test)
