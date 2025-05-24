cmake_minimum_required(VERSION 3.21)

# Set default build type to release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# Generate compile_commands.json by default
if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS "YES" CACHE STRING "" FORCE)
endif()

project(TwoDots)

include(FetchContent)

fetchcontent_declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
fetchcontent_makeavailable(SFML)

file(GLOB_RECURSE cpp_src src/*.cpp)
set_property(SOURCE ${cpp_src} PROPERTY COMPILE_FLAGS "-Wno-unused")
add_executable(TwoDots ${cpp_src})

target_link_libraries(TwoDots PUBLIC SFML::Graphics)

target_compile_options(TwoDots PRIVATE -Wall -Wextra -Werror -pedantic)
target_compile_features(TwoDots PUBLIC cxx_std_23)
target_include_directories(TwoDots PRIVATE src)
