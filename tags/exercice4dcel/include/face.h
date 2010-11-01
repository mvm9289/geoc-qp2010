
#ifndef __FACE_H__
#define __FACE_H__

#include "edge.h"
#include "triangle.h"

class Edge;

class Face
{
 public:
	Edge* edge;
	
	Face(Edge* e);
	Triangle getTriangle();
};

#endif
