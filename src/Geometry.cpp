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

}
