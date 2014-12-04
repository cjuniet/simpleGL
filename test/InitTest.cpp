#include <gtest/gtest.h>
#include <GLFW/glfw3.h>

TEST(SimpleGL, GlfwInitTest) {
  ASSERT_TRUE(glfwInit());
  glfwTerminate();
}
