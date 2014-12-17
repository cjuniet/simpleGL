#pragma once

#include "Geometry.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utility>
#include <vector>

class Shape
{
public:
  enum Type { VEC2, VEC2_COL3, VEC2_COL3_TEX2 };

public:
  Shape(GLenum mode, const std::vector<glm::vec2>& vertices);
  Shape(GLenum mode, Type type, const std::vector<GLfloat>& vertices);
  Shape(const Shape&) = delete;
  Shape& operator=(const Shape&) = delete;
  ~Shape();

  size_t nb_vertices() const;

  void draw() const;
  void draw(GLenum mode, GLint first, GLsizei count) const;

  glm::mat4 get_transform() const;
  void reset_transform();

  void set_origin(float x, float y);
  void set_position(float x, float y);
  void set_rotation(float angle);
  void set_scale(float x, float y);

  void translate(float x, float y);
  void rotate(float angle);
  void scale(float x, float y);

  void clamp_position(float xmin, float xmax, float ymin, float ymax);

  const std::vector<geometry::segment2>& get_segments() const;
  bool collide_segment(const glm::vec2& a, const glm::vec2& b,
                       glm::vec2& point, geometry::segment2& segment) const;

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

private:
  mutable glm::mat4 _transform;
  mutable geometry::segment2 _aabb;
  mutable std::vector<geometry::segment2> _segments;
  mutable bool _need_update;
  mutable bool _segments_need_update;
};
