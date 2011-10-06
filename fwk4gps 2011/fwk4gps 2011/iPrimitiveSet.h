#ifndef _I_PRIMITIVE_SET_H_
#define _I_PRIMITIVE_SET_H_

/* PrimitiveSet Interface - Model Branch
 *
 * iPrimitiveSet.h
 * fwk4gps version 2.0
 * gam666/dps901
 * Sept 2 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

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

//-------------------------------- iPrimitiveSet ------------------------------
//
// iPrimitiveSet is the Interface to the Primitive Set hierarchy
//
enum PrimitiveType;

class iPrimitiveSet {
  public:
    virtual iPrimitiveSet* clone() const                                 = 0;
    virtual void draw()                                                  = 0;
    virtual void suspend()                                               = 0;
    virtual void release()                                               = 0;
	virtual void Delete() const                                          = 0;
};

extern "C"
iPrimitiveSet* CreatePrimitiveSet(PrimitiveType, int, VertexType);

extern "C"
iPrimitiveSet* CreateBox(float minx, float miny, float minz, float maxx, 
 float maxy, float maxz);

extern "C"
iPrimitiveSet* CreateGrid(int min, int y, int max, int n);

extern "C"
iPrimitiveSet* CreateRectangleList(float minx, float miny, float maxx, 
 float maxy);

#endif
