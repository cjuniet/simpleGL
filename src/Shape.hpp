#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Shape
{
public:
  enum Type { VEC2, VEC2_COL3, VEC2_COL3_TEX2 };

public:
  Shape(GLenum mode, Type type, const std::vector<GLfloat>& vertices);
  ~Shape();

  void draw() const;

  glm::mat4 get_transform() const;
  void reset_transform();

  void set_position(float x, float y);
  void translate(float x, float y);
  void rotate(float a);
  void scale(float x, float y);

private:
  GLuint _VAO;
  GLuint _VBO;
  const GLint _mode;
  const Type _type;
  std::vector<GLfloat> _vertices;
  GLsizei _nb_vertices;

  glm::mat4 _transform;
};