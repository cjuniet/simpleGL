#include "Geometry.hpp"

#include <algorithm>
#include <cmath>

namespace geometry {
  const float _PI = 3.14159265358979323846f;
  const float _2PI = 2 * _PI;

  double angle2D(const glm::vec2& u, const glm::vec2& v)
  {
    const double a = std::atan2(v.y - u.y, v.x - u.x);
    return (a < 0 ? a + _2PI : a);
  }

  void sort_by_angle(const glm::vec2& origin, std::vector<glm::vec2>& vertices)
  {
    std::sort(vertices.begin(), vertices.end(), [origin] (const glm::vec2& lhs, const glm::vec2& rhs) {
      return angle2D(origin, lhs) < angle2D(origin, rhs);
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

  bool intersect_seg_seg(const glm::vec2& a, const glm::vec2& b,
                         const glm::vec2& c, const glm::vec2& d,
                         glm::vec2& point)
  {
    const float a1 = orient2D(a, b, c);
    const float a2 = orient2D(a, b, d);
    if (a1 == 0 && a2 == 0) {
      return false;
    }
    if (a1 == 0 && box_has_point(a, b, c)) {
      point = c;
      return true;
    }
    if (a2 == 0 && box_has_point(a, b, d)) {
      point = d;
      return true;
    }

    const float a3 = orient2D(c, d, a);
    const float a4 = orient2D(c, d, b);
    if (a3 == 0 && a4 == 0) {
      return false;
    }
    if (a3 == 0 && box_has_point(c, d, a)) {
      point = a;
      return true;
    }
    if (a4 == 0 && box_has_point(c, d, b)) {
      point = b;
      return true;
    }

    if (a1 * a2 < 0.0f) {
      // c and d are on different sides of ab
      if (a3 * a4 < 0.0f) {
        // a and b are on different sides of cd
        const float t = a3 / (a3 - a4);
        point = a + t * (b - a);
        return true;
      }
    }
    return false;
  }

  bool intersect_seg_seg(const glm::vec2& a, const glm::vec2& b,
                         const std::vector<segment2>& segments,
                         glm::vec2& point, segment2& segment)
  {
    glm::vec2 p;
    bool found = false;
    float min = (point == a ? std::numeric_limits<float>::infinity() : glm::distance(a, point));
    for (const auto& s : segments) {
      if (intersect_seg_seg(a, b, s.first, s.second, p)) {
        const float d = glm::distance(a, p);
        if (d < min) {
          min = d;
          point = p;
          segment = s;
          found = true;
        }
      }
    }
    return found;
  }
}
