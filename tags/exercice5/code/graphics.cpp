#include "circumference_3p.h"
#include "data.h"
#include "graphics.h"
#include "line_segment.h"
#include "parser.h"
#include "point.h"
#include "polygon.h"
#include "triangle.h"
#include "viewer.h"
#include "writer.h"

#include <cmath>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;

bool is2D = true;
Point VRP; //View Reference Point
double phi, theta, psi; //Angles (in degrees)
double dist; //Distance from VRP to observer
double zoom; //Angle of the camera: half the FOV (in radians).
double znear, zfar;
int windowWidth = 600;
int windowHeight = 600;

float pointsWidth = 1.5;
float segmentsWidth = 1.5;
float polygonsWidth = 1.5;
float circumsWidth = 1.5;
float *currWidth = &pointsWidth;

bool doingPan = false;
int pixelsForPan = 5; //Number of pixels panned for cursor key press.
bool doingZoom = false;
double zoomChange = 0.01;
bool doingPanZoom = false;
int oldX;
int oldY;

bool doingRotation = false;
bool rotatingPhi = false;

int border = 50; //Width of the border where clicking "enlarges" the window.

enum DrawingMode{
    POINT_MODE,
    SEGMENT_MODE,
    POLYGON_MODE,
    CIRCUM_MODE
};
DrawingMode drawingMode = POINT_MODE;
int numDrawingPoints = 0;
LineSegment drawingSegment;
Polygon drawingPolygon;
Circum3P drawingCircum;
Point currentPoint; //Point marking where is the mouse
int pixelsToClosePolygon = 20;

bool fillPolygons = true;
bool drawTextures = false;
bool drawVertexNormals = true;

//Altitude colors
unsigned char texture[8][3] = {
	{ 0xaf, 0xf8, 0x43 },       // Green
	{ 0xff, 0xff, 0x92 },       // Yellow
	{ 0xff, 0xab, 0x5c },       // Light brown
	{ 0xff, 0x79, 0x3c },       // Brown
	{ 0xcf, 0x4e, 0x2e },       // Dark brown
	{ 0x78, 0x2b, 0xee },       // Dark violet
	{ 0xa1, 0x85, 0xf0 },       // Violet
	{ 0xd3, 0xc0, 0xf4 },       // Light violet
};

//Normals for triangles
vector<bool> pointFromTriangle;

//Vertex Buffer Objects
bool ableVBO;
bool useVBO = false;

unsigned int TYPE_OF_COORD = (sizeof(float) == sizeof(coord_type) ? GL_FLOAT : GL_DOUBLE);

unsigned int numValidPoints = 0;
unsigned int numValidTriangles = 0;
unsigned int numPointsOfSegments = 0;

unsigned int idPointsCoordBuffer;
unsigned int idPointsColorBuffer;
unsigned int idPointsOfSegmentsCoordBuffer;
unsigned int idPointsOfSegmentsColorBuffer;
unsigned int idSegmentsCoordBuffer;
unsigned int idSegmentsColorBuffer;
unsigned int idTrianglesBuffer;

int distanceInPixels(Point a, Point b);
void reshape(int w, int h);
void mouseClicked(int button, int state, int x, int y);
void mouseMoved(int x, int y);
void mousePassiveMoved(int x, int y);
void idle_func();
void keyboard(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void display();
void setOpenGLParams();
void initialCam();
void initializeBuffers();
void deleteBuffers();
void setModelView();
void setProjection();
void makePan(int incrX, int incrY);
void makeZoom(int incrY);
void makeRotation(int incrX, int incrY);
Point mousePoint(int x, int y, bool pan = false);
void checkBorder(int x, int y);
void checkModelChanged();
void computeNormals();

inline double radToDeg(double rad){
    return rad * 180.0 / M_PI;
}

inline double degToRad(double deg){
    return deg * M_PI / 180.0;
}

void initializeGraphics(int *argc, char** argv){
    glutInit(argc, argv);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
    //The title of the window is the name of the program.
    glutCreateWindow(argv[0]);

    GLenum err = glewInit();
    if (GLEW_OK != err){
        cerr << "Problem using GLEW: " << glewGetErrorString(err) << endl;
        ableVBO = false;
    }
    else {
        ableVBO = GLEW_ARB_vertex_buffer_object;
    }

    glutReshapeFunc(reshape);
    glutMouseFunc(mouseClicked);
    glutMotionFunc(mouseMoved);
    glutPassiveMotionFunc(mousePassiveMoved);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(processSpecialKeys);
    glutDisplayFunc(display);
    setOpenGLParams();
    initialCam();
    if (useVBO) initializeBuffers();
    glutMainLoop();
}


void reshape(int w, int h){
    windowWidth = (w == 0)? 1 : w;
    windowHeight = (h == 0)? 1 : h;
    glViewport(0, 0, windowWidth, windowHeight);
    glutPostRedisplay();
}

void mouseClicked(int button, int state, int x, int y){
    oldX = x;
    oldY = y;
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_DOWN){//Clicked
            switch(glutGetModifiers()){
            case (GLUT_ACTIVE_CTRL):
                doingPan = true;
                return;
            case (GLUT_ACTIVE_SHIFT):
                doingZoom = true;
                return;
            case (GLUT_ACTIVE_ALT):
                doingPanZoom = true;
                return;
            default:
                break;
            }
            
            //We have no key pressed, so we are drawing some figure
            Point clicked = mousePoint(x, y);
            switch (drawingMode){
            case POINT_MODE:
                addPoint(clicked);
                pointsChanged = true;
                break;
            case SEGMENT_MODE:
                if (++numDrawingPoints == 1){
                    drawingSegment.begin = clicked;
                }
                else {
                    drawingSegment.end = clicked;
                    addSegment(drawingSegment);
                    numDrawingPoints = 0;
                    segmentsChanged = true;
                }
                break;
            case POLYGON_MODE:
                ++numDrawingPoints;
                //We also count the closing point, that's why it's 3 instead of 2
                if (numDrawingPoints > 3
                    && distanceInPixels(drawingPolygon.points.front(), clicked) < pixelsToClosePolygon){
                    addPolygon(drawingPolygon);
                    numDrawingPoints = 0;
                    drawingPolygon = Polygon();
                }
                else {
                    drawingPolygon.points.push_back(clicked);
                }
                break;
            case CIRCUM_MODE:
                drawingCircum.points[numDrawingPoints++] = clicked;
                if (numDrawingPoints == 3){
                    drawingCircum.computeCircum();
                    if (drawingCircum.drawable){
                        
                        addCircumference(drawingCircum);
                        numDrawingPoints = 0;
                        drawingCircum = Circum3P();
                    }
                    else {
                        --numDrawingPoints;
                    }
                }
                break;
            default:
                break;
            }
            checkBorder(x, y);
        }
        else {//Button released
            doingPan = false;
            doingZoom = false;
            doingPanZoom = false;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON){
        if (state == GLUT_DOWN){
            doingRotation = true;
            if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
                rotatingPhi = true;
            }
        }
        else {
            doingRotation = false;
            rotatingPhi = false;
        }
    }
    glutPostRedisplay();
}

void mouseMoved(int x, int y){
    int difX = x - oldX;
    int difY = y - oldY;
    if (doingPan) {
        makePan(difX, difY);
    }
    else if (doingZoom) {
        makeZoom(difY);
    }
    else if (doingPanZoom){
        makePan(difX, difY);
        //We want to enlarge the window by 2 times difX (or difY), so
        //we simulate a click at this distance from border
        checkBorder(border - abs(difX) * 2, border - abs(difY) * 2);
    }
    else if (doingRotation){
        makeRotation(difX, difY);
    }
    oldX = x;
    oldY = y;
    glutPostRedisplay();
}

void mousePassiveMoved(int x, int y){
    currentPoint = mousePoint(x,y);
    //If creating a figure, check whether we need to enlarge the window
    if (numDrawingPoints > 0){
        checkBorder(x, y);
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int, int){
    switch(key){
    case 27: // Esc 
        if (ableVBO && useVBO) deleteBuffers();
        exit(0);
        break;
    case '\b': //Backspace
        if (numDrawingPoints > 0){
            --numDrawingPoints;
            if (drawingMode == POLYGON_MODE){
                drawingPolygon.points.pop_back();
            }
        }
        else {
            switch(drawingMode){
            case POINT_MODE:
                if (!points.empty()) {
                    points.pop_back();
                    vNormals.pop_back();
                    pointFromTriangle.pop_back();
		    pointsChanged = true;
                }
                break;
            case SEGMENT_MODE:
                if (!segments.empty()) segments.pop_back();
                break;
            case POLYGON_MODE:
                if (!polygons.empty()) polygons.pop_back();
                break;
            case CIRCUM_MODE:
                if (!circumferences.empty()) circumferences.pop_back();
                break;
            default:
                break;
            }
            recomputeBoundingBox();
        }
        break;
    case '+':
        *currWidth *= 1.1;
        if (*currWidth > 10) *currWidth = 10;
        break;
    case '-':
        *currWidth *= 0.9;
        if (*currWidth < 1) *currWidth = 1;
        break;
    case '2':
        is2D = true;
        psi = 0;
        theta = 0;
        break;
    case '3':
        is2D = false;
        break;
    case 'a':
        if (ableVBO){
            if (useVBO){
                deleteBuffers();
                useVBO = false;
            }
            else {
                initializeBuffers();
                useVBO = true;
            }
        }
        break;
    case 'c':
        drawingMode = CIRCUM_MODE;
        currWidth = &circumsWidth;
        drawingCircum = Circum3P();
        numDrawingPoints = 0;
        break;
    case 'd':
        switch(drawingMode){
        case POINT_MODE:
            points.clear();
            triangles.clear();
            vNormals.clear();
            pointFromTriangle.clear();
            boundary.clear();
            break;
        case SEGMENT_MODE:
            segments.clear();
            break;
        case POLYGON_MODE:
            polygons.clear();
            break;
        case CIRCUM_MODE:
            circumferences.clear();
            break;
        default:
            break;
        }
        recomputeBoundingBox();
        break;
    case 'D':
        clearAll();
        initialCam();
        break;
    case 'f':
        fillPolygons = !fillPolygons;
        break;
    case 'g':
        writeToFile();
        break;
    case 'i':
        initialCam();
        break;
    case 'n':
        drawVertexNormals = !drawVertexNormals;
        break;
    case 'p': //lowercase
        drawingMode = POINT_MODE;
        currWidth = &pointsWidth;
        break;
    case 'P': //uppercase
        drawingMode = POLYGON_MODE;
        currWidth = &polygonsWidth;
        drawingPolygon = Polygon();
        numDrawingPoints = 0;
        break;
    case 'r':
        execute();
        initialCam();
        break;
    case 's':
        drawingMode = SEGMENT_MODE;
        currWidth = &segmentsWidth;
        drawingSegment = LineSegment();
        numDrawingPoints = 0;
        break;
    case 't':
        drawTextures = !drawTextures;
    default:
        break;
    }
    glutPostRedisplay();
}

void processSpecialKeys(int key, int, int){
    switch(key){
    case GLUT_KEY_LEFT:
        makePan(pixelsForPan,0);
        break;
    case GLUT_KEY_RIGHT:
        makePan(-pixelsForPan,0);
        break;
    case GLUT_KEY_UP:
        makePan(0,pixelsForPan);//y grows down
        break;
    case GLUT_KEY_DOWN:
        makePan(0,-pixelsForPan);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    GLfloat  ambient_light[] = {0.3, 0.3, 0.45, 1.0};
    GLfloat   source_light[] = {0.9, 0.8, 0.8, 1.0};
    GLfloat      light_pos[] = {0, 0, 0, 1};
    
    //Enable illumination.
    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, source_light);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glEnable(GL_LIGHT0);
    
    GLfloat a[4] = {0.5,0.8,0.5,1.0};
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, a);
    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, a);

    setModelView();
    setProjection();

    if (drawTextures){
        glTexImage1D (GL_TEXTURE_1D, 0, 3, 8, 0,GL_RGB, GL_UNSIGNED_BYTE, texture);
        
        glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        
        GLfloat plane[]={0.0,0.0, 1 /(bbox.upperRight.z - bbox.lowerLeft.z), 0};
        
        glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
        glTexGenfv(GL_S,GL_OBJECT_PLANE,plane);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_1D);
    }

    if (fillPolygons){
        glPolygonMode(GL_FRONT, GL_FILL);
    }
    else {
        glPolygonMode(GL_FRONT, GL_LINE);
    }

    checkModelChanged();

    //Draw triangles
    if (ableVBO && useVBO){
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, idTrianglesBuffer);
            //The buffer has coordinates, rough normals and smooth normals interleaved (in this order)
            glVertexPointer(3, TYPE_OF_COORD, 9 * sizeof(coord_type), 0);
            glNormalPointer(TYPE_OF_COORD, 9 * sizeof(coord_type),
                            (GLvoid *) ((drawVertexNormals ? 6 : 3) * sizeof(coord_type)));
            glDrawArrays(GL_TRIANGLES, 0, 3 * numValidTriangles);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else {
        for (vector<Triangle>::const_iterator tIt = triangles.begin(); tIt < triangles.end(); ++tIt){
            if (tIt->isCorrect()){
                tIt->draw(drawVertexNormals);
            }
        }
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_1D);

    glPointSize(pointsWidth);

    //Draw points
    if (ableVBO && useVBO){
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, idPointsCoordBuffer);
            glVertexPointer(3, TYPE_OF_COORD, 0, 0);
            glBindBuffer(GL_ARRAY_BUFFER, idPointsColorBuffer);
            glColorPointer(3, GL_FLOAT, 0, 0);
            glDrawArrays(GL_POINTS, 0, numValidPoints);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else {
        int npoint = 0;
        for (vector<Point>::const_iterator pIt = points.begin(); pIt < points.end(); ++pIt, ++npoint){
            if (!pointFromTriangle[npoint]){
                pIt->draw(true);
            }
        }
    }

    glLineWidth(segmentsWidth);
    //Draw segments
    if (ableVBO && useVBO){
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, idSegmentsCoordBuffer);
            glVertexPointer(3, TYPE_OF_COORD, 0, 0);
            glBindBuffer(GL_ARRAY_BUFFER, idSegmentsColorBuffer);
            glColorPointer(3, GL_FLOAT, 0, 0);
            glDrawArrays(GL_LINES, 0, 2 * segments.size());
            glBindBuffer(GL_ARRAY_BUFFER, idPointsOfSegmentsCoordBuffer);
            glVertexPointer(3, TYPE_OF_COORD, 0, 0);
            glBindBuffer(GL_ARRAY_BUFFER, idPointsOfSegmentsColorBuffer);
            glColorPointer(3, GL_FLOAT, 0, 0);
            glDrawArrays(GL_POINTS, 0, 2 * numPointsOfSegments);
            glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        
    }
    else {
        for (vector<LineSegment>::const_iterator sIt = segments.begin(); sIt < segments.end(); ++sIt){
            sIt->draw();
        }
    }

    glLineWidth(polygonsWidth);
    for (vector<Polygon>::const_iterator polIt = polygons.begin(); polIt < polygons.end(); ++polIt){
        polIt->draw();
    }

    glLineWidth(circumsWidth);
    for (vector<Circum3P>::const_iterator cIt = circumferences.begin(); cIt < circumferences.end(); ++cIt){
        cIt->draw();
    }

    //Current drawing
    glLineWidth(*currWidth);
    switch(drawingMode){
    case POINT_MODE:
        //We don't do anything, because there is no temporal point.
        break;
    case SEGMENT_MODE:
        if (numDrawingPoints == 1){
            drawingSegment.end = currentPoint;
            drawingSegment.draw();
        }
        break;
    case POLYGON_MODE:
        if (numDrawingPoints > 0){
            glColor3f(POL_RED, POL_GREEN, POL_BLUE);
            glBegin(GL_LINE_STRIP);
            vector<Point>::const_iterator it; 
            for(it = drawingPolygon.points.begin(); it != drawingPolygon.points.end(); ++it){
                it->draw();
            }
            glEnd();

            //Draw the line to the current mouse position
            if (numDrawingPoints > 2
                && distanceInPixels(drawingPolygon.points.front(), currentPoint) < pixelsToClosePolygon){
                glColor3f(1, 0, 0);
                glBegin(GL_LINES);
                drawingPolygon.points.back().draw();
                drawingPolygon.points.front().draw();
                glEnd();
            }
            else {
                glColor3f(POL_RED, POL_GREEN, POL_BLUE);
                glBegin(GL_LINES);
                drawingPolygon.points.back().draw();
                currentPoint.draw();
                glEnd();
            }
        }
        break;
    case CIRCUM_MODE:
        if (numDrawingPoints == 1){
            drawingCircum.points[0].draw(true);
        }
        else if (numDrawingPoints == 2){
            drawingCircum.points[0].draw(true);
            drawingCircum.points[1].draw(true);
            drawingCircum.points[2] = currentPoint;
            drawingCircum.computeCircum();
            if (drawingCircum.drawable){
                drawingCircum.draw();
            }
        }
        break;
    default:
        break;
    }

    glPopMatrix();
    glutSwapBuffers();
}

void setOpenGLParams(){

    //Enable elimination of hidden parts
    glEnable(GL_DEPTH_TEST);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Eliminate poligons by its orientation
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnable(GL_NORMALIZE);

    //Enable antialiasing
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glClearColor(0.0, 0.0, 0.0, 0.0);
}


void initialCam(){
    VRP = (bbox.upperRight - bbox.lowerLeft) / 2 + bbox.lowerLeft;

    //radius of the containing sphere
    double radius = (bbox.upperRight - VRP).norm();

    if (radius == 0){
        radius = 5; // Arbitrary value
    }

    dist = 4 * radius;

    znear = dist - 1.2 * radius;
    zfar = dist + 1.2 * radius;
    
    phi = 0;
    theta = 0;
    psi = 0;

    zoom = asin(radius / dist);

    setModelView();
    setProjection();
}

void setModelView(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslated(0, 0, -dist);
        
    glRotated(-phi, 0, 0, 1);
    if (!is2D){
        glRotated(theta, 1, 0, 0);
        glRotated(-psi, 0, 1, 0);
    }

    glTranslated(-VRP.x, -VRP.y, -VRP.z);
}


void setProjection(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double arv; // Aspect ratio of view
    arv = windowWidth / (double) windowHeight;

    if (is2D){
        double maxY = dist * tan(zoom);
        double maxX = arv * maxY;
        
        glOrtho(-maxX, maxX, -maxY, maxY, znear, zfar);
    }
    else {//3D
        double fovy;
        if (arv < 1){//1 is Aspect ratio of scene
            fovy = 2 * radToDeg(atan( tan(zoom)/arv ));
        }
        else {
            fovy = 2 * radToDeg(zoom);
        }
        gluPerspective(fovy, arv, znear, zfar);
    }
}

void makePan(int incrX, int incrY){
    VRP = mousePoint(incrX, incrY, true);
}

void makeZoom(int incrY){
    zoom += incrY * zoom * zoomChange;
    if (zoom > degToRad(89)){
        zoom = degToRad(89);
    }
}

void makeRotation(int incrX, int incrY){
    if (rotatingPhi){
        phi -= incrX;
        while (phi >= 360) phi -= 360;
        while (phi < 0) phi += 360;
    }
    else if (!is2D){
        psi -= incrX;
        while (psi >= 360) psi -= 360;
        while (psi < 0) psi += 360;

        theta += incrY;
        while (theta >= 360) theta -= 360;
        while (theta < 0) theta += 360;
    }

}

Point mousePoint(int x, int y, bool pan){
    double matrix[4][4];
    glGetDoublev(GL_MODELVIEW_MATRIX, &matrix[0][0]);
    
    Point directionX(matrix[0][0], matrix[1][0], matrix[2][0]);
    Point directionY(matrix[0][1], matrix[1][1], matrix[2][1]);

    directionX.normalize();
    directionY.normalize();

    double arv = windowWidth / (double) windowHeight;
    double maxY = dist * 2 * tan(zoom); //(zoom is only half the FOV)
    double maxX = arv * maxY;

    double relY = y / (double) windowHeight;
    double relX = x / (double) windowWidth;

    if (!pan) {
        //When we are not doing pan, (x,y) = (0,0) is the upper left corner of the screen.
        relY -= 0.5;
        relX -= 0.5;
    }

    double absY = relY * maxY;
    directionY *= absY;

    double absX = relX * maxX;
    directionX *= absX;

    if (pan) return (VRP - directionX + directionY);
    else return (VRP + directionX - directionY);
}

void checkBorder(int x, int y){
    int missingPixelsX = (x < 0.5 * windowWidth)? border - x : border - (windowWidth - x);
    int missingPixelsY = (y < 0.5 * windowHeight)? border - y : border - (windowHeight - y);

    double arv = windowWidth / (double) windowHeight;
    double height = dist * tan(zoom);
    double width = arv * height;

    if (missingPixelsX > 0){
        double newWidth = width * (1 + missingPixelsX / (double) windowWidth);
        //Divide by arv to obtain the vertical angle
        double newZoom = atan(newWidth / (dist * arv));
        zoom = max(zoom, newZoom);
    }
    if (missingPixelsY > 0){
        double newHeight = height * (1 + missingPixelsY / (double) windowHeight);
        double newZoom = atan(newHeight / dist);
        zoom = max(zoom, newZoom);
    }
}

int distanceInPixels(Point a, Point b){
    double maxY = dist * 2 * tan(zoom);
    double d = (a - b).norm();
    return (int) (windowHeight * d / maxY);
}

void initializeBuffers(){
    glGenBuffers(1, &idPointsCoordBuffer);
    glGenBuffers(1, &idPointsColorBuffer);
    glGenBuffers(1, &idPointsOfSegmentsCoordBuffer);
    glGenBuffers(1, &idPointsOfSegmentsColorBuffer);
    glGenBuffers(1, &idSegmentsCoordBuffer);
    glGenBuffers(1, &idSegmentsColorBuffer);
    glGenBuffers(1, &idTrianglesBuffer);
    pointsChanged = true;
    segmentsChanged = true;
    trianglesChanged = true;
}

void deleteBuffers(){
    glDeleteBuffers(1, &idPointsCoordBuffer);
    glDeleteBuffers(1, &idPointsColorBuffer);
    glDeleteBuffers(1, &idPointsOfSegmentsCoordBuffer);
    glDeleteBuffers(1, &idPointsOfSegmentsColorBuffer);
    glDeleteBuffers(1, &idSegmentsCoordBuffer);
    glDeleteBuffers(1, &idSegmentsColorBuffer);
    glDeleteBuffers(1, &idTrianglesBuffer);
}

void checkModelChanged(){
    if (pointsChanged || trianglesChanged){
        pointsChanged = false;
        trianglesChanged = false;

        computeNormals();

        if (ableVBO && useVBO){
            numValidPoints = 0;
            numValidTriangles = 0;
            vector<coord_type> pointCoordData;
            vector<float> pointColorData;
            vector<coord_type> trianglesData;

            for (vector<Triangle>::iterator it = triangles.begin(); it < triangles.end(); ++it){
                if (it->isCorrect()){
                    for (int i = 0; i < 3; ++i){
                        int pId = it->pointsId[i];

                        trianglesData.push_back(points[pId].x);
                        trianglesData.push_back(points[pId].y);
                        trianglesData.push_back(points[pId].z);

                        trianglesData.push_back(it->normal.x);
                        trianglesData.push_back(it->normal.y);
                        trianglesData.push_back(it->normal.z);

                        trianglesData.push_back(vNormals[pId].x);
                        trianglesData.push_back(vNormals[pId].y);
                        trianglesData.push_back(vNormals[pId].z);
                    }
                    numValidTriangles++;
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, idTrianglesBuffer);
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW); //Release memory
            //3 (coordinates, rough normals, smooth normals)
            //3 (components per vertex)
            //3 (vertices per triangle)
            glBufferData(GL_ARRAY_BUFFER, 3 * 3 * 3 * numValidTriangles * sizeof(coord_type),
                         &trianglesData[0], GL_STATIC_DRAW);

            //Split coordinates/color in two arrays
            for (unsigned int numPoint = 0; numPoint < points.size(); ++numPoint){
                if (!pointFromTriangle[numPoint]){
                    pointCoordData.push_back(points[numPoint].x);
                    pointCoordData.push_back(points[numPoint].y);
                    pointCoordData.push_back(points[numPoint].z);
                
                    pointColorData.push_back(points[numPoint].r);
                    pointColorData.push_back(points[numPoint].g);
                    pointColorData.push_back(points[numPoint].b);

                    numValidPoints++;
                }
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, idPointsCoordBuffer);
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, 3 * numValidPoints * sizeof(coord_type),
                         &pointCoordData[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, idPointsColorBuffer);
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, 3 * numValidPoints * sizeof(float), 
                         &pointColorData[0], GL_STATIC_DRAW);
        }
    }

    if (segmentsChanged){
        segmentsChanged = false;
        if (ableVBO && useVBO){
            numPointsOfSegments = 0;
            vector<coord_type> segmentsCoordData;
            vector<float> segmentsColorData;
            vector<coord_type> pointsOfSegmentsCoordData;
            vector<float> pointsOfSegmentsColorData;

            for (vector<LineSegment>::iterator it = segments.begin() ; it < segments.end(); ++it){
                segmentsCoordData.push_back(it->begin.x);
                segmentsCoordData.push_back(it->begin.y);
                segmentsCoordData.push_back(it->begin.z);

                segmentsCoordData.push_back(it->end.x);
                segmentsCoordData.push_back(it->end.y);
                segmentsCoordData.push_back(it->end.z);

                segmentsColorData.push_back(it->r);
                segmentsColorData.push_back(it->g);
                segmentsColorData.push_back(it->b);

                segmentsColorData.push_back(it->r);
                segmentsColorData.push_back(it->g);
                segmentsColorData.push_back(it->b);

                if (it->drawPoints){
                    numPointsOfSegments++;
                    pointsOfSegmentsCoordData.push_back(it->begin.x);
                    pointsOfSegmentsCoordData.push_back(it->begin.y);
                    pointsOfSegmentsCoordData.push_back(it->begin.z);
                    
                    pointsOfSegmentsCoordData.push_back(it->end.x);
                    pointsOfSegmentsCoordData.push_back(it->end.y);
                    pointsOfSegmentsCoordData.push_back(it->end.z);
                    
                    pointsOfSegmentsColorData.push_back(it->begin.r);
                    pointsOfSegmentsColorData.push_back(it->begin.g);
                    pointsOfSegmentsColorData.push_back(it->begin.b);

                    pointsOfSegmentsColorData.push_back(it->end.r);
                    pointsOfSegmentsColorData.push_back(it->end.g);
                    pointsOfSegmentsColorData.push_back(it->end.b);
                }

            }

            glBindBuffer(GL_ARRAY_BUFFER, idSegmentsCoordBuffer);
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW); //Release previous memory
            glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(coord_type) * segments.size(),
                         &segmentsCoordData[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, idSegmentsColorBuffer);
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW); //Release previous memory
            glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float) * segments.size(),
                         &segmentsColorData[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, idPointsOfSegmentsCoordBuffer);
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW); //Release previous memory
            glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(coord_type) * numPointsOfSegments,
                         &pointsOfSegmentsCoordData[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, idPointsOfSegmentsColorBuffer);
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW); //Release previous memory
            glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float) * numPointsOfSegments,
                         &pointsOfSegmentsColorData[0], GL_STATIC_DRAW);

        }
    }

}

void computeNormals(){
    vNormals.clear();
    vNormals.resize(points.size(), Point());
    pointFromTriangle.clear();
    pointFromTriangle.resize(points.size(), false);

    for (vector<Triangle>::iterator it = triangles.begin(); it < triangles.end(); ++it){
        if (it->isCorrect()){
            it->computeNormal();
            for (int i = 0; i < 3; ++i){
                int pId = it->pointsId[i];
                vNormals[pId] += it->normal;
                pointFromTriangle[pId] = true;
            }
        }
        else {
            cerr << "There's a malformed triangle: (" << it->pointsId[0] << ", "
                 << it->pointsId[1] << ", " << it->pointsId[2] << ")" << endl;
        }
    }

    int npoint = 0;
    for (vector<Point>::iterator it = vNormals.begin(); it < vNormals.end(); ++it, ++npoint){
        if (pointFromTriangle[npoint]){
            it->normalize();
        }
    }
}
