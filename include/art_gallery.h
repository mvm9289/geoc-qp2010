#ifndef __ART_GALLERY_H__
#define __ART_GALLERY_H__

#include "point.h"
#include "line_segment.h"
#include "polygon.h"
#include <vector>
#include <list>
using namespace std;

typedef vector<Point> Vertices;
typedef list<int> IntList;
typedef vector<int> IntVector;
typedef vector<bool> BoolVector;
typedef vector<BoolVector> BoolMatrix;
typedef vector<LineSegment> DrawableDiagonals;

class ArtGallery
{
  private:
    Vertices vertices;
    DrawableDiagonals dDiagonals;
    Vertices guardians;
    BoolMatrix adjacencyList;
  
    void initialize(const Polygon& p);
    bool triangulate();
    void putGuardians();
  
    void setAntiClockwise();
    bool isClockwise();
    bool isInternalDiagonal(int a, int b, const IntVector& index);
    bool inCone(int a, int b, const IntVector& index);
    bool isDiagonal(int a, int b, const IntVector& index);
    bool intersect(const Point& a, const Point& b, const Point& c, const Point& d);
    bool intersectProperly(const Point& a, const Point& b, const Point& c, const Point& d);
    bool between(const Point& a, const Point& b, const Point& c);
    bool leftOn(const Point& a, const Point& b, const Point& c);
    bool left(const Point& a, const Point& b, const Point& c);
    bool collinear(const Point& a, const Point& b, const Point& c);
    double det(const Point& a, const Point& b, const Point& c);
    int signDet(const Point& a, const Point& b, const Point& c);
    void coloring(IntVector& color, IntVector& NColours, int i, int n, bool& finish);
  
  public:
    ArtGallery(const Polygon& p);
    DrawableDiagonals getDiagonals();
    Vertices getGuardians();
};

#endif