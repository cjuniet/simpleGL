cmake_minimum_required(VERSION 2.8.12)
enable_testing()

project(SimpleGL)

# OpenGL
find_package(OpenGL REQUIRED)
# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
add_subdirectory("${CMAKE_SOURCE_DIR}/ext/glfw")
# GoogleTest
add_subdirectory("${CMAKE_SOURCE_DIR}/ext/gtest")

# Executable and UTests
include_directories("${CMAKE_SOURCE_DIR}/ext/glad/include")
include_directories("${CMAKE_SOURCE_DIR}/ext/glfw/include")
include_directories("${CMAKE_SOURCE_DIR}/ext/glm")

add_subdirectory(src)
add_subdirectory(test)
