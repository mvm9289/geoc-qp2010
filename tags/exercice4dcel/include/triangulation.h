
#ifndef __TRIANGULATION_H__
#define __TRIANGULATION_H__

#include "face.h"
#include "edge.h"
#include "vertex.h"
#include "triangle.h"
#include <vector>
using namespace std;


typedef vector<Face*> Faces;
typedef Faces::iterator FacesIterator;

typedef vector<Edge*> Edges;
typedef Edges::iterator EdgesIterator;

typedef vector<Vertex*> Vertices;
typedef Vertices::iterator VerticesIterator;

typedef vector<Triangle> Triangles;


class Triangulation
{
 private:
	Face* searchLocationFace(int pointIndex);
 
 public:
	Faces faces;
	Edges edges;
	Vertices vertices;
	
	Triangulation(int a, int b, int c);
	void addPoint(int pointIndex);
	void deleteUnwantedTriangles();
	Triangles getTriangles();
};

#endif
