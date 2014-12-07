#include "Shape.hpp"
#include <glm/gtc/matrix_transform.hpp>

Shape::Shape(GLenum mode, Type type, const std::vector<GLfloat>& vertices)
  : _mode(mode), _type(type), _vertices(vertices)
{
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);

  switch (_type) {
    case Type::VEC2:
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
      glEnableVertexAttribArray(0);
      _nb_vertices = (GLsizei)_vertices.size() / 2;
    case Type::VEC2_COL3:
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
      glEnableVertexAttribArray(1);
      _nb_vertices = (GLsizei)_vertices.size() / 5;
      break;
    case Type::VEC2_COL3_TEX2:
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), nullptr);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
      glEnableVertexAttribArray(2);
      _nb_vertices = (GLsizei)_vertices.size() / 7;
      break;
  }

  glBindVertexArray(0);
}

Shape::~Shape()
{
  glDeleteBuffers(1, &_VBO);
  glDeleteVertexArrays(1, &_VAO);
}

void Shape::draw() const
{
  glBindVertexArray(_VAO);
  glDrawArrays(_mode, 0, _nb_vertices);
  glBindVertexArray(0);
}

glm::mat4 Shape::get_transform() const
{
  return _transform;
}

void Shape::reset_transform()
{
  _transform = glm::mat4();
}

void Shape::set_position(float x, float y)
{
  _transform[3][0] = x;
  _transform[3][1] = y;
}

void Shape::translate(float x, float y)
{
  _transform = glm::translate(_transform, glm::vec3(x, y, 0));
}

void Shape::rotate(float angle)
{
  _transform = glm::rotate(_transform, angle, glm::vec3(0, 0, 1.0f));
}

void Shape::scale(float x, float y)
{
  _transform = glm::scale(_transform, glm::vec3(x, y, 1.0f));
}
