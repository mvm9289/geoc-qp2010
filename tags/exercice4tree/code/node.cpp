#include "node.h"

Node :: Node()
{
	triangle = NULL;
}

bool Node :: isInside(const Point& p)
{
	int sdet12p = signDet(points[0], points[1], p);
	int sdet23p = signDet(points[1], points[2], p);
	int sdet31p = signDet(points[2], points[0], p);
	
	return (sdet12p == sdet23p && sdet23p == sdet31p);
}
	
double Node :: det(const Point& a, const Point& b, const Point& c)
{
	return b.x*c.y - c.x*b.y + c.x*a.y - a.x*c.y + a.x*b.y - b.x*a.y;
}

int Node :: signDet(const Point& a, const Point& b, const Point& c)
{
	double d = det(a, b, c);
	
	if (d == 0) return 0;
	else if (d < 0) return -1;
	else return 1;
}


