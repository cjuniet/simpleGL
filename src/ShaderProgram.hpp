#pragma once

#include <glad/glad.h>
#include <string>

class ShaderProgram
{
public:
  ShaderProgram();
  ~ShaderProgram();

  void load(const std::string& vertex_file, const std::string& fragment_file);
  void use() const;

private:
  GLuint _vertex;
  GLuint _fragment;
  GLuint _program;
};
