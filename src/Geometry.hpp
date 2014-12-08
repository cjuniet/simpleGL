#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace geometry {
  void sort_by_angle(const glm::vec2& origin, std::vector<glm::vec2>& vertices);
}