
#ifndef __TRIANGULATION_H__
#define __TRIANGULATION_H__

#include "face.h"
#include "edge.h"
#include "vertex.h"
#include "triangle.h"
#include <vector>
using namespace std;


typedef vector<Face*> Faces;
typedef Faces :: iterator FacesIterator;

typedef vector<Edge*> Edges;
typedef Edges :: iterator EdgesIterator;

typedef vector<Vertex*> Vertices;
typedef Vertices :: iterator VerticesIterator;

typedef vector<Triangle> Triangles;


class Triangulation
{
 private:
	int containerA, containerB, containerC; // Triangle container vertices
	bool delaunay;
	
	Face* searchLocationFace(int pointIndex);
	void makeDelaunay(Vertex* v);
	void removeAllEdges(int pointIndex);
	void removeExternalEdges(int poinIndex, int nextPointIndex, int previousPointIndex);
	double det(const Point& a, const Point& b, const Point& c);
	int signDet(const Point& a, const Point& b, const Point& c);
 
 public:
	Faces faces;
	Edges edges;
	Vertices vertices;
	
	Triangulation(int a, int b, int c, bool D);
	void addPoint(int pointIndex);
	void deleteUnwantedTriangles();
	Triangles getTriangles();
};

#endif
