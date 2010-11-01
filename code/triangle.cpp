#include "triangle.h"
#include "data.h"
#include "GL/gl.h"

using namespace std;

Triangle::Triangle(int p1, int p2, int p3){
    pointsId[0] = p1;
    pointsId[1] = p2;
    pointsId[2] = p3;
}

bool Triangle::isCorrect() const{
    int num = points.size();
    return (pointsId[0] >= 0 && pointsId[0] < num &&
            pointsId[1] >= 0 && pointsId[1] < num &&
            pointsId[2] >= 0 && pointsId[2] < num);
}

void Triangle::draw(bool vertexNormals) const{
    glBegin(GL_TRIANGLES);
    if (!vertexNormals) glNormal3f(normal.x, normal.y, normal.z);
    for (int i = 0; i < 3; ++i){
        int id = pointsId[i];
        if (vertexNormals) glNormal3f(vNormals[id].x, vNormals[id].y, vNormals[id].z);
        glVertex3f(points[id].x, points[id].y, points[id].z);
    }
    glEnd();
}

void Triangle::computeNormal(){
    normal = (points[pointsId[1]] - points[pointsId[0]])
             ^ (points[pointsId[2]] - points[pointsId[1]]);
    normal.normalize();
}

ostream& operator<<(ostream &os, const Triangle &t){
    os << t.pointsId[0] << " " << t.pointsId[1] << " " << t.pointsId[2] << endl;
    return os;
}
