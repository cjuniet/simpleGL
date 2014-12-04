if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
  add_definitions("-DGLM_FORCE_RADIANS")
endif()

set(SOURCES
  main.cpp
  ShaderProgram.hpp ShaderProgram.cpp
)

set(SOURCES ${SOURCES} "${CMAKE_SOURCE_DIR}/ext/glad/src/glad.c")

add_executable(simple ${SOURCES})

target_link_libraries(simple glfw ${GLFW_LIBRARIES})

add_custom_target(run COMMAND simple DEPENDS simple WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
