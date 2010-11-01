#include "polygon.h"

#include <GL/gl.h>

using namespace std;

Polygon::Polygon(vector<Point> ps, float cr, float cg, float cb, bool drawP)
    : Options(cr, cg, cb, drawP), points(ps) {}

void Polygon::draw() const{
    glColor3f(r, g, b);
    glBegin(GL_LINE_STRIP);
    drawPolygon(false);
    points[0].draw(false);
    glEnd();
    if (drawPoints){
        drawPolygon(true);
    }
}

void Polygon::drawPolygon(bool drawP) const{
    vector<Point>::const_iterator it;
    for (it = points.begin(); it != points.end(); ++it){
        it->draw(drawP);
    }
}

ostream& operator<<(ostream &os, const Polygon &p){
    vector<Point>::const_iterator it;
    for (it = p.points.begin(); it != p.points.end(); ++it){
        os << *it;;
    }
    
    if (p.r != POL_RED || p.g != POL_GREEN || p.b != POL_BLUE || p.drawPoints){
        os << "o " << p.r << " " << p.g << " " << p.b << " " << p.drawPoints;
    }
    os << endl;
    return os;
}
