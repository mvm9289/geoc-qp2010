#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "options.h"
#include "point.h"
#include <iostream>

#define LSEGMENT_RED 1.0
#define LSEGMENT_GREEN 1.0
#define LSEGMENT_BLUE 1.0

class LineSegment : public Options {
 public:
    Point begin, end;

    LineSegment(Point b = Point(), Point e = Point(),
                float cr = LSEGMENT_RED, float cg = LSEGMENT_GREEN, float cb = LSEGMENT_BLUE,
                bool drawP = false);
    void draw() const;
};

std::ostream& operator<<(std::ostream &os, const LineSegment &s);

#endif
