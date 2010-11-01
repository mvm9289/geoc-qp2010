
#include "face.h"
#include "data.h"

Face :: Face (Edge* e) : isInfinite(false)
{
	edge = e;
}

void Face :: setInfinite()
{
	isInfinite = true;
}

Triangle Face :: getTriangle()
{
	Edge* e1 = edge;
	Edge* e2 = e1->nextEdge;
	Edge* e3 = e2->nextEdge;
	
	int a = e1->vertex->pointIndex;
	int b = e2->vertex->pointIndex;
	int c = e3->vertex->pointIndex;
	
	int notDrawable = 0;
	if (!(e1->drawable)) notDrawable++;
	if (!(e2->drawable)) notDrawable += 2;
	if (!(e3->drawable)) notDrawable += 4;
	
	switch (notDrawable)
	{
		case 3:
			b = c;
			break;
		case 5:
			a = b;
			break;
		case 6:
			c = a;
			break;
		case 7:
			b = a;
			c = a;
			break;
		default:
			break;
	}
	
	return Triangle(a, b, c);
}

bool Face :: isInsideCircle(Vertex* v)
{
	if (isInfinite) return false;
	
	Edge* e1 = edge;
	Edge* e2 = e1->nextEdge;
	Edge* e3 = e2->nextEdge;
	
	Point a = points[e1->vertex->pointIndex];
	a.z = 1;
	Point b = points[e2->vertex->pointIndex];
	b.z = 1;
	Point c = points[e3->vertex->pointIndex];
	c.z = 1;
	Point p = points[v->pointIndex];
	
	Point aux1( (b.x - a.x), (c.x - a.x), (p.x - a.x));
	Point aux2( (b.y - a.y), (c.y - a.y), (p.y - a.y));
	Point aux3( ((b.x - a.x)*(b.x + a.x) + (b.y - a.y)*(b.y + a.y)),
			((c.x - a.x)*(c.x + a.x) + (c.y - a.y)*(c.y + a.y)),
			((p.x - a.x)*(p.x + a.x) + (p.y - a.y)*(p.y + a.y)));

	int sdet = signDet(aux1, aux2, aux3)*signDet(a, b, c);
	
	return (sdet <= 0);
}

double Face :: det(const Point& a, const Point& b, const Point& c)
{
    return b.x*c.y*a.z - c.x*b.y*a.z + c.x*a.y*b.z - a.x*c.y*b.z + a.x*b.y*c.z - b.x*a.y*c.z;
}

int Face :: signDet(const Point& a, const Point& b, const Point& c)
{
    double d = det(a, b, c);
    
    if (d == 0.0) return 0;
    if (d < 0.0) return -1;
    
    return 1;
}
