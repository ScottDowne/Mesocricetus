#ifndef _API_PRIMITIVE_SET_H_
#define _API_PRIMITIVE_SET_H_

/* APIGraphic Definition - Direct API Branch
 *
 * APIGraphic.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "DirectAPISettings.h" // for graphics api headers
#include "iAPIGraphic.h"       // for APIVertexList, APIGraphic Interfaces

//-------------------------------- APIGraphic ----------------------------
//
// The APIGraphic class implements the graphics at the API level
//
class APIGraphic : public iAPIGraphic {

  protected:
    static LPDIRECT3DDEVICE9 d3dd;        // points to the display device

    virtual ~APIGraphic() {}

  public:
    static void connectsThrough(void* d) { d3dd = (LPDIRECT3DDEVICE9)d; }
    void Delete() const { delete this; }
};

// The APIPrimitiveSet class implements the primitive set at the API level
//
class APIPrimitiveSet : public APIGraphic {

    int                      nPrimitives; // number of primitives
    D3DPRIMITIVETYPE         type;        // primitive type
    LPDIRECT3DVERTEXBUFFER9  vb;          // points to the vertex buffer
    iVertexList*             vertexList;  // points to coordinator vertex list
    int                      vertexSize;  // size of a single vertex
    unsigned                 vertexFrmt;  // vertex format
    int                      bufferSize;  // size of the vertex buffer
    int                      nVertices;   // number of vertices
    VertexType               vertexType;  // type of vertex in vertex buffer

    APIPrimitiveSet(PrimitiveType t, int np, VertexType vt, iVertexList*);
    virtual ~APIPrimitiveSet();
    void    setup();

public:
    friend iAPIGraphic* CreateAPIPrimitiveSet(PrimitiveType, int, 
     VertexType, iVertexList*);
    APIPrimitiveSet(const APIPrimitiveSet&);
    APIPrimitiveSet& operator=(const APIPrimitiveSet&);
    iAPIGraphic* clone() const { return new APIPrimitiveSet(*this); }
    void draw();
    void suspend();
    void release();
};

#endif
