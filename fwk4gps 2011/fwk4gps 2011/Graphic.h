#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

/* Graphic Definition - Model Branch
 *
 * Graphic.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iGraphic.h" // for the Graphic Interface

//-------------------------------- Vertex -------------------------------------
//
// The Vertex class defines the structure for a single vertex
//
class Vertex {

    float x;   // x coordinate in the local frame
    float y;   // y coordinate in the local frame
    float z;   // z coordinate in the local frame
    float nx;  // x coordinate of normal in the local frame
    float ny;  // y coordinate of normal in the local frame
    float nz;  // z coordinate of normal in the local frame
    float tu;  // u coordinate of texture
    float tv;  // v coordinate of texture

  public:
    Vertex();
    Vertex(const Vector& p, const Vector& n, float ttu = 0, float ttv = 0);
    void   populate(void**, VertexType) const;
    Vector position() const;
};

//-------------------------------- VertexList ---------------------------------
//
// The VertexList class defines the structure of a set of vertices
//
class VertexList : public iVertexList {

    int     maxNo;        // maximum number of vertices
    int     no;           // number of vertices stored
    Vertex* vertex;       // points to the vertex list 

    virtual ~VertexList();

  public:
    friend iVertexList* CreateVertexList(int no);
    VertexList() : vertex(0), maxNo(0), no(0) {}
    VertexList(int no);
    VertexList(const VertexList&);
    VertexList& operator=(const VertexList&);
    VertexList* clone() const { return new VertexList(*this); }
    int    add(const Vector& p, const Vector& n, float u, float v);
    void   populate(int, void**, VertexType) const;
    int    noVertices() const { return no; }
    Vector position(int i) const;
    void   Delete() const { delete this; }
};

//-------------------------------- Graphic -------------------------------
//
// The Graphic class defines the structure of the graphic representations
//
class iCoordinator;
class iAPIGraphic;

class Graphic : public iGraphic {

    static iCoordinator* coordinator; // points to the coordinator

protected:
    
    Graphic();
    virtual ~Graphic();

public:
	static void isCoordinatedBy(iCoordinator* c)  { coordinator = c; }
    void Delete() const { delete this; }
};

// The PrimitiveSet class defines the structure of the primitive set
// graphic representations
//
class PrimitiveSet : public Graphic {

    iAPIGraphic* apiPrimitiveSet; // points to the API Primitive Set
    iVertexList* vertexList;      // points to the vertex list

    PrimitiveSet(PrimitiveType, int, VertexType);
    virtual ~PrimitiveSet();

public:
    friend iGraphic* CreatePrimitiveSet(PrimitiveType, int, VertexType);
    PrimitiveSet(const PrimitiveSet&);
    PrimitiveSet& operator=(const PrimitiveSet&);
    iGraphic* clone() const { return new PrimitiveSet(*this); }
    Vector getPosition(int i) const;
    virtual int  add(const Vector&, const Vector&, float = 0, float = 0);
    virtual void add(const Vector&, const Vector&, const Vector&, 
     const Vector&, const Vector&, int = 2);
    virtual void subDivide(const Vector&, const Vector&, const Vector&, 
     const Vector&, int, bool = true);
    void draw();
    void suspend();
    void release();
};

#endif
