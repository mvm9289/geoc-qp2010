#include "bounding_box.h"

using namespace std;

BoundingBox::BoundingBox(Point p)
    : lowerLeft(p), upperRight(p) {}


void BoundingBox::update(Point p){
    if (p.x < lowerLeft.x) lowerLeft.x = p.x;
    if (p.y < lowerLeft.y) lowerLeft.y = p.y;
    if (p.z < lowerLeft.z) lowerLeft.z = p.z;
    if (p.x > upperRight.x) upperRight.x = p.x;
    if (p.y > upperRight.y) upperRight.y = p.y;
    if (p.z > upperRight.z) upperRight.z = p.z;
}

void BoundingBox::update(LineSegment s){
    update(s.begin);
    update(s.end);
}

void BoundingBox::update(Polygon pol){
    for(vector<Point>::iterator it = pol.points.begin(); it < pol.points.end(); ++it){
        update(*it);
    }
}

void BoundingBox::update(Circum3P cir){
    if (cir.drawable){
        update(Point(cir.center.x - cir.radius, cir.center.y - cir.radius, cir.center.z));
        update(Point(cir.center.x + cir.radius, cir.center.y + cir.radius, cir.center.z));
    }
}
