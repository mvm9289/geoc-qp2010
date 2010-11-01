#include "point.h"

#include <cmath>
#include <GL/gl.h>

using namespace std;

Point::Point(coord_type px, coord_type py, coord_type pz,
             float cr, float cg, float cb)
    : x(px), y(py), z(pz), r(cr), g(cg), b(cb){}

bool Point::operator==(const Point& right) const{
    return ((x == right.x) && (y == right.y) && (z == right.z));
}


Point Point::operator+(const Point& p) const{
    return Point(x + p.x, y + p.y, z + p.z);
}

Point& Point::operator+=(const Point& p){
    x += p.x;
    y += p.y;
    z += p.z;
    return *this;
}

Point Point::operator-(const Point& p) const{
    return Point(x - p.x, y - p.y, z - p.z);
}

Point& Point::operator-=(const Point& p){
    x -= p.x;
    y -= p.y;
    z -= p.z;
    return *this;
}

Point Point::operator*(coord_type d) const{
    return Point(x * d, y * d, z * d);
}

Point operator*(coord_type d, Point p){
    return Point(p.x * d, p.y * d, p.z * d);
}

Point& Point::operator*=(coord_type d){
    x *= d;
    y *= d;
    z *= d;
    return *this;
}

Point Point::operator/(coord_type d) const{
    return Point(x / d, y / d, z / d);
}

Point& Point::operator/=(coord_type d){
    x /= d;
    y /= d;
    z /= d;
    return *this;
}

Point Point::operator^(const Point &p) const{
    return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

coord_type Point::dotProduct(const Point &p) const{
    return x * p.x + y * p.y + z * p.z;
}

coord_type Point::norm() const{
    return sqrt(x * x + y * y + z * z);
}

void Point::normalize(){
    coord_type n = norm();
    x /= n;
    y /= n;
    z /= n;
}

void Point::draw(bool drawPoints) const{
    if (drawPoints){
        glColor3f(r, g, b);
        glBegin(GL_POINTS);
    }
    glVertex3d(x, y, z);
    if (drawPoints){
        glEnd();
    }
}

ostream& operator<<(ostream &os, const Point &p){
    os << p.x << " " << p.y << " " << p.z;

    //Only print the color of the point if it's not the default one.
    if (p.r != POINT_RED || p.g != POINT_GREEN || p.b != POINT_BLUE){
        os << " " << p.r << " " << p.g << " " << p.b;
    }
    os << endl;
    return os;
}
