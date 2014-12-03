#include "ShaderProgram.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {
void compile_glsl(const std::string& filename, GLuint shader)
{
  std::ifstream ifs(filename);
  if (!ifs) {
    throw std::runtime_error("compile_glsl(): unable to open " + filename);
  }

  std::ostringstream oss;
  oss << ifs.rdbuf();
  ifs.close();

  const std::string glsl = oss.str();
  const char* str = glsl.c_str();
  glShaderSource(shader, 1, &str, nullptr);

  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char buf[512];
    glGetShaderInfoLog(shader, sizeof(buf), nullptr, buf);
    throw std::runtime_error(std::string("compile_glsl(): ") + buf);
  }
}
}

ShaderProgram::ShaderProgram()
  : _vertex(glCreateShader(GL_VERTEX_SHADER)),
    _fragment(glCreateShader(GL_FRAGMENT_SHADER)),
    _program(glCreateProgram())
{}

void ShaderProgram::load(const std::string& vertex_file, const std::string& fragment_file)
{
  compile_glsl(vertex_file, _vertex);
  compile_glsl(fragment_file, _fragment);

  glAttachShader(_program, _vertex);
  glAttachShader(_program, _fragment);
  glLinkProgram(_program);

  GLint success;
  glGetProgramiv(_program, GL_LINK_STATUS, &success);
  if (!success) {
    char buf[512];
    glGetProgramInfoLog(_program, sizeof(buf), nullptr, buf);
    throw std::runtime_error(std::string("Shader::Shader(): ") + buf);
  }
}

ShaderProgram::~ShaderProgram()
{
  glDeleteShader(_vertex);
  glDeleteShader(_fragment);
  glDeleteProgram(_program);
}

void ShaderProgram::use() const
{
  glUseProgram(_program);
}
