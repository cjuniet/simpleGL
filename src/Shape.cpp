#include "Shape.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <limits>

namespace {
  size_t get_stride(Shape::Type type)
  {
    switch (type) {
      case Shape::Type::VEC2:           return 2;
      case Shape::Type::VEC2_COL3:      return 5;
      case Shape::Type::VEC2_COL3_TEX2: return 7;
    }
    return 0;
  }
}

Shape::Shape(GLenum mode, const std::vector<glm::vec2>& vertices)
  : _mode(mode), _type(Type::VEC2), _scale(1.0f, 1.0f), _rotation(0),
   _need_update(false), _segments_need_update(true)
{
  _vertices.reserve(2 * vertices.size());
  glm::vec2 bl(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());;
  glm::vec2 tr(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
  for (const auto& v : vertices) {
    _vertices.push_back(v.x);
    _vertices.push_back(v.y);
    bl.x = std::min(bl.x, v.x);
    tr.x = std::max(tr.x, v.x);
    bl.y = std::min(bl.y, v.y);
    tr.y = std::max(tr.y, v.y);
  }
  _aabb.first = bl;
  _aabb.second = tr;
  init_vao();
}

Shape::Shape(GLenum mode, Type type, const std::vector<GLfloat>& vertices)
  : _mode(mode), _type(type), _vertices(vertices), _scale(1.0f, 1.0f), _rotation(0),
    _need_update(false), _segments_need_update(true)
{
  init_vao();
}

void Shape::init_vao()
{
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_DYNAMIC_DRAW);

  switch (_type) {
    case Type::VEC2:
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
      glEnableVertexAttribArray(0);
      _nb_vertices = _vertices.size() / 2;
      break;
    case Type::VEC2_COL3:
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
      glEnableVertexAttribArray(1);
      _nb_vertices = _vertices.size() / 5;
      break;
    case Type::VEC2_COL3_TEX2:
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
      glEnableVertexAttribArray(2);
      _nb_vertices = _vertices.size() / 7;
      break;
  }

  glBindVertexArray(0);
}

Shape::~Shape()
{
  glDeleteBuffers(1, &_VBO);
  glDeleteVertexArrays(1, &_VAO);
}

size_t Shape::nb_vertices() const
{
  return _nb_vertices;
}

void Shape::draw() const
{
  glBindVertexArray(_VAO);
  glDrawArrays(_mode, 0, (GLsizei)_nb_vertices);
  glBindVertexArray(0);
}

void Shape::draw(GLenum mode, GLint first, GLsizei count) const
{
  glBindVertexArray(_VAO);
  glDrawArrays(mode, first, count);
  glBindVertexArray(0);
}

glm::mat4 Shape::get_transform() const
{
  if (_need_update) {
    _transform = glm::mat4();
    _transform = glm::translate(_transform, glm::vec3(_origin.x + _position.x, _origin.y + _position.y, 0.0f));
    _transform = glm::rotate(_transform, _rotation, glm::vec3(0.0f, 0.0f , 1.0f));
    _transform = glm::scale(_transform, glm::vec3(_scale.x, _scale.y, 1.0f));
    _need_update = false;
    _segments_need_update = true;
  }
  return _transform;
}

void Shape::reset_transform()
{
  _need_update = true;
  _origin.x = _origin.y = 0;
  _position.x = _position.y = 0;
  _rotation = 0;
  _scale.x = _scale.y = 1;
}

void Shape::set_origin(float x, float y)
{
  _need_update |= (x != _origin.x || y != _origin.y);
  _origin.x = x;
  _origin.y = y;
}

void Shape::set_position(float x, float y)
{
  _need_update |= (x != _position.x || y != _position.y);
  _position.x = x;
  _position.y = y;
}

void Shape::set_rotation(float angle)
{
  _need_update |= (angle != _rotation);
  _rotation = angle;
}

void Shape::translate(float x, float y)
{
  _need_update |= (x != 0 || y != 0);
  _position.x += x;
  _position.y += y;
}

void Shape::rotate(float angle)
{
  _need_update |= (angle != 0);
  _rotation += angle;
}

void Shape::scale(float x, float y)
{
  _need_update |= (x != 1 || y != 1);
  _scale.x *= x;
  _scale.y *= y;
}

void Shape::clamp_position(float xmin, float xmax, float ymin, float ymax)
{
  const float x = std::max(xmin, std::min(xmax, _position.x));
  const float y = std::max(ymin, std::min(ymax, _position.y));
  set_position(x, y);
}

const std::vector<geometry::segment2>& Shape::get_segments() const
{
  const glm::mat4 model = get_transform();
  if (_segments_need_update) {
    glm::vec4 u = model * glm::vec4(_aabb.first.x, _aabb.first.y, 0.0f, 1.0f);
    glm::vec4 v = model * glm::vec4(_aabb.second.x, _aabb.second.y, 0.0f, 1.0f);
    _aabb.first = glm::vec2(u.x, u.y);
    _aabb.second = glm::vec2(v.x, v.y);

    const size_t stride = get_stride(_type);
    const size_t n = _vertices.size() - stride;
    _segments.clear();

    for (size_t i = 0; i < n; i += stride) {
      u = model * glm::vec4(_vertices[i], _vertices[i + 1], 0.0f, 1.0f);
      v = model * glm::vec4(_vertices[i + stride], _vertices[i + stride + 1], 0.0f, 1.0f);
      _segments.push_back({ glm::vec2(u.x, u.y), glm::vec2(v.x, v.y) });
    }
    u = model * glm::vec4(_vertices[n], _vertices[n + 1], 0.0f, 1.0f);
    v = model * glm::vec4(_vertices[0], _vertices[1], 0.0f, 1.0f);
    _segments.push_back({ glm::vec2(u.x, u.y), glm::vec2(v.x, v.y) });

    _segments_need_update = false;
  }
  return _segments;
}

bool Shape::collide_ray(const glm::vec2& o, const glm::vec2& r,
                        glm::vec2& point, geometry::segment2& segment) const
{
  return geometry::intersect_ray_seg(o, r, get_segments(), point, segment);
}

bool Shape::collide_segment(const glm::vec2& a, const glm::vec2& b,
                            glm::vec2& point, geometry::segment2& segment) const
{
  return geometry::intersect_seg_seg(a, b, get_segments(), point, segment);
}
