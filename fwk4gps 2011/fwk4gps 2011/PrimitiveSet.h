#ifndef _PRIMITIVE_SET_H_
#define _PRIMITIVE_SET_H_

/* PrimitiveSet Definition - Model Branch
 *
 * PrimitiveSet.h
 * fwk4gps version 2.0
 * gam666/dps901
 * Sept 2 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iPrimitiveSet.h"    // for the PrimitiveSet Interface

//-------------------------------- Vertex -------------------------------------
//
// The Vertex class defines the structure for a single vertex in the model
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
// The VertexList class defines the structure of a set of vertices in the model
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

//-------------------------------- PrimitiveSet -------------------------------
//
// The PrimitiveSet class defines the structure of a set of primitives
//
class iModel;
class iAPIPrimitiveSet;

class PrimitiveSet : public iPrimitiveSet {

    static iModel*    coordinator;     // points to the coordinator

    iAPIPrimitiveSet* apiPrimitiveSet; // points to the API Primitive Set
    iVertexList*      vertexList;      // points to the vertex list

protected:
    
    PrimitiveSet(PrimitiveType, int, VertexType);
    virtual ~PrimitiveSet();

public:
    friend iPrimitiveSet* CreatePrimitiveSet(PrimitiveType, int, VertexType);
	static void isCoordinatedBy(iModel* model)  { coordinator = model; }
    PrimitiveSet(const PrimitiveSet&);
    PrimitiveSet& operator=(const PrimitiveSet&);
    iPrimitiveSet* clone() const { return new PrimitiveSet(*this); }
    Vector getPosition(int i) const;
    virtual int  add(const Vector&, const Vector&, float = 0, float = 0);
    virtual void add(const Vector&, const Vector&, const Vector&, 
     const Vector&, const Vector&);
    void draw();
    void suspend();
    void release();
    void Delete() const { delete this; }
};

#endif
