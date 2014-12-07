#include "Shader.hpp"
#include "Shape.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

#define Logger std::cerr
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void main_loop(GLFWwindow* window);

int main(int argc, char* argv[])
{
  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  GLFWwindow* window = glfwCreateWindow(800, 600, "SimpleGL", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  if (!gladLoadGL()) {
    glfwTerminate();
    return -1;
  }

  try {
    main_loop(window);
  } catch (const std::runtime_error& re) {
    Logger << re.what() << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwTerminate();
  return 0;
}

void main_loop(GLFWwindow* window)
{
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  const float ratio = (float)width / height;
  const glm::mat4 proj_matrix = glm::ortho<float>(-ratio, ratio, -1, 1);

  Shape shape(GL_TRIANGLE_FAN, Shape::VEC2_COL3,
  {
    -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 1.0f, 1.0f, 1.0f
  });

  Shader shape_shader;
  shape_shader.load("assets/shaders/basic.vertex", "assets/shaders/basic.fragment");
  shape_shader.attach();
  shape_shader.set_uniform("proj", proj_matrix);
  shape_shader.detach();

  Shape cursor(GL_POINTS, Shape::VEC2, { 0.0f, 0.0f });

  Shader cursor_shader;
  cursor_shader.load("assets/shaders/cursor.vertex", "assets/shaders/cursor.fragment");
  cursor_shader.attach();
  cursor_shader.set_uniform("proj", proj_matrix);
  cursor_shader.detach();

  double xpos, ypos;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glfwGetCursorPos(window, &xpos, &ypos);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shape.rotate(0.01f);
    shape_shader.attach();
    shape_shader.set_uniform("model", shape.get_transform());
    shape.draw();
    shape_shader.detach();

    const float cx = ratio * (2 * (float)xpos / width - 1);
    const float cy = -(2 * (float)ypos / height - 1);
    cursor.set_position(cx, cy);
    cursor_shader.attach();
    cursor_shader.set_uniform("model", cursor.get_transform());
    cursor.draw();
    cursor_shader.detach();

    glfwSwapBuffers(window);
  }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key == GLFW_KEY_SPACE && action != GLFW_REPEAT) {
    glPolygonMode(GL_FRONT_AND_BACK, action == GLFW_PRESS ? GL_LINE : GL_FILL);
  }
}
