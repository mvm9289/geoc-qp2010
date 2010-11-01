#ifndef __NODE__
#define __NODE__

#include "point.h"
#include "polygon.h"
#include <vector>
using namespace std;

class Node
{
	public:
		vector<Point> points;       // Triangle representation
		Polygon* triangle;          // Pointer to the list of triangles result of triangulation
		vector<Node*> subTriangles; // Internal triangles
		
		Node();
		bool isInside(const Point& p); // Return if the point p is inside of this triangle
	
	private:
		double det(const Point& a, const Point& b, const Point& c);
		int signDet(const Point& a, const Point& b, const Point& c);
};

#endif
