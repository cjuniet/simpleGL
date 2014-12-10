#include <gtest/gtest.h>
#include "Geometry.hpp"

class IntersectTest : public ::testing::Test
{
public:
  IntersectTest()
    : a(0, 0), b(10, 10)
  {}

  virtual void SetUp()
  {}

  const glm::vec2 a;
  const glm::vec2 b;
};

TEST_F(IntersectTest, SegmentAndSelfPoint)
{
  ASSERT_TRUE(geometry::segment_has_point(a, b, a));
  ASSERT_TRUE(geometry::segment_has_point(a, b, b));
}

TEST_F(IntersectTest, SegmentAndPointInside)
{
  const glm::vec2 c(1, 1);
  const glm::vec2 d(9, 9);
  ASSERT_TRUE(geometry::segment_has_point(a, b, c));
  ASSERT_TRUE(geometry::segment_has_point(a, b, d));
}

TEST_F(IntersectTest, SegmentAndPointOutside)
{
  const glm::vec2 c(1, 2);
  const glm::vec2 d(2, 1);
  ASSERT_FALSE(geometry::segment_has_point(a, b, c));
  ASSERT_FALSE(geometry::segment_has_point(a, b, d));
}

TEST_F(IntersectTest, SegmentAndPointOutsideAligned)
{
  const glm::vec2 c(-1, -1);
  const glm::vec2 d(11, 11);
  ASSERT_FALSE(geometry::segment_has_point(a, b, c));
  ASSERT_FALSE(geometry::segment_has_point(a, b, d));
}

TEST_F(IntersectTest, SegmentAndSelf)
{
  glm::vec2 p;
  ASSERT_FALSE(geometry::intersect_seg_seg(a, b, a, b, p));
}

TEST_F(IntersectTest, SegmentAndSegmentOverlap)
{
  glm::vec2 p;
  const glm::vec2 c(5, 5);
  const glm::vec2 d(15, 15);
  ASSERT_FALSE(geometry::intersect_seg_seg(a, b, c, d, p));
}

TEST_F(IntersectTest, SegmentAndSegmentOutside)
{
  glm::vec2 p;
  const glm::vec2 c(0, 1);
  const glm::vec2 d(10, 11);
  ASSERT_FALSE(geometry::intersect_seg_seg(a, b, c, c, p));
}

TEST_F(IntersectTest, SegmentAndSegmentIntersectMiddle)
{
  glm::vec2 p;
  const glm::vec2 c(0, 10);
  const glm::vec2 d(10, 0);
  ASSERT_TRUE(geometry::intersect_seg_seg(a, b, c, d, p));
  const glm::vec2 q(5, 5);
  ASSERT_EQ(q, p);
}

TEST_F(IntersectTest, SegmentAndSegmentIntersectOutside)
{
  glm::vec2 p;
  const glm::vec2 c(0, 10);
  const glm::vec2 d(4, 6);
  ASSERT_FALSE(geometry::intersect_seg_seg(a, b, c, d, p));
}

TEST_F(IntersectTest, SegmentAndSegmentIntersectAtEnds)
{
  glm::vec2 p;
  const glm::vec2 c1(-5, +5);
  const glm::vec2 d1(+5, -5);
  ASSERT_TRUE(geometry::intersect_seg_seg(a, b, c1, d1, p));
  ASSERT_EQ(a, p);

  const glm::vec2 c2(5, 15);
  const glm::vec2 d2(15, 5);
  ASSERT_TRUE(geometry::intersect_seg_seg(a, b, c2, d2, p));
  ASSERT_EQ(b, p);
}
