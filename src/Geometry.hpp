#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace geometry {
  // sort by angle from 0 to 2*PI around origin
  void sort_by_angle(const glm::vec2& origin, std::vector<glm::vec2>& vertices);

  // orient2D returns:
  // > 0 if c is on the left of ab
  // < 0 if c is on the right of ab
  // = 0 if a, b, c are collinear
  float orient2D(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);

  bool box_has_point(const glm::vec2& a, const glm::vec2& b, const glm::vec2& point);
  bool segment_has_point(const glm::vec2& a, const glm::vec2& b, const glm::vec2& point);
  
  bool intersect_seg_seg(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, glm::vec2& point);
  bool intersect_seg_poly(const glm::vec2& a, const glm::vec2& b, const std::vector<glm::vec2>& vertices, glm::vec2& nearest);
}