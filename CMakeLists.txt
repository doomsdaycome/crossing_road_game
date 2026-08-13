cmake_minimum_required(VERSION 3.23)
project(startorch LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

if(WIN32)
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/library/SFML-3.1.0/lib/cmake/SFML")
endif()

find_package(SFML 3.1 REQUIRED COMPONENTS System Window Graphics Audio Network)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS source/*.cpp)

# add_executable(main
#     source/main.cpp
#     source/core/CGAME.cpp
#     source/states/MenuState.cpp
#     source/states/PlayingState.cpp
# )

add_executable(main ${SOURCE_FILES})

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/asset"
    "$<TARGET_FILE_DIR:main>/asset"
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/data"
    "$<TARGET_FILE_DIR:main>/data"
)

target_include_directories(main PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(main PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
    SFML::Network
)

if(WIN32)
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:main>
            $<TARGET_FILE_DIR:main>
        COMMAND_EXPAND_LISTS
    )
endif()
