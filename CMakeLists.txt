cmake_minimum_required(VERSION 3.23)
project(startorch LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(SFML_STATIC_LIBRARIES TRUE)                                             
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries/SFML-3.1.0/lib/cmake/SFML")

find_package(SFML 3.1 REQUIRED COMPONENTS System Window Graphics Audio Network)

add_executable(crossing
    sources/crossing.cpp
)

target_compile_definitions(crossing PRIVATE SFML_STATIC)                 

target_include_directories(crossing PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

target_link_libraries(crossing PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
    SFML::Network
)
