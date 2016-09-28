/*
Author: Jason Zadwick. Code Modified: Summer 2016.

SUMMARY:
A program that computes the planar convex hull of a finite set of two-dimensional Points. A monotone chain is created in time complexity
linearithmic in the size of the initial random sample size, namely the user input value stored in variable n. Two threads are launched
to compute the upper and lower hulls in the implementation of Andrew's monotone chain algorithm. Both threads' starting lambda function
computes their respective portion of the convex hull in time complexity linear in the size of the x_monotone_chain_ local data member
of the planargeometry::ConvexHull object.

CONFIGURATION:
Tested in Visual Studio Community 2015, as a 'Win32 Console Application', with x64 settings and precompiled header file,
under the Windows 10 runtime configuration.

REFERENCES:
DE BERG, M., CHEONG, O., VAN KREVELD, M., OVERMARS, M.: "Computational Geometry: Algorithms and Applications," 3rd ed., Springer-Verlag Berlin Heidelberg, 2008.

Pichardie, David, and Yves Bertot. "Formalizing convex hull algorithms." International Conference on Theorem Proving in Higher Order Logics.
Springer Berlin Heidelberg, 2001.

STROUSTRUP, B. (2014). "The C++ Programming Language," 4th edition. Addison-Wesley Professional.

WILLIAMS, A.: "C++ Concurrency in Action: Practical Multithreading," Manning Publications Co: 2012.
*/
#include "stdafx.h"
#include "planar_convex_hull.h"
using std::cout;
using std::cin;
using std::endl;

int main() {
  int n;
  float a, b;
  cout << "***COMPUTE THE PLANAR CONVEX HULL OF A FINITE SET OF POINTS***"
    "\n\nINITIAL CONDITIONS:"
    "\nAndrew's monotone chain algorithm computes the planar convex hull of an initial finite set of randomly generated Points."
    "\nEnter the size of the initial set and the distribution range bounds for generating two dimensional coordinates."
    "\n\nEnter the number of points to be generated and inserted into a std::set, followed by the lower bound and the upper bound"
    "\nwhich define the acceptable range of uniformly distributed values that a point's coordinates can take."
    "\nWhere the size must be a nonnegative integral value representable by an int, and the lower value must be less than the upper value."
    "\nUSAGE: <size : int> <lower : float> <upper : float>"
    "\nEXAMPLE: 235 -9347 3.4887e+05"
    "\nEnter the values for each of the three parameters separated by a single space:\n";
  do { // Validate the user input.
    while (!(cin >> n)) {
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      cout << "Invalid size: first value not representable by an int data type.\nUSAGE:  <size : int> <lower : float> <upper : float>\n";
    }
    if (n < 0) {
      cout << "Invalid size: first value is negative.\nUSAGE: <size : int> <lower : float> <upper : float>\n";
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else if (n == 0) {
      cout << "\nThe empty set is the trivial convex hull and no computation is needed.\n";
      return 0;
    } else if (!(cin >> a) || !(cin >> b)) {
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      cout << "Invalid range bound(s): second or third value is not representable by a float data type.\nUSAGE: <size : int> <lower : float> <upper : float>\n";
    } else if (a >= b) {
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      cout << "Invalid range bounds: distribution range is of the form [lower, upper) and the lower value must be less than the upper value."
        "\nUSAGE: <size : int> <lower : float> <upper : float>\n";
    } else {
      break;
    }
  } while (true);
  // Default construct here, then move assign an anonymous initialization-constructed object to it in the try/catch block so
  // this object remains in scope of main() for later usage.
  planargeometry::ConvexHull<> convexenvelope;
  try {
    convexenvelope = planargeometry::ConvexHull<>{n, a, b, [](const planargeometry::Point2D& l, const planargeometry::Point2D& r) -> bool {
      if (l.GetX() == r.GetX()) {
        return l.GetY() < r.GetY() ? true : false;
      } else {
        return l.GetX() < r.GetX() ? true : false;
      }
    }};
  } catch (const std::out_of_range& e) {
    cout << "Caught in main: " << e.what() << '\n';
    return -1;
  } catch (const std::exception& e) {
    cout << "Caught in main: " << e.what() << '\n';
    return -1;
  }
  return 0;
}