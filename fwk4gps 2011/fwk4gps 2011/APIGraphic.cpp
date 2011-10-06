/* APIGraphic Implementation - Direct API Branch
 *
 * APIGraphic.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCoordinator.h"    // for the Coordinator Interface
#include "iGraphic.h"        // for the Graphic Interface
#include "iUtilities.h"      // for error()

#include "APIGraphic.h"      // for the APIGraphic class definition
#include "MathDefinitions.h" // for Vector and MODEL_Z_AXIS
#include "Configuration.h"   // for POINT_LIST, ...

//-------------------------------- APIGraphic ---------------------------------
//
// The APIGraphic class hierarchy implements the Graphics at the API level
//
// The API connectivity is defined by the Display object
//
LPDIRECT3DDEVICE9 APIGraphic::d3dd = NULL;

//-------------------------------- APIPrimitiveSet ----------------------------
//
// The APIPrimitiveSet class hierarchy implements the Primitive Set Graphics
// at the API level
//
// CreateAPIPrimitiveSet creates an APIPrimitiveSet object
//
iAPIGraphic* CreateAPIPrimitiveSet(PrimitiveType t, int np, VertexType vt,
 iVertexList* v) {

    return new APIPrimitiveSet(t, np, vt, v);
}

// constructor initializes instance variables and allocates memory for the
// list
//
APIPrimitiveSet::APIPrimitiveSet(PrimitiveType t, int np, VertexType vt,
 iVertexList* v) : nPrimitives(np), vertexList(v), vertexType(vt), vb(NULL),
 bufferSize(0), nVertices(0) {

    // convert to Direct3D type
    switch (t) {
        case POINT_LIST    : type = D3DPT_POINTLIST;     break;
        case LINE_LIST     : type = D3DPT_LINELIST;      break;
        case LINE_STRIP    : type = D3DPT_LINESTRIP;     break;
        case TRIANGLE_LIST : type = D3DPT_TRIANGLELIST;  break;
        case TRIANGLE_STRIP: type = D3DPT_TRIANGLESTRIP; break;
        case TRIANGLE_FAN  : type = D3DPT_TRIANGLEFAN;   break;
        default            : type = D3DPT_POINTLIST;
    }
    switch (vt) {
    case COLOURED_VERTEX : 
        vertexSize = 16;
        vertexFrmt = D3DFVF_XYZ | D3DFVF_DIFFUSE;
        break;
    case LIT_VERTEX :
        vertexSize = 32;
        vertexFrmt = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
        break;
    default:
        vertexSize = 12;
        vertexFrmt = D3DFVF_XYZ;
    }
}

APIPrimitiveSet::APIPrimitiveSet(const APIPrimitiveSet& src) {
    
    vb     = NULL;
    *this  = src;
}

APIPrimitiveSet& APIPrimitiveSet::operator=(const APIPrimitiveSet& src) {

    if (this != &src) {
        vertexList  = src.vertexList;
        vertexSize  = src.vertexSize;
        vertexFrmt  = src.vertexFrmt;
        bufferSize  = src.bufferSize;
        nPrimitives = src.nPrimitives;
        type        = src.type;
        vertexType  = src.vertexType;
        if (vb) {
            vb->Release();
            vb = NULL;
        }
    }

    return *this;
}

// setup creates the vertex buffer and populates it
//
void APIPrimitiveSet::setup() {

    nVertices  = vertexList->noVertices();
    bufferSize = vertexSize * nVertices;

	// create the vertex buffer
    if (FAILED(d3dd->CreateVertexBuffer(bufferSize, 0, vertexFrmt, 
     D3DPOOL_DEFAULT, &vb, NULL))) {
        error(L"APIPrimitiveSet::10 Couldn\'t create the vertex buffer");
        vb = NULL;
    }
    // populate the newly created vertex buffer
    else {
        void* pv;
        if (SUCCEEDED(vb->Lock(0, bufferSize, &pv, 0)))
            for (int i = 0; i < nVertices; i++)
                vertexList->populate(i, &pv, vertexType);
        vb->Unlock();
    }
}

// draw draws the stream of vertices
//
void APIPrimitiveSet::draw() {

    if (!vb) setup();

    if (vb) {
        d3dd->SetFVF(vertexFrmt);
        d3dd->SetStreamSource(0, vb, 0, vertexSize);
        d3dd->DrawPrimitive(type, 0, nPrimitives);
    }
}

// suspend releases the interface to the vertex buffer
//
void APIPrimitiveSet::suspend() {

    if (vb) {
        vb->Release();
        vb = NULL;
    }
}

// release suspends the vertex buffer
//
void APIPrimitiveSet::release() {

    suspend();
}

// destructor releases the vertex buffer
//
APIPrimitiveSet::~APIPrimitiveSet() {

    release();
}

