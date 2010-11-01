
#include "edge.h"
#include "data.h"

Edge :: Edge() : vertex(0), nextEdge(0), twinEdge(0), face(0) , valid(true){}

void Edge :: set(Vertex* v, Edge* ne, Edge* te, Face* f)
{
	vertex = v;
	nextEdge = ne;
	twinEdge = te;
	face = f;
}

bool Edge :: isOnRight(int pointIndex)
{
	Point a = points[vertex->pointIndex];
	Point b = points[nextEdge->vertex->pointIndex];
	Point p = points[pointIndex];
	
	return b.x*p.y + a.x*b.y + a.y*p.x - b.x*a.y - p.x*b.y - a.x*p.y > 0.0;
}
