#ifndef _I_FRAME_H_
#define _I_FRAME_H_

/* Frame Interface - Model Branch
 *
 * iFrame.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iFrame -------------------------------------
//
// iFrame is the Interface to the Frame class
//
struct Matrix;
struct Vector;

class iFrame {
  public:
    virtual void   translate(float x, float y, float z) = 0;
    virtual void   scale(float x, float y, float z)     = 0;
    virtual void   rotatex(float rad)                   = 0;
    virtual void   rotatey(float rad)                   = 0;
    virtual void   rotatez(float rad)                   = 0;
    virtual Vector position() const                     = 0;
	virtual Matrix rotation() const                     = 0;
    virtual Vector orientation(char axis) const         = 0;
	virtual Matrix world() const                        = 0;
    virtual void   attachTo(iFrame* parent)             = 0;
};

class iBoundedVolume {
  public:
    virtual void setRadius(float r)                     = 0;
    virtual void setRadius(float x, float y, float z)   = 0;
    virtual void setPlane(Vector n, float d)            = 0;
    virtual void setAxisAligned(Vector min, Vector max) = 0;
};

#endif
