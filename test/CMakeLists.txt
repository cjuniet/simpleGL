include_directories(${gtest_SOURCE_DIR}/include)

set(TEST_SOURCES
  InitTest.cpp
)

add_executable(test_driver ${TEST_SOURCES})
target_link_libraries(test_driver gtest_main glfw ${GLFW_LIBRARIES})
add_test(UnitTests test_driver)

add_custom_target(check COMMAND test_driver --gtest_color=yes DEPENDS test_driver)
