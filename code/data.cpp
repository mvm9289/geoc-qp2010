#include "data.h"
#include <iostream>

using namespace std;

vector<Circum3P> circumferences;
vector<LineSegment> segments;
vector<Point> points;
vector<Polygon> polygons;
vector<Triangle> triangles;

vector<Point> vNormals;

vector<vector<int> > boundary;

BoundingBox bbox;

bool pointsChanged = true;
bool segmentsChanged = true;
bool trianglesChanged = true;

void addCircumference(Circum3P cir){
    bbox.update(cir);
    circumferences.push_back(cir);
}

void addPoint(Point p){
    bbox.update(p);
    points.push_back(p);
    pointsChanged = true;
}

void addPolygon(Polygon pol){
    bbox.update(pol);
    polygons.push_back(pol);
}

void addSegment(LineSegment s){
    bbox.update(s);
    segments.push_back(s);
    segmentsChanged = true;
}

void addTriangle(Triangle t){
    //There's no need to update the bounding box: the triangle is made of
    //points already in vector "points".
    triangles.push_back(t);
    trianglesChanged = true;
}

void clearAll(){
    boundary.clear();
    circumferences.clear();
    points.clear();
    polygons.clear();
    segments.clear();
    triangles.clear();
    pointsChanged = true;
    segmentsChanged = true;
    trianglesChanged = true;
    recomputeBoundingBox();
}

void recomputeBoundingBox(){
    if (!points.empty()){
        bbox = BoundingBox(points[0]);
    } 
    else if (!segments.empty()){
        bbox = BoundingBox(segments[0].begin);
    }
    else if (!polygons.empty()){
        bbox = BoundingBox(polygons[0].points[0]);
    }
    else if (!circumferences.empty()){
        bbox = BoundingBox(circumferences[0].points[0]);
    }
    //Don't need to check for the triangles: they are made of points
    //already in vector "points".
    else {
        bbox = BoundingBox();
    }

    for (vector<Point>::const_iterator it = points.begin(); it < points.end(); ++it){
        bbox.update(*it);
    }

    for (vector<LineSegment>::const_iterator it = segments.begin(); it < segments.end(); ++it){
        bbox.update(*it);
    }

    for (vector<Polygon>::const_iterator it = polygons.begin(); it < polygons.end(); ++it){
        bbox.update(*it);
    }

    for (vector<Circum3P>::const_iterator it = circumferences.begin(); it < circumferences.end(); ++it){
        bbox.update(*it);
    }
}

