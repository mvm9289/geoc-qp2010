#ifndef __EXER_4__
#define __EXER_4__

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
void exer4execute(); // Make triangulation

#endif