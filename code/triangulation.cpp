
#include "triangulation.h"
#include "triangle.h"
#include "data.h"

Triangulation :: Triangulation(int a, int b, int c)
{
	Edge* e1 = new Edge();
	edges.push_back(e1);
	Edge* e2 = new Edge();
	edges.push_back(e2);
	Edge* e3 = new Edge();
	edges.push_back(e3);
	
	Vertex* v1 = new Vertex(a, e1);
	vertices.push_back(v1);
	Vertex* v2 = new Vertex(b, e2);
	vertices.push_back(v2);
	Vertex* v3 = new Vertex(c, e3);
	vertices.push_back(v3);
	
	Face* f = new Face(e1);
	faces.push_back(f);
	
	e1->set(v1, e2, e1, f);
	e2->set(v2, e3, e2, f);
	e3->set(v3, e1, e3, f);
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
}

void Triangulation :: deleteUnwantedTriangles()
{
	int n = boundary.size();
	for (int i = 0; i < n; i++)
	{
		if (boundary[i].size() == 1)
		{
			int pointIndex = boundary[i][0];
			VerticesIterator j;
			for (j = vertices.begin(); (*j)->pointIndex != pointIndex; j++) {};
			Edge* e = (*j)->edge;
			Edge* end = e;
			do
			{
				e->valid = false;
				e = e->nextEdge->nextEdge;
				e->valid = false;
				e = e->twinEdge;
			} while (e != end);
		}
		else
		{
			int m = boundary[i].size();
			for (int k = 0; k < m; k++)
			{
				Point next = points[boundary[i][(k + m + 1)%m]];
				Point prev = points[boundary[i][(k + m - 1)%m]];
				Point it = points[boundary[i][k]];
				
				VerticesIterator j;
				for (j = vertices.begin(); (*j)->pointIndex != boundary[i][k]; j++) {};
				Edge* e = (*j)->edge;
				Edge* end = e;
				do
				{
					Edge* aux = e->nextEdge->nextEdge;
					Point paux = points[e->nextEdge->vertex->pointIndex];
					if (it.x*paux.y + next.x*it.y + next.y*paux.x - it.x*next.y - paux.x*it.y - next.x*paux.y > 0.0
						|| prev.x*paux.y + it.x*prev.y + it.y*paux.x - prev.x*it.y - paux.x*prev.y - it.x*paux.y > 0.0)
					{
						e->valid = false;
						e->twinEdge->valid = false;
					}
					e = aux->twinEdge;
				} while (e != end);
			}
		}
	}
	
	for (int i = 1;  i <= 3; i++)
	{
		int pointIndex = points.size() - i;
		VerticesIterator j;
		for (j = vertices.begin(); (*j)->pointIndex != pointIndex; j++) {};
		Edge* e = (*j)->edge;
		Edge* end = e;
		do
		{
			e->valid = false;
			e = e->nextEdge->nextEdge;
			e->valid = false;
			e = e->twinEdge;
		} while (e != end);
	}
}

Triangles Triangulation :: getTriangles()
{
	Triangles triangles2;
	
	for (FacesIterator i = faces.begin(); i != faces.end(); i++)
		triangles2.push_back((*i)->getTriangle());
	
	return triangles2;
}

Face* Triangulation :: searchLocationFace(int pointIndex)
{
	Edge* e = edges.front();
	
	bool found = false;
	while (!found) {
		Edge* end = e;
		while (e->isOnRight(pointIndex) && e->nextEdge != end)
			e = e->nextEdge;
		if (e->isOnRight(pointIndex)) found = true;
		else e = e->twinEdge;
	}
	
	return e->face;
}
