#include "exer4.h"
#include "data.h"
#include "triangulation.h"

using namespace std;

SquareContainer getSquareContainer()
{
	SquareContainer container;
	container.up = container.bottom = points[0].y;
	container.right = container.left = points[0].x;
	
	unsigned int n = points.size();
	for (unsigned int i = 1; i < n; i++)
	{
		if (points[i].y > container.up) container.up = points[i].y;
		if (points[i].y < container.bottom) container.bottom = points[i].y;
		if (points[i].x > container.right) container.right = points[i].x;
		if (points[i].x < container.left) container.left = points[i].x;
	}
    
    // Expand the perimeter of the square to ensure that all points are inside
	container.up++;
	container.bottom--;
	container.right++;
	container.left--;
	
	return container;
}

TriangleContainer getTriangleContainer()
{
	SquareContainer squareContainer = getSquareContainer();
	double squareHeight = squareContainer.up - squareContainer.bottom;
	double squareWidth = squareContainer.right - squareContainer.left;
	
	TriangleContainer container;
	container.a = Point((squareContainer.right + squareContainer.left)/2, squareContainer.up + squareWidth/2);
	container.b = Point(squareContainer.left - squareHeight, squareContainer.bottom);
	container.c = Point(squareContainer.right + squareHeight, squareContainer.bottom);
	
	return container;
}

void exer4execute()
{
	if (points.size() > 0)
	{
		TriangleContainer triangleContainer = getTriangleContainer();
		
		points.push_back(triangleContainer.a);
		points.push_back(triangleContainer.b);
		points.push_back(triangleContainer.c);
		
		Triangulation triangulation(points.size() - 3, points.size() - 2, points.size() - 1);
		for(unsigned int i = 0; i < (points.size() - 3); i++) triangulation.addPoint(i);
		
		triangulation.deleteUnwantedTriangles();
		triangles = triangulation.getTriangles();
	}
}
