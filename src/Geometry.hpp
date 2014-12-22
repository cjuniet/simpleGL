#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <utility>

namespace geometry {
  using segment2 = std::pair<glm::vec2, glm::vec2>;

  // sort by angle from 0 to 2*PI around origin
  float angle2D(const glm::vec2& u, const glm::vec2& v);
  void sort_by_angle(const glm::vec2& origin, std::vector<glm::vec2>& vertices);

  // orient2D returns:
  // > 0 if c is on the left of ab
  // < 0 if c is on the right of ab
  // = 0 if a, b, c are collinear
  float orient2D(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);

  bool box_has_point(const glm::vec2& a, const glm::vec2& b, const glm::vec2& point);

  bool segment_has_point(const glm::vec2& a, const glm::vec2& b, const glm::vec2& point);

  bool intersect_ray_seg(const glm::vec2& o, const glm::vec2& r,
                         const glm::vec2& a, const glm::vec2& b,
                         glm::vec2& point);
  bool intersect_ray_seg(const glm::vec2& o, const glm::vec2& r,
                         const std::vector<segment2>& segments,
                         glm::vec2& point, segment2& segment);

  bool intersect_seg_seg(const glm::vec2& a, const glm::vec2& b,
                         const glm::vec2& c, const glm::vec2& d,
                         glm::vec2& point);
  bool intersect_seg_seg(const glm::vec2& a, const glm::vec2& b,
                         const std::vector<segment2>& segments,
                         glm::vec2& point, segment2& segment);
}
