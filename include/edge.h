
#ifndef __EDGE_H__
#define __EDGE_H__

#include "face.h"
#include "vertex.h"

class Face;
class Vertex;

class Edge
{
 public:
	Vertex* vertex;
	Edge* nextEdge;
	Edge* twinEdge;
	Face* face;
	bool valid;
	
	Edge();
	void set(Vertex* v, Edge* ne, Edge* te, Face* f);
	bool isOnRight(int pointIndex);
};

#endif
