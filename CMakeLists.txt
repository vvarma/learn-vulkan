cmake_minimum_required(VERSION 3.25)
project(learn-vulkan CXX)

set(CMAKE_CXX_STANDARD 20)


include(cmake/dependencies.cmake)
include(cmake/functions.cmake)
message("env variable for vulkan $ENV{VULKAN_SDK} and $ENV{VK_LAYER_PATH}")

add_subdirectory(shaders)
add_subdirectory(src)
