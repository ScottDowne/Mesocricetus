#ifndef _I_GRAPHIC_H_
#define _I_GRAPHIC_H_

/* Graphic Interface - Model Branch
 *
 * iGraphic.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include <list>

//-------------------------------- iVertexList --------------------------------
//
enum VertexType;
struct Vector;

class iVertexList {
  public:
    virtual int    add(const Vector& p, const Vector& n, float u, float v) = 0;
    virtual iVertexList* clone() const                                     = 0;
    virtual void   populate(int, void**, VertexType) const                 = 0;
    virtual int    noVertices() const                                      = 0;
    virtual Vector position(int) const                                     = 0;
    virtual void   Delete() const = 0;
};

iVertexList* CreateVertexList(int no);

//-------------------------------- iGraphic ------------------------------
//
// iGraphic is the Interface to the Graphic hierarchy
//
enum PrimitiveType;

class iGraphic {
  public:
    virtual iGraphic* clone() const                                 = 0;
    virtual void      draw()                                        = 0;
    virtual void      suspend()                                     = 0;
    virtual void      release()                                     = 0;
	virtual void      Delete() const                                = 0;
};

iGraphic* CreatePrimitiveSet(PrimitiveType, int, VertexType);

iGraphic* CreateBox(float minx, float miny, float minz, float maxx, 
 float maxy, float maxz);

iGraphic* CreateFaces(float minx, float miny, float minz, float maxx, 
 float maxy, float maxz, int front, int right, int back, int left, int bottom, int top,
 int subdivide = 0);

iGraphic* CreateGrid(int min, int y, int max, int n);

iGraphic* CreateRectangleList(float minx, float miny, float maxx, 
 float maxy);

#endif
