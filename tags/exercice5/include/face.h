
#ifndef __FACE_H__
#define __FACE_H__

#include "edge.h"
#include "triangle.h"

class Edge;
class Vertex;

class Face
{
 private:
	double det(const Point& a, const Point& b, const Point& c);
	int signDet(const Point& a, const Point& b, const Point& c);
 
 public:
	Edge* edge;
	bool isInfinite;
	
	Face(Edge* e);
	void setInfinite();
	Triangle getTriangle();
	bool isInsideCircle(Vertex* v);
};

#endif
