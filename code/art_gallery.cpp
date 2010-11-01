#include "art_gallery.h"

ArtGallery :: ArtGallery(const Polygon& p)
{
    initialize(p);
    if (triangulate()) putGuardians();
    else
    {
        dDiagonals.clear();
        cerr << "Error: Maybe this is not a simple polygon" << endl;
    }
}

void ArtGallery :: initialize(const Polygon& p)
{
    vertices = Vertices(p.points.begin(), p.points.end());
    setAntiClockwise();
        
    int n = vertices.size();
    adjacencyList = BoolMatrix(n, BoolVector(n, false));
    for (int i = 0; i < n; i++)
    {
        int i1 = (i + 1)%n;
        adjacencyList[i][i1] = adjacencyList[i1][i] = true;
    }
}

bool ArtGallery :: triangulate()
{
    int n = vertices.size();
    
    IntVector index(n);
    for (int i = 0; i < n; i++) index[i] = i;
    
    BoolVector isEar(n);
    for (int i = 0; i < n; i++)
    {
        int i1 = (i + 1)%n;
        int i2 = (i + 2)%n;
        isEar[i1] = isInternalDiagonal(i, i2, index);
    }
    
    int m = n;
    
    while (m > 3)
    {
        int ear = m;
        for(int i = 0; i < m; i++)
            if (isEar[index[i]])
            {
                ear = i;
                break;
            }
        if (ear == m) return false;
        
        int earn1 = (ear + m - 1)%m;
        int ear1 = (ear + 1)%m;
        int earn2 = (ear + m - 2)%m;
        int ear2 = (ear + 2)%m;
        isEar[index[earn1]] = isInternalDiagonal(earn2, ear1, index);
        isEar[index[ear1]] = isInternalDiagonal(earn1, ear2, index);
        
        dDiagonals.push_back(LineSegment(vertices[index[earn1]], vertices[index[ear1]]));
        adjacencyList[index[earn1]][index[ear1]] = adjacencyList[index[ear1]][index[earn1]] = true;
        
        index.erase(index.begin() + ear);
        m--;
    }
    
    return true;
}

void ArtGallery :: putGuardians()
{
    int n = vertices.size();
    IntVector colours(n, 0);
    IntVector NColours(3, 0);
    bool finish = false;
    coloring(colours, NColours, 0, n, finish);
    
    int minColour = 1;
    if (NColours[1] < NColours[0]) minColour = 2;
    if (NColours[2] < NColours[minColour-1]) minColour = 3;
    
    for (int i = 0; i < n; i++)
        if (colours[i] == minColour) guardians.push_back(vertices[i]);
}

void ArtGallery :: setAntiClockwise()
{
    int n = vertices.size();
    int yMax = 0;
    for (int i = 1; i < n; i++)
        if (vertices[i].y > vertices[yMax].y) yMax = i;
    
    if (yMax != 0)
    {
        Vertices verticesAux(n);
        for (int i = 0; i < n; i++)
        {
            verticesAux[i] = vertices[yMax];
            yMax = (yMax + 1 + n)%n;
        }
        vertices = Vertices(verticesAux.begin(), verticesAux.end());
    }
    
    if (!isClockwise())
    {
        Vertices verticesAux(n);
    verticesAux[0] = vertices[0];
        for (int i = 1; i < n; i++)
            verticesAux[i] = vertices[n - i];
        vertices = Vertices(verticesAux.begin(), verticesAux.end());
    }
}

bool ArtGallery :: isClockwise()
{
    int n = vertices.size();
    int sDet = signDet(vertices[n - 1], vertices[0], vertices[1]);
    for (int i = 2; sDet == 0 && i < n; i++)
        sDet = signDet(vertices[i - 2], vertices[i - 1], vertices[i]);
    
    return sDet == 1;
}

bool ArtGallery :: isInternalDiagonal(int a, int b, const IntVector& index)
{
    return inCone(a, b, index) && isDiagonal(a, b, index);
}

bool ArtGallery :: inCone(int a, int b, const IntVector& index)
{
    int n = index.size();
    int a1 = (a + 1)%n;
    int an1 = (a + n - 1)%n;
    if (leftOn(vertices[index[an1]], vertices[index[a]], vertices[index[a1]]))
        return left(vertices[index[a]], vertices[index[b]], vertices[index[an1]]) && left(vertices[index[b]], vertices[index[a]], vertices[index[a1]]);
    
    return !(left(vertices[index[a]], vertices[index[b]], vertices[index[a1]]) && left(vertices[index[b]], vertices[index[a]], vertices[index[an1]]));
}

bool ArtGallery :: isDiagonal(int a, int b, const IntVector& index)
{
    int n = index.size();
    for (int k = 0; k < n; k++)
    {
        int k1 = (k + 1)%n;
        if (!((k == a) || (k1 == a) || (k == b) || (k1 == b)))
            if (intersect(vertices[index[a]], vertices[index[b]], vertices[index[k]], vertices[index[k1]]))
                return false;
    }
    
    return true;
}

bool ArtGallery :: intersect(const Point& a, const Point& b, const Point& c, const Point& d)
{
    if (intersectProperly(a, b, c, d)) return true;
    else if (between(a, b, c) || between(a, b, d) || between(c, d, a) || between(c, d, b))
        return true;
    
    return false;
}

bool ArtGallery :: intersectProperly(const Point& a, const Point& b, const Point& c, const Point& d)
{
    if (collinear(a, b, c) || collinear(a, b, d) || collinear(c, d, a) || collinear(c, d, b)) return false;
    
    return (left(a, b, c) ^ left(a, b, d)) && (left(c, d, a) ^ left(c, d, b));
}

bool ArtGallery :: between(const Point& a, const Point& b, const Point& c)
{
    if (!collinear(a, b, c)) return false;
    
    if (a.x != b.x)
        return ((a.x <= c.x) && (c.x <= b.x)) || ((a.x >= c.x) && (c.x >= b.x));
    
    return ((a.y <= c.y) && (c.y <= b.y)) || ((a.y >= c.y) && (c.y >= b.y));
}

bool ArtGallery :: leftOn(const Point& a, const Point& b, const Point& c)
{
    return signDet(a, b, c) >= 0;
}

bool ArtGallery :: left(const Point& a, const Point& b, const Point& c)
{
    return signDet(a, b, c) > 0;
}

bool ArtGallery :: collinear(const Point& a, const Point& b, const Point& c)
{
    return signDet(a, b, c) == 0;
}

double ArtGallery :: det(const Point& a, const Point& b, const Point& c)
{
    return b.x*c.y - c.x*b.y + c.x*a.y - a.x*c.y + a.x*b.y - b.x*a.y;
}

int ArtGallery :: signDet(const Point& a, const Point& b, const Point& c)
{
    double d = det(a, b, c);
    
    if (d == 0) return 0;
    if (d < 0) return -1;
    
    return 1;
}

void ArtGallery :: coloring(IntVector& colours, IntVector& NColours, int i, int n, bool& finish)
{
    if (i == n) finish = true;
    else
    {
        int c = 1;
        for (int k = 0; k < i; k++)
            if (adjacencyList[i][k] && colours[k] == c)
            {
                c++;
                k = -1;
            }
        
        while (c < 4 && !finish)
        {
            colours[i] = c;
            NColours[c - 1]++;
            coloring(colours, NColours, i + 1, n, finish);
            if (!finish)
            {
                NColours[c - 1]--;
                colours[i] = 0;
                c++;
                for (int k = 0; k < i; k++)
                    if (adjacencyList[i][k] && colours[k] == c)
                    {
                        c++;
                        k = -1;
                    }
            }
        }
    }
}

DrawableDiagonals ArtGallery :: getDiagonals()
{
    return dDiagonals;
}

Vertices ArtGallery :: getGuardians()
{
    return guardians;
}
