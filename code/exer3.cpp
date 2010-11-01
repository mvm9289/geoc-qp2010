#include "exer3.h"
#include "data.h"
#include <iostream>
#include <vector>

using namespace std;

// R G B arrays indicate RGB values depending point position
// Point Positions     INSIDE	   OUTSIDE	   ON_SIDE
const double R[] = {    0.0,		1.0,		1.0	    };
const double G[] = {    1.0,		0.0,		1.0	    };
const double B[] = {    0.0,		0.0,		0.0	    };

// Indicate the next point to analyze
unsigned int nextPoint = 0;

// Return determinant of points a, b , c
inline double det(const Point& a, const Point& b, const Point& c)
{
    return b.x*c.y*a.z - c.x*b.y*a.z + c.x*a.y*b.z - a.x*c.y*b.z + a.x*b.y*c.z - b.x*a.y*c.z;
}

// Return signed determinant of points a, b , c
int signDet(const Point& a, const Point& b, const Point& c)
{
    double d = det(a, b, c);
    
    if (d == 0) return 0;
    else if (d < 0) return -1;
    else return 1;
}

// Return true if there are any circumference and return this in circumference. Else return false
bool getLastCircumference(Circum3P &circumference)
{
    int n = circumferences.size();
    
    if (n == 0) return false;
    circumference = circumferences[n - 1];
    
    return true;
}

// Return the position of point p in circumference circumference
int getPointPositionOnCircumference(const Circum3P &circumference, const Point &p)
{
    Point a(circumference.points[0].x,
            circumference.points[0].y,
            1);
    Point b(circumference.points[1].x,
            circumference.points[1].y,
            1);
    Point c(circumference.points[2].x,
            circumference.points[2].y,
            1);
    
    Point aux1( (b.x - a.x),
                (c.x - a.x),
                (p.x - a.x));
    Point aux2( (b.y - a.y),
                (c.y - a.y),
                (p.y - a.y));
    Point aux3( ((b.x - a.x)*(b.x + a.x) + (b.y - a.y)*(b.y + a.y)),
                ((c.x - a.x)*(c.x + a.x) + (c.y - a.y)*(c.y + a.y)),
                ((p.x - a.x)*(p.x + a.x) + (p.y - a.y)*(p.y + a.y)));
    
    int sdet = signDet(aux1, aux2, aux3)*signDet(a, b, c);
    
    if (sdet < 0) return INSIDE;
    if (sdet > 0) return OUTSIDE;
    
    return ON_SIDE;
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
            cout << "Sortida del test d'inclusió d'un Punt/Point en un cercle per " << fileCatalan << endl;
            for (int i = fileCatalan.size() + 60; i >= 0; i--)
                cout << "=";
            cout << endl;
            cout << "Output of the point-in-circle test for the " << fileEnglish << endl << endl;
            break;
        case INSIDE:
            cout << "Interior";
            break;
        case OUTSIDE:
            cout << "Exterior";
            break;
        case ON_SIDE:
            cout << "Frontera/Boundary";
            break;
        default:
            break;
    }
}

// Decrement the counter of next point to analyze (last point has been removed)
void exer3stepBack()
{
    if (nextPoint) nextPoint--;
}

// Reset the counter of next point to analyze
void exer3reset()
{
    nextPoint = 0;
}

// Print initial messages and execute first test if needed
void exer3initialization(bool fromFile, string fileName)
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
    if (fromFile) exer3executeTest();
}

// Execute test
void exer3executeTest()
{
    Circum3P circumference;
    
    if (getLastCircumference(circumference))
    {
        for (; nextPoint < points.size(); nextPoint++)
        {
            int pointPosition = getPointPositionOnCircumference(circumference, points[nextPoint]);
            setPointColor(points[nextPoint], R[pointPosition], G[pointPosition], B[pointPosition]);
            cout << "Punt/Point " << nextPoint + 1 << ": ";
            printMessage(pointPosition);
            cout << endl;
        }
    }
    else resetPointColors();
}
