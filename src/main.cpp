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

bool g_rotation = false;
bool g_wireframe = false;

void update_title(GLFWwindow* window, double frametime);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void main_loop(GLFWwindow* window);
std::vector<glm::vec2> compute_fov(const std::vector<Shape*>& shapes,
                                   const glm::vec2& cursor_pos,
                                   float ratio);

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
    std::cerr << re.what() << std::endl;
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

  Shape cursor(GL_POINTS, Shape::VEC2, { 0.0f, 0.0f,
    0.01f, 0.0f, -0.01f, 0.0f,
    0.0f, 0.01f, 0.0f, -0.01f
  });

  std::vector<Shape*> shapes;

  Shape room(GL_LINE_LOOP, Shape::VEC2, {
    -ratio, -1.0f, ratio, -1.0f, +ratio, +1.0f, -ratio, +1.0f
  });
  shapes.push_back(&room);

  Shape shape1(GL_TRIANGLE_FAN, Shape::VEC2_COL3, {
    -0.2f, -0.2f, 0.0f, 0.0f, 1.0f,
     0.2f, -0.2f, 0.0f, 0.5f, 1.0f,
     0.2f,  0.2f, 0.5f, 0.5f, 1.0f,
    -0.2f,  0.2f, 0.5f, 0.0f, 1.0f
  });
  shape1.set_position(0.5f, -0.25f);
  shape1.rotate(-0.25f);
  shapes.push_back(&shape1);

  Shape shape2(GL_TRIANGLE_FAN, Shape::VEC2_COL3, {
     0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
    -0.3f, -0.3f, 1.0f, 0.0f, 0.0f,
     0.3f, -0.3f, 1.0f, 0.5f, 0.0f,
     0.2f,  0.0f, 1.0f, 0.5f, 0.5f,
     0.3f,  0.3f, 1.0f, 0.5f, 0.5f,
    -0.3f,  0.3f, 1.0f, 0.0f, 0.5f,
    -0.2f,  0.0f, 1.0f, 0.0f, 0.5f,
    -0.3f, -0.3f, 1.0f, 0.0f, 0.0f
  });
  shape2.set_position(-0.25f, 0.5f);
  shape2.rotate(0.5f);
  shapes.push_back(&shape2);

  Shape shape3(GL_TRIANGLE_FAN, Shape::VEC2_COL3, {
    -0.1f, -0.1f, 0.0f, 1.0f, 0.0f,
     0.0f, -0.125f, 0.5f, 1.0f, 0.0f,
     0.1f, -0.1f, 0.5f, 1.0f, 0.5f,
     0.125f, 0.0f, 0.0f, 1.0f, 0.5f,
     0.1f, 0.1f, 0.0f, 1.0f, 0.5f,
     0.0f, 0.125f, 0.5f, 1.0f, 0.5f,
    -0.1f, 0.1f, 0.5f, 1.0f, 0.0f,
    -0.125f, 0.0f, 0.0f, 1.0f, 0.0f
  });
  shape3.set_position(-0.5f, -0.75f);
  shape3.rotate(0.1f);
  shapes.push_back(&shape3);

  Shader shape_shader("assets/shaders/basic.vertex", "assets/shaders/basic.fragment");
  shape_shader.attach();
  shape_shader.set_uniform("proj", proj_matrix);
  shape_shader.detach();

  Shader cursor_shader("assets/shaders/cursor.vertex", "assets/shaders/cursor.fragment");
  cursor_shader.attach();
  cursor_shader.set_uniform("proj", proj_matrix);
  cursor_shader.detach();

  Shader fov_shader("assets/shaders/fov.vertex", "assets/shaders/fov.fragment");
  fov_shader.attach();
  fov_shader.set_uniform("proj", proj_matrix);
  fov_shader.detach();

  double xpos, ypos, oldxpos, oldypos;
  xpos = ypos = oldxpos = oldypos = 0;
  glfwSetCursorPos(window, xpos, ypos);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glfwGetCursorPos(window, &xpos, &ypos);

    double frametime = glfwGetTime();

    const float dx = (float)(2.0 * (xpos - oldxpos) / width);
    const float dy = (float)(2.0 *(oldypos - ypos) / height);
    oldxpos = xpos; oldypos = ypos;

    cursor.translate(dx, dy);
    cursor.clamp_position(-ratio+0.0001f, ratio-0.0001f, -0.9999f, 0.9999f);
    const glm::mat4 cursor_model = cursor.get_transform();
    const glm::vec2 cursor_pos(cursor_model[3][0], cursor_model[3][1]);

    if (g_rotation) {
      shape1.rotate(0.01f);
      shape2.rotate(0.02f);
      shape3.rotate(0.03f);
    }

    std::vector<glm::vec2> fov_vertices = compute_fov(shapes, cursor_pos, ratio);
    const Shape fov(GL_TRIANGLE_FAN, fov_vertices);
    fov_vertices = compute_fov(shapes, cursor_pos + glm::vec2(0.00f, 0.05f), ratio);
    const Shape fovU(GL_TRIANGLE_FAN, fov_vertices);
    fov_vertices = compute_fov(shapes, cursor_pos - glm::vec2(0.00f, 0.05f), ratio);
    const Shape fovD(GL_TRIANGLE_FAN, fov_vertices);
    fov_vertices = compute_fov(shapes, cursor_pos - glm::vec2(0.05f, 0.00f), ratio);
    const Shape fovL(GL_TRIANGLE_FAN, fov_vertices);
    fov_vertices = compute_fov(shapes, cursor_pos + glm::vec2(0.05f, 0.00f), ratio);
    const Shape fovR(GL_TRIANGLE_FAN, fov_vertices);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    fov_shader.attach();
    fov_shader.set_uniform("origin", cursor_pos);
    fov_shader.set_uniform("lightcolor", glm::vec4(1.0f, 0.8f, 0.8f, 0.25f));
    fov_shader.set_uniform("model", fov.get_transform());
    fov.draw();
    fovU.draw();
    fovD.draw();
    fovL.draw();
    fovR.draw();

    shape_shader.attach();
    for (auto s : shapes) {
      shape_shader.set_uniform("model", s->get_transform());
      s->draw();
    }

    cursor_shader.attach();
    cursor_shader.set_uniform("model", cursor.get_transform());
    cursor.draw();

    frametime = glfwGetTime() - frametime;

    glfwSwapBuffers(window);
    update_title(window, frametime);
  }
}

std::vector<glm::vec2> compute_fov(const std::vector<Shape*>& shapes,
                                   const glm::vec2& cursor_pos, float ratio)
{
  glm::vec2 last_point = cursor_pos;
  std::vector<glm::vec2> fov_vertices;
  geometry::segment2 last_segment;
  for (float a = 0; a <= 6.283f; a += 0.001f) {
    glm::vec2 ray(2 * ratio * glm::cos(a), 2 * ratio * glm::sin(a));
    glm::vec2 point = cursor_pos;
    geometry::segment2 segment;
    for (auto s : shapes) {
      s->collide_segment(cursor_pos, ray, point, segment);
    }
    if (segment != last_segment) {
      fov_vertices.push_back(last_point);
      fov_vertices.push_back(point);
      last_segment = segment;
    }
    last_point = point;
  }
  fov_vertices.push_back(fov_vertices[1]);
  return fov_vertices;
}

void update_title(GLFWwindow* window, double frametime)
{
  static char title[256];
  static int frames = 0;
  static double ftime = 0;
  static double tlast = 0;

  ++frames;
  ftime += frametime;
  double now = glfwGetTime();
  if (now - tlast > 1.0) {
    double fps = frames / (now - tlast);
    double tpf = 1000 * (ftime / frames);
    frames = 0;
    ftime = 0;
    tlast = now;

    sprintf(title, "SimpleGL - fps=%d tpf=%dms", (int)fps, (int)tpf);
    glfwSetWindowTitle(window, title);
  }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key == GLFW_KEY_W && action != GLFW_PRESS) {
    g_wireframe = !g_wireframe;
    glPolygonMode(GL_FRONT_AND_BACK, g_wireframe == GLFW_PRESS ? GL_LINE : GL_FILL);
  }

  if (key == GLFW_KEY_R && action == GLFW_PRESS) {
    g_rotation = !g_rotation;
  }
}
