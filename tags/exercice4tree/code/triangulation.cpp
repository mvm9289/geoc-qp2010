#include "triangulation.h"

Triangulation :: Triangulation()
{
	triangulation.triangle = NULL;
}

Triangulation :: Triangulation(const Point& a, const Point& b, const Point& c)
{
	Polygon* aux = newTriangle(a, b, c);
    #ifndef WITHOUT_EXTERN_LINES
        triangles.push_back(*aux);
    #endif
	
	triangulation.points.push_back(a);
	triangulation.points.push_back(b);
	triangulation.points.push_back(c);
	triangulation.triangle = aux;
}

void Triangulation :: add(const Point& p)
{
	addRecursive(&triangulation, p);
}

vector<Polygon> Triangulation :: getTriangles()
{
	return triangles;
}

Node* Triangulation :: newNode(const Point& a, const Point& b, const Point& c, Polygon* p)
{
	Node* aux = new Node;
	aux->points.push_back(a);
	aux->points.push_back(b);
	aux->points.push_back(c);
	aux->triangle = p;
	
	return aux;
}

Polygon* Triangulation :: newTriangle(const Point& a, const Point& b, const Point& c)
{
	vector<Point> vertices;
	vertices.push_back(a);
	vertices.push_back(b);
	vertices.push_back(c);
	
	return new Polygon(vertices);
}

void Triangulation :: addRecursive(Node* n, const Point& p)
{
	if (n->subTriangles.size() == 0)
	{
		n->subTriangles.push_back(newNode(n->points[0], n->points[1], p, n->triangle));
		n->triangle->points[2] = p;
		n->triangle = NULL;
		
        Point aux0 = n->points[0];
        #ifdef WITHOUT_EXTERN_LINES
            if (aux0 == triangulation.points[0] || aux0 == triangulation.points[1] || aux0 == triangulation.points[2]) aux0 = p;
        #endif
        Point aux1 = n->points[1];
        #ifdef WITHOUT_EXTERN_LINES
            if (aux1 == triangulation.points[0] || aux1 == triangulation.points[1] || aux1 == triangulation.points[2]) aux1 = p;
        #endif
        Point aux2 = n->points[2];
        #ifdef WITHOUT_EXTERN_LINES
            if (aux2 == triangulation.points[0] || aux2 == triangulation.points[1] || aux2 == triangulation.points[2]) aux2 = p;
        #endif
        
		Polygon* aux = newTriangle(aux1, aux2, p);
		triangles.push_back(*aux);
		n->subTriangles.push_back(newNode(n->points[1], n->points[2], p, aux));
		
		aux = newTriangle(aux2, aux0, p);
		triangles.push_back(*aux);
		n->subTriangles.push_back(newNode(n->points[2], n->points[0], p, aux));
	}
	else if (n->subTriangles[0]->isInside(p)) addRecursive(n->subTriangles[0], p);
	else if (n->subTriangles[1]->isInside(p)) addRecursive(n->subTriangles[1], p);
	else addRecursive(n->subTriangles[2], p);
}
