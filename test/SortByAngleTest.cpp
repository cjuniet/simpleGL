#include <gtest/gtest.h>
#include "Geometry.hpp"
#include <algorithm>

bool Vec2Pred(const glm::vec2& lhs, const glm::vec2& rhs) {
  return (lhs.x == rhs.x ? lhs.y < rhs.y : lhs.x < rhs.x);
}

void TestSort(const glm::vec2& origin, const std::vector<glm::vec2>& vertices)
{
  auto perms = vertices;
  std::sort(perms.begin(), perms.end(), Vec2Pred);
  do {
    auto v = perms;
    geometry::sort_by_angle(origin, v);
    ASSERT_EQ(vertices, v);
  } while (std::next_permutation(perms.begin(), perms.end(), Vec2Pred));
}

class GeometryTest : public ::testing::Test
{
public:
  GeometryTest()
    : origin(0, 0), u(2, 1), v(1, 2), w(1, 1),
      a(+1, +1), b(-1, +1), c(-1, -1), d(+1, -1)
  {}

  virtual void SetUp()
  {
    vertices.clear();
  }

  const glm::vec2 origin;
  const glm::vec2 u, v, w;
  const glm::vec2 a, b, c, d;
  std::vector<glm::vec2> vertices;
};

TEST_F(GeometryTest, SortOnePointAndOrigin) {
  vertices.push_back(origin);
  vertices.push_back(u);
  TestSort(origin, vertices);
}

TEST_F(GeometryTest, SortTwoPoints) {
  vertices.push_back(u);
  vertices.push_back(v);
  TestSort(origin, vertices);
}

TEST_F(GeometryTest, SortThreePointsAndOrigin) {
  vertices.push_back(origin);
  vertices.push_back(u);
  vertices.push_back(w);
  vertices.push_back(v);
  TestSort(origin, vertices);
}

TEST_F(GeometryTest, SortTwoQuadrants) {
  vertices.push_back(a);
  vertices.push_back(b);
  TestSort(origin, vertices);
}

TEST_F(GeometryTest, SortThreeQuadrants) {
  vertices.push_back(a);
  vertices.push_back(b);
  vertices.push_back(c);
  TestSort(origin, vertices);
}

TEST_F(GeometryTest, SortAllQuadrants) {
  vertices.push_back(a);
  vertices.push_back(b);
  vertices.push_back(c);
  vertices.push_back(d);
  TestSort(origin, vertices);
}

TEST_F(GeometryTest, SortAllQuadrantsAndOrigin) {
  vertices.push_back(origin);
  vertices.push_back(a);
  vertices.push_back(b);
  vertices.push_back(c);
  vertices.push_back(d);
  TestSort(origin, vertices);
}
