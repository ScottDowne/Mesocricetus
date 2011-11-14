/* Graphic Implementation - Model Branch
 *
 * Graphic.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCoordinator.h"    // for the Coordinator Interface
#include "iAPIGraphic.h"     // for the APIGraphic Interface
#include "iUtilities.h"      // for error()

#include "Graphic.h"         // for Vertex, VertexList, Graphic class def
#include "MathDefinitions.h" // for Vector and MODEL_Z_AXIS
#include "Configuration.h"   // for POINT_LIST, ...

//-------------------------------- Vertex -------------------------------------
//
// Vertex holds the data for a single vertex
//
// constructors initialize the vertex to the values received
//
Vertex::Vertex() : x(0), y(0), z(0), nx(0), ny(0), nz(0), tu(0), tv(0) {}

Vertex::Vertex(const Vector& p, const Vector& n, float ttu, float ttv) :
 x(p.x), y(p.y), z(p.z * MODEL_Z_AXIS), nx(n.x), ny(n.y), nz(n.z), tu(ttu), 
 tv(ttv) {}

 // populate populates *pv with the data for the vertex depending on the
 // vertex type
 //
 void  Vertex::populate(void** pv, VertexType t) const {
    float* p = *(float**)pv;

    switch (t) {
        case COLOURED_VERTEX:
            *p++ = x;
            *p++ = y;
            *p++ = z;
            *pv  = p;
            break;
        case LIT_VERTEX:
            *p++ = x;
            *p++ = y;
            *p++ = z;
            *p++ = nx;
            *p++ = ny;
            *p++ = nz;
            *p++ = tu;
            *p++ = tv;
            *pv  = p;
            break;
    }
}

// position returns the position of the vertex in local coordinates
//
Vector Vertex::position() const {
    return Vector(x, y, z);
}

//-------------------------------- VertexList --------------------------------
//
// VertexList manages lists of vertices
//
iVertexList* CreateVertexList(int no) {

    return new VertexList(no);
}

VertexList::VertexList(int n) : maxNo(n), no(0) {

    if (n <= 0) {
        error(L"VertexList::01 Invalid number of vertices");
        maxNo  = 0;
        vertex = NULL;
    }
    else {
        maxNo  = n;
        vertex = new Vertex[maxNo];
    }
}

VertexList::VertexList(const VertexList& src) {
    
    vertex = NULL;
    *this  = src;
}

VertexList& VertexList::operator=(const VertexList& src) {

    if (this != &src) {
        maxNo = src.maxNo;
        no    = src.no;
        if (vertex) {
            delete [] vertex;
            vertex = NULL;
        }
        if (src.no) {
            vertex = new Vertex[src.no];
            for (int i = 0; i < no; i++)
                vertex[i] = src.vertex[i];
        }
    }

    return *this;
}

int VertexList::add(const Vector& p, const Vector& n, float u, float v) {
    int rc = no;

    if (no >= maxNo)
        error(L"VertexList::10 Vertex list is full");
    else if (!vertex)
        error(L"VertexList::11 Vertex memory not allocated");
    else
        vertex[no++] = Vertex(p, n, u, v);

    return rc;
}

// position retruns the position of vertex i
//
Vector VertexList::position(int i) const { 
    
    return vertex[i].position(); 
}

// populate populates the address pv with the data in vertex i
//
void VertexList::populate(int i, void** pv, VertexType t) const {

    if (i < no) vertex[i].populate(pv, t);
}

VertexList::~VertexList() {

    delete [] vertex;
}

//-------------------------------- Graphic ---------------------------------
//
// The Graphic class is the base class of the Graphic hierarchy
//
iCoordinator* Graphic::coordinator = NULL;

// constructor adds the Graphic to the coordinator
//
Graphic::Graphic() {

    if (!coordinator)
        error(L"Graphic::00 Couldn\'t access the Coordinator");
    else if(!coordinator->add(this))
        error(L"Graphic::01 Couldn\'t add object to the Coordinator");
}

// destructor removes the Graphic from the coordinator
//
Graphic::~Graphic() {

    if (coordinator)
        coordinator->remove(this);
    else
        error(L"Graphic::90 Couldn\'t access the Coordinator");
}

//-------------------------------- PrimitiveSet ---------------------------------
//
// CreatePrimitiveSet creates a PrimitiveSet object
//
iGraphic* CreatePrimitiveSet(PrimitiveType t, int np, VertexType vt) {

    return new PrimitiveSet(t, np, vt);
}

// constructor initializes instance variables and allocates memory for the
// list
//
PrimitiveSet::PrimitiveSet(PrimitiveType t, int np, VertexType vt) {

    int nv;
    switch (t) {
        case POINT_LIST:     nv = np;     break;
        case LINE_LIST:      nv = 2 * np; break;
        case LINE_STRIP:     nv = np + 1; break;
        case TRIANGLE_LIST:  nv = 3 * np; break;
        case TRIANGLE_STRIP: nv = np + 2; break;
        case TRIANGLE_FAN:   nv = np + 1; break;
        default: nv = np;
    }

    vertexList      = CreateVertexList(nv);
    apiPrimitiveSet = CreateAPIPrimitiveSet(t, np, vt, vertexList);
}

// copy constructor calls assignment operator
//
PrimitiveSet::PrimitiveSet(const PrimitiveSet& src) {

    apiPrimitiveSet = NULL;
    vertexList      = NULL;
    *this           = src;
}

// assignment operator
//
PrimitiveSet& PrimitiveSet::operator=(const PrimitiveSet& src) {

    if (this != &src) {
        if (apiPrimitiveSet)
            apiPrimitiveSet->Delete();
        if (vertexList)
            vertexList->Delete();
        apiPrimitiveSet = src.apiPrimitiveSet->clone();
        vertexList      = src.vertexList->clone();
    }

    return *this;
}

// add adds a single vertex to the vertex list of the primitive set
//
int PrimitiveSet::add(const Vector& p, const Vector& n, float u, float v) {

    return vertexList->add(p, n, u, v);
}

// add adds six vertices to the vertex list describing a quadrilateral that
// has been defined in terms of four Vectors and a normal vector - the 
// ordering of the vertices in each triangle of the quad exposes the front
// (visible) face of the triangle on the host platform
//
void PrimitiveSet::add(const Vector& p1, const Vector& p2, const Vector& p3, 
 const Vector& p4, const Vector& n) {

    vertexList->add(p1, n, 0, 0);
    vertexList->add(p2, n, 0, 1);
    vertexList->add(p3, n, 1, 1);
    vertexList->add(p1, n, 0, 0);
    vertexList->add(p3, n, 1, 1);
    vertexList->add(p4, n, 1, 0);
}

// getPosition returns the position of vertex i
//
Vector PrimitiveSet::getPosition(int i) const {

    return vertexList->position(i);
}

// draw draws the primitive set
//
void PrimitiveSet::draw() {

    apiPrimitiveSet->draw();
}

// suspend releases the interface to the vertex buffer
//
void PrimitiveSet::suspend() {

    apiPrimitiveSet->suspend();
}

// release suspends the vertex list and releases the vertex declaration
//
void PrimitiveSet::release() {

    suspend();
    apiPrimitiveSet->release();
}

// destructor release and deletes the vertex list
//
PrimitiveSet::~PrimitiveSet() {

    release();
    if (apiPrimitiveSet)
        apiPrimitiveSet->Delete();
    if (vertexList)
        vertexList->Delete();
}

//-------------------------------- Graphic Structures -------------------
//
// CreateBox builds a triangle vertex list for a brick-like box from
// two extreme points one face at a time with all faces having the same
// attributes
//
iGraphic* CreateBox(float minx, float miny, float minz, float maxx, 
 float maxy, float maxz) {
    
   return CreateBox(minx, miny, minz, maxx, maxy, maxz, 1,1,1,1,1,1);
}

iGraphic* CreateBox(float minx, float miny, float minz, float maxx, 
 float maxy, float maxz, int front, int right, int back, int left, int bottom, int top) {
    
    PrimitiveSet* primitiveSet = 
     (PrimitiveSet*)CreatePrimitiveSet(TRIANGLE_LIST, (front + right + back + left + bottom + top) * 2, LIT_VERTEX);

    /*float x = (minx + maxx) / 2;
    float y = (miny + maxy) / 2;
    float z = (minz + maxz) / 2;
    minx -= x;
    miny -= y;
    minz -= z;
    maxx -= x;
    maxy -= y;
    maxz -= z;*/
    // locate centroid at origin
    Vector p1 = Vector(minx, miny, minz),
           p2 = Vector(minx, maxy, minz),
           p3 = Vector(maxx, maxy, minz),
           p4 = Vector(maxx, miny, minz),
           p5 = Vector(minx, miny, maxz),
           p6 = Vector(minx, maxy, maxz),
           p7 = Vector(maxx, maxy, maxz),
           p8 = Vector(maxx, miny, maxz);
    if (front) primitiveSet->add(p1, p2, p3, p4, Vector(0, 0, -1)); // front
    if (right) primitiveSet->add(p4, p3, p7, p8, Vector(1, 0,  0)); // right
    if (back) primitiveSet->add(p8, p7, p6, p5, Vector(0, 0,  1)); // back
    if (left) primitiveSet->add(p6, p2, p1, p5, Vector(-1, 0, 0)); // left
    if (bottom) primitiveSet->add(p1, p4, p8, p5, Vector(0, -1, 0)); // bottom
    if (top) primitiveSet->add(p2, p6, p7, p3, Vector(0, 1,  0)); // top

    return primitiveSet;
}

std::list<iGraphic*>  CreateMaze(int mazeArray[][20], int colCount, int rowCount ) {
   
   std::list<iGraphic*> primitiveSets;
   int left, right, back, front;

   for (int y = 0; y < 2; y++) {

      for (int x = 0; x < rowCount; x++) {
      
         if ( mazeArray[y][x] == 0) {
         
            left = right = back = front = 0;

            if (x == 0 || mazeArray[y][x-1] > 0) {
            
               left = 1;
            }

            if (x == colCount-1 || mazeArray[y][x+1] > 0) {
            
               right = 1;
            }

            if (y == 0 || mazeArray[y+1][x] > 0) {
            
               back = 1;
            }

            if (y == rowCount-1 || mazeArray[y+1][x] > 0) {
            
               front = 1;
            }

            primitiveSets.push_back(CreateBox(x, colCount - y, 0, x+1, (colCount - y) + 1, 1,
                                              front, right, back, left, 0, 1));
         }
         else
         {
            primitiveSets.push_back(CreateBox(x, colCount - y - 1, 0, x+1, (colCount - y) + 1 - 1, 1,
                                              0, 0, 0, 0, 0, 1));
         }
 
      }
   }

   return primitiveSets;
}

// CreateGrid builds a grid-like line list one line at a time
//
iGraphic* CreateGrid(int min, int y, int max, int n) {
    
    PrimitiveSet* primitiveSet = 
     (PrimitiveSet*)CreatePrimitiveSet(LINE_LIST, 2 * (n + 1), LIT_VERTEX);

    int i, inc = (max - min) / (n - 1);
    for (i = min; i <= max; i += inc) {
        primitiveSet->add(Vector((float)min, (float)y, (float)i), Vector(0, 1, 0));
        primitiveSet->add(Vector((float)max, (float)y, (float)i), Vector(0, 1, 0));
        primitiveSet->add(Vector((float)i, (float)y, (float)min), Vector(0, 1, 0));
        primitiveSet->add(Vector((float)i, (float)y, (float)max), Vector(0, 1, 0));
    }

    return primitiveSet;
}

// CreateRectangleList builds a triangle list in the x-y plane from its two 
// extreme points
//
iGraphic* CreateRectangleList(float minx, float miny, float maxx, 
 float maxy) {
    
    PrimitiveSet* primitiveSet = 
     (PrimitiveSet*)CreatePrimitiveSet(TRIANGLE_LIST, 2, LIT_VERTEX);

    float x = (minx + maxx) / 2, y = (miny + maxy) / 2;
    minx -= x;
    miny -= y;
    maxx -= x;
    maxy -= y;
    Vector p1 = Vector(minx, miny, 0),
           p2 = Vector(minx, maxy, 0),
           p3 = Vector(maxx, maxy, 0),
           p4 = Vector(maxx, miny, 0);
    primitiveSet->add(p1, p2, p3, p4, Vector(0, 0, -1)); 
    
    return primitiveSet;
}
