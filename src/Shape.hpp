#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Shape
{
public:
  enum Type { VEC2, VEC2_COL3, VEC2_COL3_TEX2 };

public:
  Shape(GLenum mode, const std::vector<glm::vec2>& vertices);
  Shape(GLenum mode, Type type, const std::vector<GLfloat>& vertices);
  ~Shape();

  size_t nb_vertices() const;

  void draw() const;
  void draw(GLenum mode, GLint first, GLsizei count) const;

  glm::mat4 get_transform() const;
  void reset_transform();

  std::vector<glm::vec2> get_world_vertices() const;

  void set_origin(float x, float y);
  void set_position(float x, float y);
  void set_rotation(float angle);
  void set_scale(float x, float y);

  void translate(float x, float y);
  void rotate(float angle);
  void scale(float x, float y);

  void clamp_position(float xmin, float xmax, float ymin, float ymax);

private:
  void init_vao();

private:
  GLuint _VAO;
  GLuint _VBO;
  const GLint _mode;
  const Type _type;
  std::vector<GLfloat> _vertices;
  size_t _nb_vertices;

  glm::vec2 _origin;
  glm::vec2 _position;
  glm::vec2 _scale;
  float _rotation;
  mutable glm::mat4 _transform;
  mutable bool _need_update;
};
