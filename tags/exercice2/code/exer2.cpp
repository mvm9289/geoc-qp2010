#include "exer2.h"
#include "data.h"
#include <iostream>
#include <vector>

using namespace std;

// R G B arrays indicate RGB values depending point position
// Point Positions     INSIDE	   OUTSIDE	   ON_SIDE	  ON_VERTEX
const double R[] = {    0.0,		1.0,		1.0,		1.0	    };
const double G[] = {    1.0,		0.0,		1.0,		0.0	    };
const double B[] = {    0.0,		0.0,		0.0,		1.0	    };

// Indicate the next point to analyze
unsigned int nextPoint = 0;

// Return determinant of points a, b , c
inline double det(const Point& a, const Point& b, const Point& c)
{
    return b.x*c.y - c.x*b.y + c.x*a.y - a.x*c.y + a.x*b.y - b.x*a.y;
}

// Return signed determinant of points a, b , c
int signDet(const Point& a, const Point& b, const Point& c)
{
    double d = det(a, b, c);
    
    if (d == 0) return 0;
    else if (d < 0) return -1;
    else return 1;
}

// Return true if there are any triangle and return this in triangle. Else return false
bool getLastTriangle(Polygon &triangle)
{
    bool found = false;
    
    //~ for (int i = polygons.size() - 1; !found && i >= 0; i--)
    //~ {
        //~ if (polygons[i].points.size() == 3)
        //~ {
            //~ found = true;
            //~ triangle = polygons[i];
        //~ }
    //~ }
	
	if (polygons.size() > 0)
	{
		found = true;
		triangle = polygons[polygons.size() - 1];
	}
    
    return found;
}

// Return the position of point p in triangle triangle
int getPointPositionOnTriangle(const Polygon &triangle, const Point &p)
{
	int n = triangle.points.size();
	int zeros = 0;
	int sdet12p = signDet(triangle.points[0], triangle.points[1], p);
	if (sdet12p == 0)
	{
		zeros++;
		int sdetAux = signDet(triangle.points[n-1], triangle.points[0], p);
		if (sdetAux == 0) return ON_VERTEX;
		sdet12p = signDet(triangle.points[1], triangle.points[2], p);
		if (sdet12p == 0) return ON_VERTEX;
		
		if (sdetAux != sdet12p) return OUTSIDE;
	}
	for (int i = 1; i < n; i++)
	{
		int i1 = (i + 1)%n;
		int sdetAux = signDet(triangle.points[i], triangle.points[i1], p);
		if (sdetAux == 0)
		{
			zeros++;cerr << endl << zeros << endl << endl;
			if (zeros == 1)
			{
				int i2 = (i + 2)%n;
				int sdetAux1 = signDet(triangle.points[i-1], triangle.points[i], p);
				if (sdetAux1 == 0) return ON_VERTEX;
				int sdetAux2 = signDet(triangle.points[i1], triangle.points[i2], p);
				if (sdetAux2 == 0) return ON_VERTEX;
				if (sdetAux1 != sdetAux2) return OUTSIDE;
			}
			return ON_VERTEX;
		}
		else if (sdetAux != sdet12p) return OUTSIDE;
	}
	if (zeros > 0) return ON_SIDE;
	
	return INSIDE;
    /*Point t1 = triangle.points[0];
    Point t2 = triangle.points[1];
    Point t3 = triangle.points[2];
    int sdet12p = signDet(t1, t2, p);
    int sdet23p = signDet(t2, t3, p);
    int sdet31p = signDet(t3, t1, p);
    int zeros = 0;
    
    if (sdet12p == 0) zeros++;
    if (sdet23p == 0) zeros++;
    if (sdet31p == 0) zeros++;
    
    if (zeros == 2) return ON_VERTEX;
    if (zeros == 1)
    {
        int sdet1 = sdet12p;
        int sdet2 = sdet23p;
        
        if (sdet1 == 0) sdet1 = sdet31p;
        else if (sdet2 == 0) sdet2 = sdet31p;
        
        if (sdet1 == sdet2) return ON_SIDE;
        else return OUTSIDE;
    }
    if(sdet12p == sdet23p && sdet23p == sdet31p) return INSIDE;
    
    return OUTSIDE;*/
}

// Set a new color to point p
void setPointColor(Point &p, double red, double green, double blue)
{
    p.r = red;
    p.g = green;
    p.b = blue;
    p.draw(true);
}

// Set default color of all points
void resetPointColors()
{
    for (unsigned int i = 0; i < points.size(); i++)
        setPointColor(points[i], 1.0, 1.0, 0.0); // Yellow
}

// Print a message depending value of  message var
void printMessage(int message, string fileEnglish = "Nothing", string fileCatalan = "Nothing")
{
    switch(message)
    {
        case INITIAL:
            cout << "Sortida del test d'inclusió d'un Punt/Point en un triangle per " << fileCatalan << endl;
            for (int i = fileCatalan.size() + 62; i >= 0; i--)
                cout << "=";
            cout << endl;
            cout << "Output of the point-in-triangle test for the " << fileEnglish << endl << endl;
            break;
        case INSIDE:
            cout << "Interior";
            break;
        case OUTSIDE:
            cout << "Exterior";
            break;
        case ON_SIDE:
            cout << "Frontera/Boundary (costat/edge)";
            break;
        case ON_VERTEX:
            cout << "Frontera/Boundary (vèrtex/vertex)";
            break;
        default:
            break;
    }
}

// Decrement the counter of next point to analyze (last point has been removed)
void exer2stepBack()
{
    if (nextPoint) nextPoint--;
}

// Reset the counter of next point to analyze
void exer2reset()
{
    nextPoint = 0;
}

// Print initial messages and execute first test if needed
void exer2initialization(bool fromFile, string fileName)
{
    // Print instructions and recommendations
    cerr << endl;
    cerr << "Test is executed with last triangle ";
    cerr << "introduced. If you remove last triangle";
    cerr << " or paint other, test is executed again ";
    cerr << "for all points.";
    cerr << endl << endl;
    cerr << "To save the test output to a file, "
        << "redirect stdout to a file from "
        << "console with \"> output_file\".";
    cerr << endl << endl;
    cerr << "RECOMMENDED: "
        << "To compare two output files that "
        << "should be the same, use diff command "
        << "with -B option to ignore blank lines.";
    cerr << endl << endl << endl;
    
    string fileEnglish;
    string fileCatalan;
    if (fromFile)
    {
        fileEnglish = "test file " + fileName;
        fileCatalan = "al fitxer de proves " + fileName;
    }
    else
    {
        fileEnglish = "screen input";
        fileCatalan = "la entrada per pantalla";
    }
    
    // Print initial message
    printMessage(INITIAL, fileEnglish, fileCatalan);
    if (fromFile) exer2executeTest();
}

// Execute test
void exer2executeTest()
{
    Polygon triangle;
    
    if (getLastTriangle(triangle))
    {
        for (; nextPoint < points.size(); nextPoint++)
        {
            int pointPosition = getPointPositionOnTriangle(triangle, points[nextPoint]);
            setPointColor(points[nextPoint], R[pointPosition], G[pointPosition], B[pointPosition]);
            cout << "Punt/Point " << nextPoint + 1 << ": ";
            printMessage(pointPosition);
            cout << endl;
        }
    }
    else resetPointColors();
}
