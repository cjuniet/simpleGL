#include "Geometry.hpp"
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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  const float ratio = (float)width / height;
  const glm::mat4 proj_matrix = glm::ortho<float>(-ratio, ratio, -1, 1);

  Shape shape1(GL_LINE_LOOP, Shape::VEC2_COL3, {
    -0.2f, -0.2f, 0.0f, 0.0f, 1.0f,
     0.2f, -0.2f, 0.0f, 0.5f, 1.0f,
     0.2f,  0.2f, 0.5f, 0.5f, 1.0f,
    -0.2f,  0.2f, 0.5f, 0.0f, 1.0f
  });
  shape1.set_position(0.5f, -0.25f);
  shape1.rotate(-0.25f);

  Shape shape2(GL_LINE_LOOP, Shape::VEC2_COL3, {
    -0.3f, -0.3f, 1.0f, 0.0f, 0.0f,
    0.3f, -0.3f, 1.0f, 0.5f, 0.0f,
    0.2f, 0.0f, 1.0f, 0.5f, 0.5f,
    0.3f, 0.3f, 1.0f, 0.5f, 0.5f,
    -0.3f, 0.3f, 1.0f, 0.0f, 0.5f,
    -0.2f, 0.0f, 1.0f, 0.0f, 0.5f,
    -0.3f, -0.3f, 1.0f, 0.0f, 0.0f
  });
  shape2.set_position(-0.25f, 0.5f);
  shape2.rotate(0.5f);

  Shape shape3(GL_LINE_LOOP, Shape::VEC2_COL3, {
    -0.1f, -0.1f, 0.0f, 1.0f, 0.0f,
     0.0f, -0.125f, 0.5f, 1.0f, 0.0f,
     0.1f, -0.1f, 0.5f, 1.0f, 0.5f,
     0.125f, 0.0f, 0.0f, 1.0f, 0.5f,
     0.1f,  0.1f, 0.0f, 1.0f, 0.5f,
     0.0f,  0.125f, 0.5f, 1.0f, 0.5f,
    -0.1f,  0.1f, 0.5f, 1.0f, 0.0f,
    -0.125f, 0.0f, 0.0f, 1.0f, 0.0f
  });
  shape3.set_position(-0.5f, -0.75f);
  shape3.rotate(0.1f);

  Shader shape_shader("assets/shaders/basic.vertex", "assets/shaders/basic.fragment");
  shape_shader.attach();
  shape_shader.set_uniform("proj", proj_matrix);
  shape_shader.detach();

  Shape cursor(GL_POINTS, Shape::VEC2, { 0.0f, 0.0f,
    0.01f, 0.0f, -0.01f, 0.0f,
    0.0f, 0.01f, 0.0f, -0.01f
  });

  Shader cursor_shader("assets/shaders/cursor.vertex", "assets/shaders/cursor.fragment");
  cursor_shader.attach();
  cursor_shader.set_uniform("proj", proj_matrix);
  cursor_shader.detach();

  Shader fillcolor_shader("assets/shaders/fillcolor.vertex", "assets/shaders/fillcolor.fragment");
  fillcolor_shader.attach();
  fillcolor_shader.set_uniform("proj", proj_matrix);
  fillcolor_shader.detach();

  double xpos, ypos, oldxpos, oldypos;
  xpos = ypos = oldxpos = oldypos = 0;
  glfwSetCursorPos(window, xpos, ypos);

  int offset = 0;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glfwGetCursorPos(window, &xpos, &ypos);
    const float dx = (float)(2.0 * (xpos - oldxpos) / width);
    const float dy = (float)(2.0 *(oldypos - ypos) / height);
    oldxpos = xpos; oldypos = ypos;

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //shape1.rotate(0.01f);
    //shape2.rotate(0.01f);
    //shape3.rotate(0.01f);

    shape_shader.attach();
    shape_shader.set_uniform("model", shape1.get_transform());
    shape1.draw();
    shape_shader.set_uniform("model", shape2.get_transform());
    shape2.draw();
    shape_shader.set_uniform("model", shape3.get_transform());
    shape3.draw();
    shape_shader.detach();

    const glm::vec2 origin = cursor.get_world_vertices().front();
    std::vector<glm::vec2> coords;
    coords.push_back(origin);
    std::vector<glm::vec2> v = shape1.get_world_vertices();
    coords.insert(coords.end(), v.begin(), v.end());
    v = shape2.get_world_vertices();
    coords.insert(coords.end(), v.begin(), v.end());
    v = shape3.get_world_vertices();
    coords.insert(coords.end(), v.begin(), v.end());

    geometry::sort_by_angle(origin, coords);

    Shape rays(GL_TRIANGLE_FAN, coords);
    fillcolor_shader.attach();
    fillcolor_shader.set_uniform("model", rays.get_transform());
    fillcolor_shader.set_uniform("fillcolor", glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    GLsizei count = 1 + (offset++/20) % rays.nb_vertices();
    rays.draw(GL_TRIANGLE_FAN, 0, count);

    int pmode;
    glGetIntegerv(GL_POLYGON_MODE, &pmode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    fillcolor_shader.set_uniform("fillcolor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    rays.draw();
    glPolygonMode(GL_FRONT_AND_BACK, pmode);
    fillcolor_shader.detach();

    cursor_shader.attach();
    cursor.translate(dx, dy);
    cursor.clamp_position(-ratio, ratio, -1.0f, 1.0f);
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
