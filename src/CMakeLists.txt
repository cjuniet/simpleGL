if(MSVC)
  add_definitions("/D_CRT_SECURE_NO_WARNINGS")
  set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
  add_definitions("-DGLM_FORCE_RADIANS")
  include_directories("${OPENGL_INCLUDE_DIR}")
endif()

set(SOURCES
  main.cpp
  Geometry.hpp Geometry.cpp
  Shader.hpp Shader.cpp
  Shape.hpp Shape.cpp
)

set(SOURCES ${SOURCES} "${CMAKE_SOURCE_DIR}/ext/glad/src/glad.c")

add_executable(simple ${SOURCES})

target_link_libraries(simple glfw ${GLFW_LIBRARIES})

add_custom_target(run COMMAND simple DEPENDS simple WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
