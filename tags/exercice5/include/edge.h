
#ifndef __EDGE_H__
#define __EDGE_H__

#include "face.h"
#include "vertex.h"
#include "point.h"

class Face;
class Vertex;

class Edge
{
 private:
	double det(const Point& a, const Point& b, const Point& c);
	int signDet(const Point& a, const Point& b, const Point& c);
 
 public:
	Vertex* vertex;
	Edge* nextEdge;
	Edge* twinEdge;
	Face* face;
	bool drawable;
	
	Edge();
	void set(Vertex* v, Edge* ne, Edge* te, Face* f);
	bool isOnLeft(int pointIndex);
};

#endif
