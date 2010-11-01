#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "point.h"
#include "line_segment.h"
#include "polygon.h"
#include "circumference_3p.h"

class BoundingBox {
 public:
    Point lowerLeft;
    Point upperRight;

    BoundingBox(Point p = Point());
    void update(Point p);
    void update(LineSegment s);
    void update(Polygon pol);
    void update(Circum3P cir);
};

#endif
