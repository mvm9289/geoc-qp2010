
#include "triangulation.h"
#include "triangle.h"
#include "data.h"
#include <stack>

Triangulation :: Triangulation(int a, int b, int c, bool D)
{
	containerA = a;
	containerB = b;
	containerC = c;
	delaunay = D;
	
	Edge* e1 = new Edge();
	edges.push_back(e1);
	Edge* e2 = new Edge();
	edges.push_back(e2);
	Edge* e3 = new Edge();
	edges.push_back(e3);
	
	Edge* e1twin = new Edge();
	Edge* e2twin = new Edge();
	Edge* e3twin = new Edge();
	
	Vertex* v1 = new Vertex(a, e1);
	vertices.push_back(v1);
	Vertex* v2 = new Vertex(b, e2);
	vertices.push_back(v2);
	Vertex* v3 = new Vertex(c, e3);
	vertices.push_back(v3);
	
	Face* f = new Face(e1);
	faces.push_back(f);
	Face* infinite = new Face(e1twin);
	infinite->setInfinite();
	
	e1->set(v1, e2, e1twin, f);
	e2->set(v2, e3, e2twin, f);
	e3->set(v3, e1, e3twin, f);
	
	e1twin->set(v2, e3twin, e1, infinite);
	e2twin->set(v3, e1twin, e2, infinite);
	e3twin->set(v1, e2twin, e3, infinite);
}

void Triangulation :: addPoint (int pointIndex)
{
	Edge* e1new = new Edge();
	edges.push_back(e1new);
	Edge* e1newTwin = new Edge();
	edges.push_back(e1newTwin);
	
	Edge* e2new = new Edge();
	edges.push_back(e2new);
	Edge* e2newTwin = new Edge();
	edges.push_back(e2newTwin);
	
	Edge* e3new = new Edge();
	edges.push_back(e3new);
	Edge* e3newTwin = new Edge();
	edges.push_back(e3newTwin);
	
	Vertex* vNew = new Vertex(pointIndex, e1newTwin);
	vertices.push_back(vNew);
	
	Face* locatedFace = searchLocationFace(pointIndex);
	
	Edge* e1 = locatedFace->edge;
	Edge* e2 = e1->nextEdge;
	Edge* e3 = e2->nextEdge;

	Face* f1 = new Face(e2);
	faces.push_back(f1);
	Face* f2 = new Face(e3);
	faces.push_back(f2);

	e1->set(e1->vertex, e2new, e1->twinEdge, locatedFace);
	e2->set(e2->vertex, e3new, e2->twinEdge, f1);
	e3->set(e3->vertex, e1new, e3->twinEdge, f2);
	
	e1new->set(e1->vertex, e3newTwin, e1newTwin, f2);
	e2new->set(e2->vertex, e1newTwin, e2newTwin, locatedFace);
	e3new->set(e3->vertex, e2newTwin, e3newTwin, f1);
	
	e1newTwin->set(vNew, e1, e1new, locatedFace);
	e2newTwin->set(vNew, e2, e2new, f1);
	e3newTwin->set(vNew, e3, e3new, f2);
	
	if (delaunay) makeDelaunay(vNew);
}

void Triangulation :: deleteUnwantedTriangles()
{
	int n = boundary.size();
	for (int i = 0; i < n; i++)
	{
		if (boundary[i].size() == 1) removeAllEdges(boundary[i][0]);
		else
		{
			int m = boundary[i].size();
			for (int k = 0; k < m; k++)
				removeExternalEdges(boundary[i][k],
					boundary[i][(k + m + 1)%m],
					boundary[i][(k + m - 1)%m]);
		}
	}
	
	removeAllEdges(containerA);
	removeAllEdges(containerB);
	removeAllEdges(containerC);
}

Triangles Triangulation :: getTriangles()
{
	Triangles triangles_;
	
	for (FacesIterator i = faces.begin(); i != faces.end(); i++)
		triangles_.push_back((*i)->getTriangle());
	
	return triangles_;
}

Face* Triangulation :: searchLocationFace(int pointIndex)
{
	Edge* e = edges.front();
	
	bool found = false;
	while (!found) {
		Edge* end = e;
		while (e->isOnLeft(pointIndex) && e->nextEdge != end)
			e = e->nextEdge;
		if (e->isOnLeft(pointIndex)) found = true;
		else e = e->twinEdge;
	}
	
	return e->face;
}

void Triangulation :: makeDelaunay(Vertex* v)
{
	stack<Face*> incidentFaces;
	Edge* e = v->edge;
	Edge* end = e;
	do
	{
		incidentFaces.push(e->face);
		e = e->nextEdge->nextEdge->twinEdge;
	} while (e != end);
	
	while(!incidentFaces.empty())
	{
		Face* incidentFace = incidentFaces.top();
		incidentFaces.pop();
		
		for (e = incidentFace->edge; e->vertex != v; e = e->nextEdge) {};
		
		Face* adjacentFace = e->nextEdge->twinEdge->face;
		if (adjacentFace->isInsideCircle(v))
		{
			Edge* e1iFace = e;
			Edge* e2iFace = e1iFace->nextEdge;
			Edge* e3iFace = e2iFace->nextEdge;
			
			Edge* e1aFace = e2iFace->twinEdge;
			Edge* e2aFace = e1aFace->nextEdge;
			Edge* e3aFace = e2aFace->nextEdge;
			
			incidentFace->edge = e1iFace;
			adjacentFace->edge = e3aFace;
			
			e2aFace->face = incidentFace;
			e3iFace->face = adjacentFace;
			
			e2iFace->vertex = e3aFace->vertex;
			e1aFace->vertex = e1iFace->vertex;
			
			e1iFace->nextEdge = e2aFace;
			e2aFace->nextEdge = e2iFace;
			e2iFace->nextEdge = e1iFace;
			
			e3aFace->nextEdge = e3iFace;
			e3iFace->nextEdge = e1aFace;
			e1aFace->nextEdge = e3aFace;
			
			e2aFace->vertex->edge = e2aFace;
			e3iFace->vertex->edge = e3iFace;
			
			incidentFaces.push(incidentFace);
			incidentFaces.push(adjacentFace);
		}
	}
}

void Triangulation :: removeAllEdges(int pointIndex)
{
	VerticesIterator i;
	for (i = vertices.begin(); (*i)->pointIndex != pointIndex; i++) {};
	Edge* e = (*i)->edge;
	Edge* end = e;
	do
	{
		e->drawable = false;
		e = e->nextEdge->nextEdge;
		e->drawable = false;
		e = e->twinEdge;
	} while (e != end);
}

void Triangulation :: removeExternalEdges(int pointIndex, int nextPointIndex, int previousPointIndex)
{
	Point it = points[pointIndex];
	Point next = points[nextPointIndex];
	Point prev = points[previousPointIndex];
	
	VerticesIterator i;
	for (i = vertices.begin(); (*i)->pointIndex != pointIndex; i++) {};
	Edge* e = (*i)->edge;
	Edge* end = e;
	do
	{
		Edge* aux = e->nextEdge->nextEdge;
		Point paux = points[e->nextEdge->vertex->pointIndex];
		if (signDet(next, it, paux) > 0 || signDet(it, prev, paux) > 0)
		{
			e->drawable = false;
			e->twinEdge->drawable = false;
		}
		e = aux->twinEdge;
	} while (e != end);
}

double Triangulation :: det(const Point& a, const Point& b, const Point& c)
{
	return b.x*c.y + a.x*b.y + a.y*c.x - b.x*a.y - c.x*b.y - a.x*c.y;
}

int Triangulation :: signDet(const Point& a, const Point& b, const Point& c)
{
    double d = det(a, b, c);
    
    if (d == 0.0) return 0;
    if (d < 0.0) return -1;
    
    return 1;
}
