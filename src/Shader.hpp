#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
  Shader(const std::string& vertex_file, const std::string& fragment_file);
  ~Shader();

  void attach() const;
  void detach() const;

  void set_uniform(const GLchar* name, GLfloat value) const;
  void set_uniform(const GLchar* name, const glm::mat4& value) const;

private:
  void load(const std::string& vertex_file, const std::string& fragment_file);

private:
  GLuint _vertex;
  GLuint _fragment;
  GLuint _program;
};
