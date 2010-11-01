#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"
#include <iostream>

class Triangle {
 public:
    int pointsId[3];
    Point normal;

    Triangle(int p1 = 0, int p2 = 0, int p3 = 0);
    bool isCorrect() const;
    void draw(bool vertexNormals = true) const;
    void computeNormal();
};

std::ostream& operator<<(std::ostream &os, const Triangle &t);

#endif
