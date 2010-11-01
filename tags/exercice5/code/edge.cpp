
#include "edge.h"
#include "data.h"

Edge :: Edge() : vertex(0), nextEdge(0), twinEdge(0), face(0) , drawable(true){}

void Edge :: set(Vertex* v, Edge* ne, Edge* te, Face* f)
{
	vertex = v;
	nextEdge = ne;
	twinEdge = te;
	face = f;
}

bool Edge :: isOnLeft(int pointIndex)
{
	Point a = points[vertex->pointIndex];
	Point b = points[nextEdge->vertex->pointIndex];
	Point p = points[pointIndex];
	
	return signDet(a, b, p) > 0;
}

double Edge :: det(const Point& a, const Point& b, const Point& c)
{
	return b.x*c.y + a.x*b.y + a.y*c.x - b.x*a.y - c.x*b.y - a.x*c.y;
}

int Edge :: signDet(const Point& a, const Point& b, const Point& c)
{
    double d = det(a, b, c);
    
    if (d == 0.0) return 0;
    if (d < 0.0) return -1;
    
    return 1;
}
