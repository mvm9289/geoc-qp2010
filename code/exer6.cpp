#include "exer6.h"
#include "data.h"
#include "art_gallery.h"
#include <vector>

using namespace std;

unsigned int nextPolygon = 0;
vector<int> segmentsQuantity;
vector<int> pointsQuantity;

bool getLastPolygon(Polygon &polygon)
{
    if (polygons.size() == 0) return false;
    
    polygon = polygons[polygons.size() - 1];
    return true;
}

void makeArtGallery(const Polygon& polygon)
{
    ArtGallery artGallery(polygon);
    
    vector<LineSegment> diagonals = artGallery.getDiagonals();
    vector<Point> guardians = artGallery.getGuardians();
    
    int n = diagonals.size();
    for (int i = 0; i < n; i++)
        addSegment(diagonals[i]);
	
    segmentsQuantity.push_back(n);
	
    n = guardians.size();
    for (int i = 0; i < n; i++)
        addPoint(guardians[i]);
    
    pointsQuantity.push_back(n);
}

void exer6stepBack()
{
    if (nextPolygon) nextPolygon--;
    
    int aux = segmentsQuantity.back();
    segmentsQuantity.pop_back();
    for (int i = 0; i < aux; i++) segments.pop_back();
    
    aux = pointsQuantity.back();
    points.pop_back();
    for (int i = 0; i < aux; i++) points.pop_back();
}

void exer6reset()
{
    nextPolygon = 0;
    segments.clear();
    points.clear();
}

void exer6initialization(bool fromFile)
{
    if (fromFile)
    {
        int n = polygons.size();
        for (int i = 0; i < n; i++) makeArtGallery(polygons[i]);
    }
}

void exer6executeTest()
{
    Polygon polygon;
    
    if (getLastPolygon(polygon)) makeArtGallery(polygon);
}
