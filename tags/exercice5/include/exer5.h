#ifndef __EXER_5__
#define __EXER_5__

#include "point.h"

struct SquareContainer
{
	double up;
	double bottom;
	double right;
	double left;
};

struct TriangleContainer
{
	Point a;
	Point b;
	Point c;
};

SquareContainer getSquareContainer();
TriangleContainer getTriangleContainer();
void makeTriangulation(bool delaunay, bool unwantedTriangles); // Make triangulation

#endif