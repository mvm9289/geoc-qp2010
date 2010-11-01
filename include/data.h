#ifndef DATA_H
#define DATA_H

#include "bounding_box.h"
#include "circumference_3p.h"
#include "line_segment.h"
#include "point.h"
#include "polygon.h"
#include "triangle.h"
#include <vector>

extern std::vector<Circum3P> circumferences;
extern std::vector<LineSegment> segments;
extern std::vector<Point> points;
extern std::vector<Polygon> polygons;
extern std::vector<Triangle> triangles;

extern std::vector<Point> vNormals;

extern std::vector<std::vector<int> > boundary;

extern BoundingBox bbox;

extern bool pointsChanged;
extern bool segmentsChanged;
extern bool trianglesChanged;

void addCircumference(Circum3P cir);
void addPoint(Point p);
void addPolygon(Polygon pol);
void addSegment(LineSegment s);
void addTriangle(Triangle t);
void clearAll();
void recomputeBoundingBox();

#endif
