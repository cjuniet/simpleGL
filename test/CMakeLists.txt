include_directories("${gtest_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/src")

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
  add_definitions("-DGLM_FORCE_RADIANS")
endif()

set(TEST_SOURCES
  InitTest.cpp
  SortByAngleTest.cpp ../src/Geometry.hpp ../src/Geometry.cpp
)

add_executable(test_driver ${TEST_SOURCES})
target_link_libraries(test_driver gtest_main glfw ${GLFW_LIBRARIES})
add_test(UnitTests test_driver)

add_custom_target(check COMMAND test_driver --gtest_color=yes DEPENDS test_driver)
