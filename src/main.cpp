#include "ShaderProgram.hpp"
#include "Shape.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

std::ofstream Logger("out.txt");
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

  ShaderProgram shader;
  shader.load("assets/shaders/basic.vertex.glsl", "assets/shaders/basic.fragment.glsl");

  const float ratio = (float)width / height;
  const glm::mat4 proj_matrix = glm::ortho<float>(-ratio, ratio, -1, 1);
  shader.attach();
  shader.set_uniform("proj", proj_matrix);
  shader.detach();

  Shape shape(GL_TRIANGLE_FAN, Shape::VEC2_COL3,
  {
    -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 1.0f, 1.0f, 1.0f
  });

  Shape cursor(GL_POINTS, Shape::VEC2_COL3, { 0.0f, 0.0f, 1.0f, 1.0f, 1.0f });

  double xpos, ypos;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glfwGetCursorPos(window, &xpos, &ypos);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.attach();

    shape.rotate(0.01f);
    shader.set_uniform("model", shape.get_transform());
    shape.draw();

    cursor.set_position(ratio*(2 * (float)xpos / width - 1), -(2 * (float)ypos / height - 1));
    shader.set_uniform("model", cursor.get_transform());
    cursor.draw();

    shader.detach();

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
