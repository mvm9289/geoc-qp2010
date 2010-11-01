#ifndef __TRIANGULATION__
#define __TRIANGULATION__

#include "polygon.h"
#include <vector>
#include "node.h"
using namespace std;

#define WITHOUT_EXTERN_LINES // Comment this line to see all lines of triangulation

class Triangulation
{
	private:
		vector<Polygon> triangles;  // List of triangles result of triangulation
		Node triangulation;         // First node of the tree
		
		Node* newNode(const Point& a, const Point& b, const Point& c, Polygon* p);  // Make a new node of the tree
		Polygon* newTriangle(const Point& a, const Point& b, const Point& c);  // Make a new element of list of triangles result of triangulation
		void addRecursive(Node* n, const Point& p); // Add the point p to the structure
	
	public:
		Triangulation();    // Costruct an empty triangulation structure
		Triangulation(const Point& a, const Point& b, const Point& c);  // Costruct a triangulation structure with triangle container
		void add(const Point& p);   // Add the point p to the structure
		vector<Polygon> getTriangles(); // Get the list of triangles result of triangulation
};

#endif
