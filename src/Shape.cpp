#include "Shape.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

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
  : _mode(mode), _type(Type::VEC2),
    _scale(1.0f, 1.0f), _rotation(0), _need_update(false)
{
  _vertices.reserve(2 * vertices.size());
  for (const auto& v : vertices) {
    _vertices.push_back(v.x);
    _vertices.push_back(v.y);
  }
  init_vao();
}

Shape::Shape(GLenum mode, Type type, const std::vector<GLfloat>& vertices)
  : _mode(mode), _type(type), _vertices(vertices),
    _scale(1.0f, 1.0f), _rotation(0), _need_update(false)
{
  init_vao();
}

void Shape::init_vao()
{
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);

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
  }
  return _transform;
}

void Shape::reset_transform()
{
  _need_update = false;
  _transform = glm::mat4();
}

std::vector<glm::vec2> Shape::get_world_vertices() const
{
  std::vector<glm::vec2> vertices;
  const glm::mat4 model = get_transform();
  const size_t stride = get_stride(_type);
  for (size_t i = 0; i < _vertices.size(); i += stride) {
    glm::vec4 point(_vertices[i], _vertices[i+1], 0.0f, 1.0f);
    point = model * point;
    vertices.push_back(glm::vec2(point.x, point.y));
  }
  return vertices;
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

