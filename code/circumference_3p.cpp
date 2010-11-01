#include "circumference_3p.h"
#include <GL/glut.h>
#include <cmath>

using namespace std;

GLUquadricObj* disk = gluNewQuadric();

static coord_type determ(coord_type a11, coord_type a12, coord_type a13,
                  coord_type a21, coord_type a22, coord_type a23,
                  coord_type a31, coord_type a32, coord_type a33){
	return a11 * a22 * a33 + a21 * a32 * a13 + a31 * a12 * a23
         - a13 * a22 * a31 - a11 * a23 * a32 - a12 * a21 * a33;
}

Circum3P::Circum3P(Point p0, Point p1, Point p2, float cr, float cg, float cb, bool drawP)
    : Options(cr, cg, cb, drawP), drawable(false)
{
    points[0] = p0;
    points[1] = p1;
    points[2] = p2;

    computeCircum();
}

void Circum3P::computeCircum(){
	coord_type div = determ(2 * points[0].x, 2 * points[0].y, 1,
                          2 * points[1].x, 2 * points[1].y, 1,
                          2 * points[2].x, 2 * points[2].y, 1);
	if(div == 0){
		drawable = false;
	}
    else{
		drawable = true;
		coord_type a = determ(points[0].x * points[0].x + points[0].y * points[0].y,
                              2 * points[0].y,
                              1,
                              points[1].x * points[1].x + points[1].y * points[1].y,
                              2 * points[1].y,
                              1,
                              points[2].x * points[2].x + points[2].y * points[2].y,
                              2 * points[2].y,
                              1);

		coord_type c = determ(2 * points[0].x,
                              points[0].x * points[0].x + points[0].y * points[0].y,
                              1,
                              2 * points[1].x,
                              points[1].x * points[1].x + points[1].y * points[1].y,
                              1,
                              2 * points[2].x,
                              points[2].x * points[2].x + points[2].y * points[2].y,
                              1);

		coord_type d = determ(2 * points[0].x,
                              2 * points[0].y,
                              points[0].x * points[0].x + points[0].y * points[0].y,
                              2 * points[1].x,
                              2 * points[1].y,
                              points[1].x * points[1].x + points[1].y * points[1].y,
                              2 * points[2].x,
                              2 * points[2].y,
                              points[2].x * points[2].x + points[2].y * points[2].y);

		center.x = a / div;
		center.y = c / div;
		center.z = 0;
		radius = sqrt(d / div + center.x * center.x + center.y * center.y);
    }
}

void Circum3P::draw() const{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(center.x,center.y,center.z);
    glColor3f(r, g, b);
	gluQuadricDrawStyle(disk,GLU_LINE);
	gluDisk(disk, radius, radius, 100,1);
	glPopMatrix();

    if (drawPoints){
        points[0].draw(true);
        points[1].draw(true);
        points[2].draw(true);
    }
}

ostream& operator<<(ostream &os, const Circum3P &c){
    os << c.points[0] << c.points[1] << c.points[2];
    
    //Only print color and drawPoints if they are different from default.
    if (c.r != CIRCUM3P_RED || c.g != CIRCUM3P_GREEN || c.b != CIRCUM3P_BLUE
        || !c.drawPoints){
        os << "o " << c.r << " " << c.g << " " << c.b << " " << c.drawPoints << endl;
    }
    return os;
}
