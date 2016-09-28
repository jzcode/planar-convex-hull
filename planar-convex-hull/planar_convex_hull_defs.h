/*Author: Jason Zadwick. Code Modified: Summer 2016.*/

#ifndef PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_DEFS_H_
#define PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_DEFS_H_
#include "stdafx.h"

namespace planargeometry {
std::ostream& operator<<(std::ostream& os, const Point2D& p) {
  os << std::right << '(' << std::setw(12) << p.x_ << ", " << std::setw(12) << p.y_ << ')';
  return os;
}

template<class Compare>
ConvexHull<Compare>::ConvexHull(int n, float a, float b, Compare&& strict_weak_ordering)
  : x_monotone_chain_(strict_weak_ordering) {
  std::ios cout_original_fmt{ nullptr };
  cout_original_fmt.copyfmt(std::cout);
  cout.precision(std::numeric_limits<float>::digits10 - 1);
  cout.setf(std::ios::scientific);
  for (int i = 0; i < n; ++i) {
    x_monotone_chain_.emplace(a, b);
  }
  std::cout << "\n> Input:\n\nDATA STRUCTURE:\n X-Monotone Polygonal Chain\n" << std::left << std::setw(11)
    << "Size" << ": " << (x_monotone_chain_.size()) << '\n' << std::setw(11)
    << "Container" << ": " << "std::set\n" << std::setw(11)
    << "Element" << ": " << "unique vertices of the polygonal chain\n" << std::setw(11)
    << "Comparator" << ": strict-weak-ordering binary-predicate inducing a lexicographic ordering on the set";
  if (x_monotone_chain_.size() == size_t{ 1 }) {
    std::cout << "\n\nA " << (x_monotone_chain_.size()) << " element set is exactly the vertex set of its planar convex hull."
      "\n\n> Output:\n\nDATA STRUCTURE:\n Planar Convex Hull\n\n";
    std::cout << std::right << std::setw(4) << "P0" << std::left << std::setw(3) << ':' << (*x_monotone_chain_.begin()) << "\n\n";
    cout.copyfmt(cout_original_fmt);
  } else if (x_monotone_chain_.size() == size_t{ 2 }) {
    std::cout << "\n\nA " << (x_monotone_chain_.size()) << " element set is exactly the vertex set of its planar convex hull."
      "\n\n> Output:\n\nDATA STRUCTURE:\n Planar Convex Hull Extreme Points\nFormatting : -displaytype = convex_hull_vertices\n"
      "             -displaymode = cw_enumeration\n             -minimalvertex = (*)\n             -maximalvertex = (**)\n\n";
    std::cout << std::right << std::setw(4) << "*P0" << std::left << std::setw(3) << ':' << (*x_monotone_chain_.begin()) << "\n";
    std::cout << std::right << "**P1" << std::left << std::setw(3) << ':' << (*++x_monotone_chain_.begin()) << "\n\n";
    cout.copyfmt(cout_original_fmt);
  } else {
    GenerateConvexHull();
    PrintHull();
    cout.copyfmt(cout_original_fmt);
  }
}

template<class Compare>
void ConvexHull<Compare>::PrintHull() const {
  std::cout << "\n\n> Output:\n\nDATA STRUCTURE:\n Planar Convex Hull Extreme Points\n" << std::left << std::setw(11)
    << "Size" << ": " << (convex_hull_.size()) << '\n' << std::setw(11)
    << "Container" << ": " << "std::vector\n" << std::setw(11)
    << "Element" << ": " << "vertices of the convex hull\n"
    "Print Mode : -traversal     = clockwise_enumeration\n"
    "             -minimalvertex = (*)\n"
    "             -maximalvertex = (**)\n\n";
  int i{ 0 }, width{ 0 }, n{ static_cast<int>(convex_hull_.size()) };
  while (n) {
    ++width;
    n /= 10;
  }
  for (const auto& elt : convex_hull_) {
    if (&elt == &convex_hull_[0]) {
      std::cout << std::right << std::setw(7) << "*P" << std::left << std::setw(width) << (i++) << " = " << elt << '\n';
    } else if (&elt == &convex_hull_.at(maximal_hull_vertex_index_)) {
      std::cout << std::right << std::setw(7) << "**P" << std::left << std::setw(width) << (i++) << " = " << elt << '\n';
    } else {
      std::cout << std::right << std::setw(7) << "P" << std::left << std::setw(width) << (i++) << " = " << elt << '\n';
    }
  }
  std::cout << "\nDESCRIPTION OF THE PLANAR CONVEX HULL:"
    "\nLet the initial set of Points be denoted by P. The planar convex hull is stored in the ConvexHull::convex_hull_ data member, and it is a subset of P, by way"
    "\nof construction. But Andrew's algorithm forms this subset by first adding the upper convex hull, then it proceeds to add the lower convex hull, so that the subset is also"
    "\nthe entire planar convex hull. Hence, the data member will contain an unambiguous representation of the planar convex hull in the form of a carefully chosen subset of P."
    "\nFollowing the implementation details of Andrew's algorithm, formation of the subset depends on the ordering on P. Thus, a lexicographical ordering on x_monotone_chain_ is"
    "\ninduced by a custom comparator lambda function passed to the constructor. Then convex_hull_ is repeatedly updated to contain the current candidate subset of P representing"
    "\nthe planar convex hull--based on the information available at the most recent Point2D visited during traversal of x_monotone_chain_."
    "\n\nThe final state of convex_hull_ correctly represents the planar convex hull by its extreme points. convex_hull_ will then comprise a subset of P that are"
    "\nalso the vertices of a convex polygon containing every element of the initial set, P. Furthermore, this convex polygon is"
    "\nuniquely defined, and it is the intersection of all convex sets containing P.\n\n";
}

// Launch two threads with lambda functions for creating the upper and lower hulls. Current thread synchronizes with both then appends
// the lower hull to the data structure containing the upper hull, i.e., the final result is stored in the convex_hull_ data member.
template<class Compare>
void ConvexHull<Compare>::GenerateConvexHull() {
  std::cout << "\n\nExecuting incremental convex hull construction by the monotone chain algorithm.";
  std::vector<Point2D> lower_hull(x_monotone_chain_.rbegin(), ++(++x_monotone_chain_.rbegin()));

  std::cout << "\nConstructing upper hull.";
  std::thread up_thread([this]()->void {
    // The next index at which to modify the upper hull.
    int k{ 0 };
    // Time Complexity of this for statement: O(n), where  n is the size of x_monotone_chain_. Any element of x_monotone_chain_ is
    // accessed at most twice in the for statement.
    for (auto it = x_monotone_chain_.begin(); it != x_monotone_chain_.end(); ++k, ++it) {
      while (k >= 2 && IsOrientationNegative(convex_hull_[k - 2], convex_hull_[k - 1], *it) == false) {
        convex_hull_.pop_back();
        --k;
      }
      convex_hull_.push_back(*it);
    }
    // Remove last point to avoid duplication of maximal convex hull vertex.
    convex_hull_.pop_back();
    maximal_hull_vertex_index_ = convex_hull_.size();
  });
  std::cout << "\nConstructing lower hull.";
  std::thread low_thread([this, &lower_hull]()->void {
    // The next index at which to modify the upper hull.
    int k{ 2 };
    // Guards against invalid calls to the orientation predicate function.
    const int j{ 2 };
    // Time Complexity of this for statement: O(n), where  n is the size of x_monotone_chain_. Any element of x_monotone_chain_ is
    // accessed at most twice in the for statement.
    for (auto rit = ++(++x_monotone_chain_.rbegin()); rit != x_monotone_chain_.rend(); ++k, ++rit) {
      while (k >= j && IsOrientationNegative(lower_hull[k - 2], lower_hull[k - 1], *rit) == false) {
        lower_hull.pop_back();
        --k;
      }
      lower_hull.push_back(*rit);
    }
    // Remove last point to avoid duplication of minimal convex hull vertex.
    lower_hull.pop_back();
  });
  // Synchronize the current thread with both worker threads.
  up_thread.join();
  low_thread.join();
  // Current thread appends lower_hull to convex_hull_, and convex_hull_ attains its final state.
  convex_hull_.reserve(convex_hull_.size() + lower_hull.size());
  for (auto it = lower_hull.begin(); it != lower_hull.end(); ++it) {
    convex_hull_.push_back(*it);
  }
  convex_hull_.shrink_to_fit();
  std::cout << "\nConvex hull is completely constructed!";
}

// Returns true to indicate a negative orientation.
// Returns false to indicate a positive orientation or the Points are collinear.
// Although this program considers only two-dimensional Points, the third-component of the cross 
// product can still be evaluated since it depends solely on the first two coordinates of the
// points passed as method parameters.	Furthermore, this implementation of the cross product returns 
// an orientation for Points located in a positively oriented coordinate system.
template<class Compare>
bool ConvexHull<Compare>::IsOrientationNegative(const Point2D& p, const Point2D& q, const Point2D& r) const {
  return ((q.XToDouble() - p.XToDouble()) * (r.YToDouble() - p.YToDouble())) < ((q.YToDouble() - p.YToDouble()) * (r.XToDouble() - p.XToDouble())) ? true : false;
}
} // End namespace planargeometry

#endif // PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_PLANAR_CONVEX_HULL_DEFS_H_
