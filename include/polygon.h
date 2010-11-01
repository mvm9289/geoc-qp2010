#ifndef POLYGON_H
#define POLYGON_H

#include "options.h"
#include "point.h"
#include <vector>
#include <iostream>

#define POL_RED 0.0
#define POL_GREEN 1.0
#define POL_BLUE 1.0

class Polygon : public Options {
 public:
    std::vector<Point> points;

    Polygon(std::vector<Point> ps = std::vector<Point>(),
            float cr = POL_RED, float cg = POL_GREEN, float cb = POL_BLUE,
            bool drawP = false);
    void draw() const;

 private:
    void drawPolygon(bool drawP) const;
};

std::ostream& operator<<(std::ostream &os, const Polygon &p);

#endif
