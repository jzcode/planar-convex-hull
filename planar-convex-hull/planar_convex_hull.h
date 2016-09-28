/*Author: Jason Zadwick. Code Modified: Summmer 2016.*/

#ifndef PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_H_
#define PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_H_
#include "stdafx.h"

namespace planargeometry {
// A class to represent two-dimensional point objects.
// Example:
//  float a = -298, b = 9876;
//  planargeometry::Point2D p{a, b};
class Point2D {
 public:
  Point2D() = default;
  ~Point2D() = default;
  Point2D(const Point2D&) = default;
  Point2D(Point2D&&) = default;
  Point2D(const float& a, const float& b) : // Initialization constructor.
    x_{ std::uniform_real_distribution<float>{a, b}(std::default_random_engine{std::random_device{}()}) },
    y_{ std::uniform_real_distribution<float>{a, b}(std::default_random_engine{std::random_device{}()}) } {}
  Point2D& operator=(const Point2D&) = default;
  Point2D& operator=(Point2D&&) = default;
  // Local data member access methods.
  float GetX() const { return{ x_ }; }
  float GetY() const { return{ y_ }; }
  double XToDouble() const { return{ x_ }; }
  double YToDouble() const { return{ y_ }; }
  // Overload insertion operator to print Point2D objects.
  friend std::ostream& operator<<(std::ostream&, const Point2D&);

 private:
  // Local data members for coordinates and the random number generator object.
  float x_, y_;
};

// A class to handle all aspects related to computing the planar convex hull of a set of points.
// Example:
//  int n = 300;
//  float a = -400, b = 1235;
//  planargeometry::ConvexHull convex_envelope{n, a, b, [](const planargeometry::Point2D& l, const planargeometry::Point2D& r) -> bool {
//      if (l.GetX() == r.GetX()) {
//        return l.GetY() < r.GetY() ? true : false;
//      }
//      else {
//        return l.GetX() < r.GetX() ? true : false;
//      }
//  }};
template<class Compare = std::function<bool(const Point2D&, const Point2D&)>>
class ConvexHull {
 public:
  ConvexHull() = default;
  ~ConvexHull() = default;
  ConvexHull(const ConvexHull&) = default;
  ConvexHull(ConvexHull&&) = default;
  ConvexHull(int, float, float, Compare&&); // Initialization constructor.
  ConvexHull& operator=(const ConvexHull&) = default;
  ConvexHull& operator=(ConvexHull&&) = default;
  // Prints the contents of convex_hull_ local to standard output.
  // May throw a std::out_of_range exception.
  void PrintHull() const;

 private:
  // An index into convex_hull_ locating the maximal vertex.
  size_t maximal_hull_vertex_index_;
  // Lexicographically ordered set filled with Points from random sample.
  std::set<Point2D, Compare> x_monotone_chain_;
  // Is modified until it stores the points representing the planar convex hull starting with the lexicographically  
  // first point, P0, with subsequent points being ordered as they would appear in a clockwise traversal of the 
  // planar convex hull vertices, beginning from P0.
  std::vector<Point2D> convex_hull_;
  // Computes the planar convex hull based on the contents of the x_monotone_chain_ data member.
  void GenerateConvexHull();
  // The orientation predicate function taking three Points as parameters.
  bool IsOrientationNegative(const Point2D&, const Point2D&, const Point2D&) const;
};
} // End namespace planargeometry
#include "planar_convex_hull_defs.h"

#endif // PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_H_