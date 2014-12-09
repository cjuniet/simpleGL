#include "Geometry.hpp"

#include <algorithm>
#include <cmath>

namespace geometry {
  const float _PI = 3.14159265358979323846f;
  const float _2PI = 2 * _PI;

  void sort_by_angle(const glm::vec2& origin, std::vector<glm::vec2>& vertices)
  {
    std::sort(vertices.begin(), vertices.end(), [origin] (const glm::vec2& lhs, const glm::vec2& rhs) {
      float a = std::atan2f(lhs.y - origin.y, lhs.x - origin.x);
      if (a < 0) a += _2PI;
      float b = std::atan2f(rhs.y - origin.y, rhs.x - origin.x);
      if (b < 0) b += _2PI;
      return a < b;
    });
  }

  float orient2D(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
  {
    return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
  }

  bool box_has_point(const glm::vec2& a, const glm::vec2& b, const glm::vec2& point)
  {
    const auto xx = std::minmax(a.x, b.x);
    const auto yy = std::minmax(a.y, b.y);
    return point.x >= xx.first && point.x <= xx.second && point.y >= yy.first && point.y <= yy.second;
  }

  bool segment_has_point(const glm::vec2& a, const glm::vec2& b, const glm::vec2& point)
  {
    return box_has_point(a, b, point) && orient2D(a, b, point) == 0;
  }

  bool intersect_seg_seg(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, glm::vec2& point)
  {
    const float a1 = orient2D(a, b, c);
    const float a2 = orient2D(a, b, d);    
    if (a1 * a2 < 0.0f) {
      // c and d are on different sides of ab
      const float a3 = orient2D(c, d, a);
      const float a4 = orient2D(c, d, b);
      if (a3 * a4 < 0.0f) {
        // a and b are on different sides of cd
        const float t = a3 / (a3 - a4);
        point = a + t * (b - a);
        return true;
      }
    }
    return false;
  }

  bool intersect_seg_poly(const glm::vec2& a, const glm::vec2& b, const std::vector<glm::vec2>& vertices, glm::vec2& nearest)
  {
    glm::vec2 p;
    bool found = false;
    float min = std::numeric_limits<float>::infinity();
    const int n = vertices.size() - 1;

    for (int i = 0; i < n; ++i) {
      if (intersect_seg_seg(a, b, vertices[i], vertices[i + 1], p)) {
        const float d = glm::distance(a, p);
        if (d < min) {
          min = d;
          nearest = p;
          found = true;
        }
      }
    }
    
    if (intersect_seg_seg(a, b, vertices[n], vertices[0], p) && glm::distance(a, p) < min) {
      nearest = p;
      return true;
    }

    return found;
  }
}
