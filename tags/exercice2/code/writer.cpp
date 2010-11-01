#include "writer.h"
#include "point.h"
#include "line_segment.h"
#include "polygon.h"
#include "circumference_3p.h"
#include "data.h"
#include <iostream>
#include <fstream>

using namespace std;

const char *outFile = "output.txt"; //Default file

void writeToFile(){
    ofstream os(outFile, ofstream::out);
    if (!os.is_open()){
        cerr << "Couldn't open file " << outFile << "." << endl;
    }

    os << fixed;

    if (!points.empty()){
        os << "p" << endl;
        for (vector<Point>::const_iterator it = points.begin(); it < points.end(); ++it){
            os << *it;
        }
    }

    if (!segments.empty()){
        os << "s" << endl;
        for (vector<LineSegment>::const_iterator it = segments.begin(); it < segments.end(); ++it){
            os << *it;
        }
    }

    if (!polygons.empty()){
        os << "P" << endl;
        for (vector<Polygon>::const_iterator it = polygons.begin(); it < polygons.end(); ++it){
            os << *it;
        }
    }

    if (!circumferences.empty()){
        os << "c" << endl;
        for (vector<Circum3P>::const_iterator it = circumferences.begin(); it < circumferences.end(); ++it){
            os << *it;
        }
    }

    if (!triangles.empty()){
        os << "t" << endl;
        for (vector<Triangle>::const_iterator it = triangles.begin(); it < triangles.end(); ++it){
            os << *it;
        }
    }

    if (!boundary.empty()){
        os << "b" << endl;
        for (vector<vector<int> >::const_iterator it = boundary.begin(); it != boundary.end(); ++it){
            for (vector<int>::const_iterator iit = it->begin(); iit != it->end(); ++iit){
                os << *iit << endl;
            }
            os << endl;
        }
    }
}
