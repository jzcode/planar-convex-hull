#  A program that computes the convex hull of a finite set of two-dimensional Points by Andrew's monotone chain algorithm.

### Steps to Build and Run planar-convex-hull from a cloned repository
Obtain the file at the following relative path: *./planar-convex-hull.sln* and Open it with Visual Studio 2015,
then use the keyboard shortcuts: Ctrl-Shift-B (to Build) and Ctrl-F5 (to Run), or simply Ctrl-F5 if it is setup
in your Visual Studio 2015 environment by selecting the 'Always build' menu-item from the
'On Run, when projects are out of date' dropdown menu bar located at
*Tools > Options > Projects and Solutions > Build and Run* interface.

### Relative path to planar-convex-hull source files
The relative path to the source files is *./planar-convex-hull*.

### planar-convex-hull explained
A program that computes the planar convex hull of a finite set of two-dimensional Points. A monotone chain is created
in time complexity that is linearithmic in the size of the initial random sample size, namely the user input value
stored in variable n. Two threads are launched to compute the upper and lower hulls in the implementation of Andrew's
monotone chain algorithm. Each std::threads' initial lambda function computes their respective portion of the convex hull
in time complexity that is linear in the size of the planargeometry::ConvexHull::x_monotone_chain_ local data member of
the planargeometry::ConvexHull object instantiated by the main thread.
