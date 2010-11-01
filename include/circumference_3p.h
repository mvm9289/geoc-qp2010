#ifndef CIRCUMFERENCE_3P_H
#define CIRCUMFERENCE_3P_H

#include "options.h"
#include "point.h"
#include <iostream>

#define CIRCUM3P_RED 0
#define CIRCUM3P_GREEN 0.3
#define CIRCUM3P_BLUE 0.3

class Circum3P : public Options {
 public:
    Point points[3];
    Point center;
    coord_type radius;
    bool drawable;

    Circum3P(Point p0 = Point(), Point p1 = Point(), Point p2 = Point(),
             float cr = CIRCUM3P_RED, float cg =CIRCUM3P_GREEN, float cb = CIRCUM3P_BLUE,
             bool drawP = true);
    void computeCircum();
    void draw() const;
};

std::ostream& operator<<(std::ostream &os, const Circum3P &c);

#endif
