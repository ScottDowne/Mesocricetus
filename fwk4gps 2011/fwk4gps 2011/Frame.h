#ifndef _FRAME_H_
#define _FRAME_H_

/* Frame Definition - Model Branch
 *
 * Frame.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iFrame.h"           // for the Frame Interface
#include "MathDeclarations.h" // for Matrix

//-------------------------------- Frame --------------------------------------
//
// The Frame class defines a reference frame in the Model Branch
//
class Frame : public iFrame {

    Matrix T;       // relative transformation wrt parent frame or world space
    iFrame* parent; // points to parent frame, if any

  public:
    Frame();
	void   scale(float sx, float sy, float sz)   { T.scale(sx, sy, sz); }
	void   rotatex(float rad)                    { T.rotatex(rad); }
    void   rotatey(float rad)                    { T.rotatey(rad); }
    void   rotatez(float rad)                    { T.rotatez(rad); }
    void   rotate(const Vector& axis, float rad) { T.rotate(axis, rad); }
	void   translate(float x, float y, float z)  { T.translate(x, y, z); }
    void   orient(const Matrix& rot)             { T.orient(rot); }
    Vector position() const;
    Matrix rotation() const;
	Vector orientation(const Vector& v) const;
	Vector orientation(char c) const;
    Matrix world() const;
	void   attachTo(iFrame* newParent);
    virtual ~Frame() {}
};

//-------------------------------- BoundedVolume ----------------------------
//
// A Bounded Volume is a Frame that has a Volume
//
class BoundedVolume : public Frame, public iBoundedVolume {

    bool   sphere;
    bool   plane;
    bool   axisAligned;
    float  radius;
    Vector normal;
    Vector minimum;
    Vector maximum;

public:
    BoundedVolume() : sphere(false), plane(false), axisAligned(false),
     radius(0) {}
    void  setRadius(float r);
    void  setRadius(float x, float y, float z);
    float getRadius() const { return radius; }
    void  setPlane(Vector n, float d);
    void  setAxisAligned(Vector min, Vector max);
    friend bool collision(const BoundedVolume* f1, const BoundedVolume* f2);
};

#endif