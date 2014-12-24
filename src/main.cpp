#include "Geometry.hpp"
#include "ParticleSystem.hpp"
#include "Shader.hpp"
#include "Shape.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

bool g_reset = true;
bool g_pause = false;
bool g_step = false;
bool g_wireframe = false;

void main_loop(GLFWwindow* window);
void update_title(GLFWwindow* window, double frametime);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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
  //GLFWwindow* window = glfwCreateWindow(1920, 1080, "SimpleGL", glfwGetPrimaryMonitor(), nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSwapInterval(1);

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
  glPointSize(4.0f);

  const float ratio = (float)width / height;
  const glm::mat4 proj_matrix = glm::ortho<float>(-ratio, ratio, -1, 1);

  Shape cursor(GL_POINTS, Shape::VEC2, { 0.0f, 0.0f });

  Shape room(GL_LINE_LOOP, Shape::VEC2, {
    -ratio, -1.0f, ratio, -1.0f, +ratio, +1.0f, -ratio, +1.0f
  });

  Shader shape_shader("assets/shaders/basic.vertex", "assets/shaders/basic.fragment");
  shape_shader.attach();
  shape_shader.set_uniform("proj", proj_matrix);
  shape_shader.detach();

  Shader cursor_shader("assets/shaders/cursor.vertex", "assets/shaders/cursor.fragment");
  cursor_shader.attach();
  cursor_shader.set_uniform("proj", proj_matrix);
  cursor_shader.detach();

  double xpos, ypos, oldxpos, oldypos;
  xpos = ypos = oldxpos = oldypos = 0;
  glfwSetCursorPos(window, xpos, ypos);

  ParticleSystem ps({-ratio, -1.0f}, {ratio, 1.0f});

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glfwGetCursorPos(window, &xpos, &ypos);

    double frametime = glfwGetTime();
    const float dx = (float)(2.0 * (xpos - oldxpos) / width);
    const float dy = (float)(2.0 * (oldypos - ypos) / height);
    oldxpos = xpos; oldypos = ypos;

    cursor.translate(dx, dy);
    cursor.clamp_position(-ratio, ratio, -1.0f, 1.0f);
    const glm::mat4 cursor_model = cursor.get_transform();
    const glm::vec2 cursor_pos(cursor_model[3][0], cursor_model[3][1]);

    if (g_reset) {
      ps.read("assets/particles.txt");
      g_reset = false;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    cursor_shader.attach();

    const std::vector<glm::vec2>& particles = ps.particles();
    Shape points(GL_POINTS, particles);
    cursor_shader.set_uniform("model", points.get_transform());
    points.draw();

    std::vector<glm::vec2> vertices;
    for (const auto& c : ps.constraints()) {
      vertices.push_back(particles[c.first]);
      vertices.push_back(particles[c.second]);
    }
    Shape lines(GL_LINES, vertices);
    cursor_shader.set_uniform("model", lines.get_transform());
    lines.draw();

    //cursor_shader.set_uniform("model", cursor.get_transform());
    //cursor.draw();

    if (!g_pause || g_step) {
      ps.step();
    }

    frametime = glfwGetTime() - frametime;

    glfwSwapBuffers(window);

    update_title(window, frametime);
  }
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
  if (now - tlast >= 1.0) {
    double fps = frames / (now - tlast);
    double tpf = 1000 * ftime / frames;
    frames = 0;
    ftime = 0;
    tlast = now;

    sprintf(title, "SimpleGL - fps=%.0f tpf=%.3fms", fps, tpf);
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

  if (key == GLFW_KEY_P && action == GLFW_PRESS) {
    g_pause = !g_pause;
  }

  if (key == GLFW_KEY_S) {
    g_step = (action != GLFW_RELEASE);
  }

  if (key == GLFW_KEY_R && action == GLFW_PRESS && action != GLFW_REPEAT) {
    g_reset = true;
  }
}
