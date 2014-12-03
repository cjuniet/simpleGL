cmake_minimum_required(VERSION 2.8.12)
enable_testing()

project(SimpleGL)

if(NOT MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif()

# Source Files
set(SOURCES src/main.cpp
    src/ShaderProgram.hpp src/ShaderProgram.cpp
)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
add_subdirectory("${CMAKE_SOURCE_DIR}/ext/glfw")
include_directories("${CMAKE_SOURCE_DIR}/ext/glfw/include")

# GLAD
include_directories("${CMAKE_SOURCE_DIR}/ext/glad/include")
set(SOURCES ${SOURCES} "${CMAKE_SOURCE_DIR}/ext/glad/src/glad.c")

# GLM
include_directories("${CMAKE_SOURCE_DIR}/ext/glm")
add_definitions("-DGLM_FORCE_RADIANS")

add_executable(simple ${SOURCES})
target_link_libraries(simple glfw ${GLFW_LIBRARIES})
