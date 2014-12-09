include_directories("${gtest_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/src")

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
  add_definitions("-DGLM_FORCE_RADIANS")
endif()

set(TEST_SOURCES
  SortByAngleTest.cpp
  IntersectTest.cpp
)

set(TEST_SOURCES ${TEST_SOURCES} ../src/Geometry.hpp ../src/Geometry.cpp)

add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests gtest_main)

add_test(UnitTests tests)

add_custom_target(check COMMAND tests --gtest_color=yes DEPENDS tests)
