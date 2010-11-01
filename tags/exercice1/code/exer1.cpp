#include "exer1.h"
#include "data.h"
#include <iostream>
#include <string>

using namespace std;

int ii = 0;

// Print a message depending value of  message var
void printMessage(int message)
{
    switch(message)
    {
        case INITIAL:
            cout << "Output of the segment intersection test for the ";
            break;
        case NO_INTERSECT:
            cout << "They do not intersect";
            break;
        case BOTH_INTERIOR:
            cout << "They intersect in a point interior to both segments";
            break;
        case BOTH_ENDPOINT:
            cout << "They intersect in an endpoint of both segments";
            break;
        case ENDPOINT_INTERIOR:
            cout << "They intersect in an endpoint of both segments, which is interior to the other segment";
            break;
        case BOTH_SUBSET_SEGMENT:
            cout << "They intersect in a segment which is a subset of both segments";
            break;
        case SUBSET_OF_ANOTHER:
            cout << "They intersect in one of the segments which is a subset of the other one";
            break;
        default:
            break;
    }
}

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

// Return if point c is between points a, b
bool isBetween(const Point& a, const Point& b, const Point& c)
{
    double p1 = a.x;
    double p2 = b.x;
    double p3 = c.x;
    
    if(p1 == p2) // Segment ab is vertical, so we compare y coordinate
    {
        p1 = a.y;
        p2 = b.y;
        p3 = c.y;
    }
    
    return (p1 <= p3 && p3 <= p2) || (p2 <= p3 && p3 <= p1);
}

// Return the intersection type of segments ab, cd
int intersection(const Point& a, const Point& b, const Point& c, const Point& d)
{
    int detABC = signDet(a, b, c);
    int detABD = signDet(a, b , d);
    int detCDA= signDet(c, d, a);
    int detCDB = signDet(c, d, b);
    
    if((detCDA && detCDA == detCDB) || (detABC && detABC == detABD))
        return NO_INTERSECT;
        // One segment is on the side of another
    
    if(detABC && detCDA && detABC == -detABD && detCDA == -detCDB)
        return BOTH_INTERIOR;
        // Both segments intersect at interior point
    
    // From here, some point is colineal with another segment
    bool CinAB = !detABC && isBetween(a, b, c);
    bool DinAB = !detABD && isBetween(a, b, d);
    bool AinCD = !detCDA && isBetween(c, d, a);
    bool BinCD = !detCDB && isBetween(c, d, b);
    
    if((CinAB && DinAB) || (AinCD && BinCD))
        return SUBSET_OF_ANOTHER;
        // One segment is contained in another
    
    // From here, both endpoints of a segment cannot be inside another*
    if((a == c) || (a == d) || (b == c) || (b == d))
        return BOTH_ENDPOINT;
        // Both segments intersect at endpoint of both
    
    // From here, both segments cannot intersect at endpoints of both*
    if((CinAB || DinAB) && (AinCD || BinCD))
        return BOTH_SUBSET_SEGMENT;
        // Both segments intersect at subset segment of both
    
    // From here, only one of  four endpoints can be inside another segment*
    if(CinAB || DinAB || AinCD || BinCD)
        return ENDPOINT_INTERIOR;
        // Both segments intersect at endpoint of one which is interior of another
    
    return NO_INTERSECT;
    // All of above conditions are false, so both segments are colineal but do not intersect
    
    // NOTE: The comments with * determine the conditions of posterior ifs
}

// Execute test and print results
void exer1executeTest()
{
    if(!(++ii%2) && (unsigned)ii <= segments.size())
    {
        LineSegment s1 = segments[ii - 2];
        LineSegment s2 = segments[ii - 1];
        int result = intersection(s1.begin, s1.end, s2.begin, s2.end);
        
        cout << "Pair " << ii/2 << ": ";
        printMessage(result);
        cout << endl;
    }
}

// Print initial messages and execute first test if needed
void exer1initialization(bool fromFile, string fileName)
{
    // Print instructions and recommendations
    cerr << endl;
    cerr << "To save the test output to a file, "
        << "redirect stdout to a file from "
        << "console with \"> output_file\".";
    cerr << endl << endl;
    cerr << "RECOMMENDED: "
        << "To compare two output files that "
        << "should be the same, use diff command "
        << "with -B option to ignore blank lines.";
    cerr << endl << endl << endl;
    
    // Print initial message
    printMessage(INITIAL);
    if(fromFile)
    {
        // Print initial message of file input
        string aux = "test file " + fileName;
        int n = aux.size() + 48;
        cout << aux << endl;
        for(int i = 0; i < n; i++)
            cout << '=';
        cout << endl << endl;
        
        // Execute all test of file inputs
        n = segments.size();
        for(ii = 1; ii < n; ii += 1)
            exer1executeTest();
        ii = n;
    }
    else
    {
        // Print initial message of screen input
        cout << "screen input" << endl;
        for(int i = 0; i < 60; i++)
            cout << '=';
        cout << endl << endl;
    }
}

// Update segments count when remove one
void exer1stepBack()
{
    if(ii) ii--;
}

// Update segments count whenn remove all
void exer1clear()
{
    ii = 0;
}
