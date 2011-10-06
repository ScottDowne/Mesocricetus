#ifndef _API_PRIMITIVE_SET_H_
#define _API_PRIMITIVE_SET_H_

/* APIPrimitiveSet Definition - Direct API Branch
 *
 * PrimitiveSet.h
 * fwk4gps version 2.0
 * gam666/dps901
 * Sept 2 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iAPIPrimitiveSet.h" // for APIVertexList, APIPrimitiveSet Interfaces
#include "GraphicsPlatform.h" // for graphics api headers

//-------------------------------- APIPrimitiveSet ----------------------------
//
// The APIPrimitiveSet class implements the primitive set at the API level
//
class APIPrimitiveSet : public iAPIPrimitiveSet {

    static LPDIRECT3DDEVICE9 d3dd;        // points to the display device

    int                      nPrimitives; // number of primitives
    D3DPRIMITIVETYPE         type;        // primitive type
    LPDIRECT3DVERTEXBUFFER9  vb;          // points to the vertex buffer
    iVertexList*             vertexList;  // points to model vertex list
    int                      vertexSize;  // size of a single vertex
    unsigned                 vertexFrmt;  // vertex format
    int                      bufferSize;  // size of the vertex buffer
    int                      nVertices;   // number of vertices
    VertexType               vertexType;  // type of vertex in vertex buffer

    APIPrimitiveSet(PrimitiveType t, int np, VertexType vt, iVertexList*);
    virtual ~APIPrimitiveSet();
    void    setup();

public:
    friend iAPIPrimitiveSet* CreateAPIPrimitiveSet(PrimitiveType, int, 
     VertexType, iVertexList*);
    APIPrimitiveSet(const APIPrimitiveSet&);
    APIPrimitiveSet& operator=(const APIPrimitiveSet&);
    static void connectsThrough(void* d) { d3dd = (LPDIRECT3DDEVICE9)d; }
    iAPIPrimitiveSet* clone() const { return new APIPrimitiveSet(*this); }
    void draw();
    void suspend();
    void release();
    void Delete() const { delete this; }
};

#endif
