#ifndef POINT_H
#define POINT_H

#include "types.h"
#include <iostream>

#define POINT_RED 1.0
#define POINT_GREEN 1.0
#define POINT_BLUE 0.0

class Point {
 public:
    coord_type x, y, z;
    float r, g, b;
    
    Point(coord_type px = 0.0, coord_type py = 0.0, coord_type pz = 0.0,
          float cr = POINT_RED, float cg = POINT_GREEN, float cb = POINT_BLUE);

    bool operator==(const Point& right) const;
    Point operator+(const Point& p) const;
    Point& operator+=(const Point& p);
    Point operator-(const Point& p) const;
    Point& operator-=(const Point & p);
    Point operator*(coord_type d) const;
    friend Point operator*(coord_type d, Point p);
    Point& operator*=(coord_type d);
    Point operator/(coord_type d) const;
    Point& operator/=(coord_type d);
    Point operator^(const Point& p) const;
    coord_type dotProduct(const Point& p) const;
    coord_type norm() const;
    void normalize();

    void draw(bool drawPoints = false) const;
};

std::ostream& operator<<(std::ostream &os, const Point &p);

#endif
