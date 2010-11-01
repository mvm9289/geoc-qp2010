#include "circumference_3p.h"
#include "data.h"
#include "line_segment.h"
#include "parser.h"
#include "point.h"
#include "polygon.h"
#include <iostream>
#include <fstream>

enum ReadingMode {
    MODE_POINT,
    MODE_SEGMENT,
    MODE_POLYGON,
    MODE_CIRCUM,
    MODE_TRIANGLE,
    MODE_BOUNDARY
};

using namespace std;

bool inFileRead = false;
const char *inFile;

istream& operator>>(istream &is, Point &p) throw (string);
istream& operator>>(istream &is, LineSegment &s) throw (string);
istream& operator>>(istream &is, Polygon &pol) throw (string);
istream& operator>>(istream &is, Circum3P &cir) throw (string);
istream& operator>>(istream &is, Triangle &tri) throw (string);

bool discardSpacesAndComments(istream &is);
bool isFirstOfPoint(const char c);
void readBoundary(istream &is);
void readOptions(istream &is, Options &opt, const char *name) throw (string);

int line;
char strLine[20];

//returns true if there was some error reading the file
bool readFile(const char* file){
    ifstream is(file, ifstream::in);
    if (!is.is_open()){
        return true;
    }

    line = 1;
    ReadingMode mode = MODE_POINT;
    bool first = true;

    while (is.good()){
        discardSpacesAndComments(is);
        if (is.good()){
            char next = is.peek();
            if (next == '\n' || next == '\r'){
                is.get();
                if (next == '\r' && is.peek() == '\n') is.get();
                line++;
            }
            else if (isFirstOfPoint(next)){
                try {
                    switch(mode){
                    case MODE_POINT:{
                        Point p;
                        is >> p;
                        if (first){
                            bbox = BoundingBox(p);
                            first = false;
                        }
                        addPoint(p);
                    }break;
                    case MODE_SEGMENT:{
                        LineSegment s;
                        is >> s;
                        if (first){
                            bbox = BoundingBox(s.begin);
                            first = false;
                        }
                        addSegment(s);
                    }break;
                    case MODE_POLYGON:{
                        Polygon pol;
                        is >> pol;
                        if (first){
                            bbox = BoundingBox(pol.points[0]);
                            first = false;
                        }
                        addPolygon(pol);
                    }break;
                    case MODE_CIRCUM:{
                        Circum3P cir;
                        is >> cir;
                        cir.computeCircum();
                        if (cir.drawable){
                            if (first){
                                bbox = BoundingBox(cir.points[0]);
                                first = false;
                            }
                            addCircumference(cir);
                        }
                        else {
                            cerr << "[line " << line - 1 
                                 << "] Non drawable circumference discarded." << endl;
                        }
                    }break;
                    case MODE_TRIANGLE:{
                        Triangle tri;
                        is >> tri;
                        addTriangle(tri);
                        
                    }break;
                    case MODE_BOUNDARY:
                        readBoundary(is);
                        break;
                    default:
                        break;
                    }
                }
                catch(string msg){
                    cerr << msg << endl;
                    is.close();
                    return true;
                }
            }
            else {
                switch(next){
                case 'p':
                    mode = MODE_POINT;
                    break;
                case 's':
                    mode = MODE_SEGMENT;
                    break;
                case 'P':
                    mode = MODE_POLYGON;
                    break;
                case 'c':
                    mode = MODE_CIRCUM;
                    break;
                case 't':
                    mode = MODE_TRIANGLE;
                    break;
                case 'b':
                    mode = MODE_BOUNDARY;
                    break;
                default:
                    cerr << "[line " << line << "] The character '" << next;
                    cerr << "' is not allowed here." << endl;
                    is.close();
                    return true;
                }
                //Consume the mode-changing letter
                is.get();
            }
        }
    }
    is.close();
    
    return false;
}

istream& operator>>(istream &is, Point &p) throw (string){
    bool finish = false;
    int read = 0;

    while (!finish && is.good()){
        //If we have read part of a point and we find a comment, we finish.
        finish = discardSpacesAndComments(is) && (read > 0);
        if (!finish && is.good()){
            char next = is.peek();
            if (next == '\n' || next == '\r'){
                is.get();
                if (next == '\r' && is.peek() == '\n') is.get();
                finish = true;
                line++;
            }
            else if (read < 3 && isFirstOfPoint(next)){
                switch (read){
                case 0:
                    is >> p.x;
                    read = 1;
                    break;
                case 1:
                    is >> p.y;
                    read = 2;
                    break;
                case 2:
                    is >> p.z;
                    p.z *= 10;
                    read = 3;
                    break;
                default:
                    break;
                }
            }
            //Color belongs to [0,1]
            else if (read >= 3 && (next == '0' || next == '1' || next == '.')){
                switch (read){
                case 3:
                    is >> p.r;
                    read = 4;
                    break;
                case 4:
                    is >> p.g;
                    read = 5;
                    break;
                case 5:
                    is >> p.b;
                    read = 6;
                    break;
                default:
                    sprintf(strLine,"%i", line);
                    throw string("[line ") + strLine
                                 + "] Problem reading point: too much options.";
                }
            }
            else {
                sprintf(strLine,"%i", line);
                throw string("[line ") + strLine + "] The character '"
                    + next + "' is not allowed here.";
            }
        }
    }
    if (read < 2){
        //As we have finished the point, we are refering to the last line
        sprintf(strLine,"%i", line - 1);
        throw string("[line ") + strLine
                     + "] Problem reading the point: line ended before full point was read.";
    }
    else if (read > 3 && read < 6){
        sprintf(strLine, "%i", line - 1);
        throw string("[line ") + strLine + "] Problem reading point: incomplete options found.";
    }

    return is;
}


istream& operator>>(istream &is, LineSegment &s) throw (string){
    try {
        is >> s.begin;
    }
    catch (string msg){
        throw "Problem reading first point of segment:\n\t" + msg;
    }
    try {
        is >> s.end;
    }
    catch (string msg){
        throw "Problem reading second point of segment:\n\t" + msg;
    }

    bool finish = false;

    while (!finish && is.good()){
        discardSpacesAndComments(is);
        if (!finish && is.good()){
            char next = is.peek();
            if (next == 'o'){
                is.get();
                readOptions(is, s, "line segment");
                finish = true;
            }
            else if (next == '\n' || next == '\r'){//newline ends the segment
                is.get();
                if (next == '\r' && is.peek() == '\n') is.get();
                finish = true;
                line++;
            }
            else {
                finish = true;
            }
        }
    }
    return is;
}


istream& operator>>(istream &is, Polygon &pol) throw (string){

    bool finish = false;
    int numPoints = 0;

    while(!finish && is.good()){
        discardSpacesAndComments(is);
        if (!finish && is.good()){
            char next = is.peek();
            if (isFirstOfPoint(next)){
                Point p;
                try {
                    is >> p;
                }
                catch (string msg){
                    throw "There was some problem reading the polygon:\n\t" + msg;
                }
                pol.points.push_back(p);
                numPoints++;
            }
            else if (next == 'o'){
                is.get();
                readOptions(is, pol, "polygon");
                finish = true;
            }
            else if (next == '\n' || next == '\r'){
                is.get();
                if (next == '\r' && is.peek() == '\n') is.get();
                finish = true;
                line++;
            }
            else {
                finish = true;
            }
        }
    }
    if (numPoints < 3){
        sprintf(strLine,"%i", line - 1);
        throw string("[line ") + strLine + "] There was some problem reading the polygon: it should have at least 3 points.";
    }
    return is;
}


istream& operator>>(istream &is, Circum3P &cir) throw (string){

    for (int i = 0; i < 3; i++){
        try {
            is >> cir.points[i];
        }
        catch(string msg){
            sprintf(strLine, "%i", line);
            throw string("Problem reading point of circumference (") + strLine + "):\n\t" + msg;
        }
    }

    bool finish = false;

    while (!finish && is.good()){
        discardSpacesAndComments(is);
        if (!finish && is.good()){
            char next = is.peek();
            if (next == 'o'){
                is.get();
                readOptions(is, cir, "circumference");
                finish = true;
            }
            else if (next == '\n' || next == '\r'){//newline ends the circumference
                is.get();
                if (next == '\r' && is.peek() == '\n') is.get();
                finish = true;
                line++;
            }
            else {
                finish = true;
            }
        }
    }
    return is;
}

istream& operator>>(istream &is, Triangle &tri) throw (string){
    bool finish = false;
    int numRead = 0;

    while (!finish && is.good()){
        finish = discardSpacesAndComments(is) && numRead > 0;
        if (!finish && is.good()){
            char next = is.peek();
            if (next >= '0' && next <= '9'){
                if (numRead < 3){
                    is >> tri.pointsId[numRead];
                    numRead++;
                }
                else {
                    sprintf(strLine, "%i", line);
                    throw string("[line ") + strLine +
                        "] Problem reading triangle: too much points.";
                }
            }
            else if (next == '\n' || next == '\r'){
                is.get();
                if (next == '\r' && is.peek() == '\n') is.get();
                if (numRead == 3){
                    line++;
                    finish = true;
                }
                else {
                    sprintf(strLine, "%i", line);
                    throw string("[line ") + strLine +
                        "] Problem reading triangle: not enough points.";
                }
            }
            else {
                sprintf(strLine, "%i", line);
                throw string("[line ") + strLine +
                    "] The character " + next + " is not allowed here.";
            }
        }

    }
    return is;
}

//Test whether the character belongs to "First" of Point.
bool isFirstOfPoint(const char c){
    return ((c >= '0' && c <= '9')
            || c == '.' 
            || c == '-');
}

// Discard next spaces and possibly a comment. Returns true if it
// found a comment.
bool discardSpacesAndComments(istream &is){
    bool finish = false;
    bool comment = false;
    while (!finish && is.good()){
        char next = is.peek();
        if (next == '/'){
            while (next != '\n' && next != '\r'){
                next = is.get();
            }
            if (next == '\r' && is.peek() == '\n') is.get(); //Windows EOL
            comment = true;
            line++;
        }
        else if (next == ' ' || next == '\t'){
            is.get();
        }
        else {
            finish = true;
        }
    }
    return comment;
}

void readBoundary(istream &is){
    bool finish = false;
    bool watchForBlankLine = false;
    vector<int> currVec;

    while (is.good() && !finish){
        discardSpacesAndComments(is);
        if (is.good()){
            char next = is.peek();
            if (next == '\n'){
                is.get();
                line++;
                if (watchForBlankLine){
                    watchForBlankLine = false;
                    boundary.push_back(currVec);
                    currVec.clear();
                }
                else {
                    watchForBlankLine = true;
                }
            }
            else if (next == '\r'){
                is.get();
                line++;
                if (is.peek() == '\n'){
                    is.get();
                }
                if (watchForBlankLine){
                    watchForBlankLine = false;
                    boundary.push_back(currVec);
                    currVec.clear();
                }
                else {
                    watchForBlankLine = true;
                }
            }
            else if (next >= '0' && next <= '9'){
                watchForBlankLine = false;
                int num;
                is >> num;
                currVec.push_back(num);
            }
            else {
                finish = true;
            }
        }
    }
}

void readOptions(istream &is, Options &opt, const char *name) throw (string){
    int options = 0;
    bool finish = false;

    while (!finish && is.good()){
        finish = discardSpacesAndComments(is);
        if (!finish && is.good()){
            char next = is.peek();
            if (next == '\n' || next == '\r'){
                is.get();
                if (next == '\r' && is.peek() == '\n') is.get();
                finish = true;
                line++;
            }
            else if (options < 3 && (next == '0' || next == '1' || next == '.')){
                switch (options){
                case 0:
                    is >> opt.r;
                    break;
                case 1:
                    is >> opt.g;
                    break;
                case 2:
                    is >> opt.b;
                    break;
                default:
                    break;
                }
                options++;
            }
            else if (options == 3 && (next == '0' || next == '1')){
                is >> opt.drawPoints;
                options++;
            }
            else {
                sprintf(strLine, "%i", line);
                throw string("[line ") + strLine + "] The character '" + next
                    + "' is not allowed here.";
            }
        }
    }
    if (options != 4){
        sprintf(strLine, "%i", line - 1);
        throw string("[line ") + strLine + "] Problem reading the "
            + name + ": malformed options found.";
    }
}
