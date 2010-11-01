
#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "edge.h"

class Edge;

class Vertex
{
 public:
	int pointIndex;
	Edge* edge;
	
	Vertex(int pi, Edge* e);
};

#endif
