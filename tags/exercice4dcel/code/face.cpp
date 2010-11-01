
#include "face.h"
#include <iostream>
using namespace std;

Face :: Face (Edge* e)
{
	edge = e;
}

Triangle Face :: getTriangle()
{
	Edge* e1 = edge;
	Edge* e2 = e1->nextEdge;
	Edge* e3 = e2->nextEdge;
	
	int a = e1->vertex->pointIndex;
	int b = e2->vertex->pointIndex;
	int c = e3->vertex->pointIndex;
	
	int invalids = 0;
	if (!(e1->valid)) invalids++;
	if (!(e2->valid)) invalids += 2;
	if (!(e3->valid)) invalids += 4;
	
	switch (invalids)
	{
		case 3:
			b = c;
			break;
		case 5:
			a = b;
			break;
		case 6:
			c = a;
			break;
		case 7:
			b = a;
			c = a;
			break;
		default:
			break;
	}
	
	return Triangle(a, b, c);
}
