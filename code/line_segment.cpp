#include "line_segment.h"
#include <GL/gl.h>

using namespace std;

LineSegment::LineSegment(Point bp, Point ep, float cr, float cg, float cb, bool drawP)
    : Options(cr, cg, cb, drawP), begin(bp), end(ep) {}

void LineSegment::draw() const{
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    begin.draw();
    end.draw();
    glEnd();
    if (drawPoints){
        glBegin(GL_POINTS);
        begin.draw(true);
        end.draw(true);
        glEnd();
    }
}

ostream& operator<<(ostream &os, const LineSegment &s){
    os << s.begin << s.end;
    
    //Only print color and drawPoints if they are different from default.
    if (s.r != LSEGMENT_RED || s.g != LSEGMENT_GREEN || s.b != LSEGMENT_BLUE
        || s.drawPoints){
        os << "o " << s.r << " " << s.g << " " << s.b << " " << s.drawPoints << endl;
    }
    return os;
}

