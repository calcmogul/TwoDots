cmake_minimum_required(VERSION 3.21)

# Set default build type to release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(TwoDots)

include(FetchContent)

file(GLOB_RECURSE cpp_src src/*.cpp)
add_executable(TwoDots ${cpp_src})

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)
target_link_libraries(TwoDots PUBLIC SFML::Graphics)

if(NOT MSVC)
    target_compile_options(TwoDots PRIVATE -Wall -Wextra -pedantic)
endif()
set_property(TARGET TwoDots PROPERTY COMPILE_WARNING_AS_ERROR ON)

target_compile_features(TwoDots PUBLIC cxx_std_23)
target_include_directories(TwoDots PRIVATE src)
