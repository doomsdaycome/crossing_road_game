cmake_minimum_required(VERSION 3.23)
project(startorch LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

if(WIN32)
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/library/SFML-3.1.0/lib/cmake/SFML")
endif()

find_package(SFML 3.1 REQUIRED COMPONENTS System Window Graphics Audio Network)

add_executable(crossing
    source/crossing.cpp
)

target_include_directories(crossing PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(crossing PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
    SFML::Network
)

if(WIN32)
    add_custom_command(TARGET crossing POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:crossing>
            $<TARGET_FILE_DIR:crossing>
        COMMAND_EXPAND_LISTS
    )
endif()
