/* Frame Implementation - Model Branch
 *
 * Frame.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * Chris Szalwinski
 */

#include "Frame.h"           // for the Frame class definition
#include "MathDefinitions.h" // for position, rotation

//-------------------------------- Frame --------------------------------------
//
// The Frame object represents a reference frame in the Model Branch
//
Frame::Frame() : T(1), parent(0) { }

// position returns the Frame's position vector in world space
//
// Note that this method constructs the vector recursively
//
Vector Frame::position() const {

	return parent ? ::position(T) * parent->rotation() + parent->position() : 
	 ::position(T);
}

// rotation returns the Frame's orientation in world space
//
// Note that this method constructs the transformation recursively
//
Matrix Frame::rotation() const {

	return parent ? ::rotation(T) * parent->rotation() : ::rotation(T);
}

// orientation returns the orientation of local vector v in world space
//
Vector Frame::orientation(const Vector& v) const {

    return v * rotation();
}


// orientation returns the orientation of the local ? axis of the
// Frame in world space
//
Vector Frame::orientation(char axis) const {

    Vector v;

    switch(axis) {
        case 'x':
            v = Vector(1, 0, 0);
            break;
        case 'y':
            v = Vector(0, 1, 0);
            break;
        case 'z':
            v = Vector(0, 0, 1);
            break;
    }

    return orientation(v);
}

// world returns a reference to the homogeneous transformation of
// the Frame with respect to world space corrected for
// the coordinate system of the coordinator
//
// Note that this function constructs the transformation recursively
//
Matrix Frame::world() const {

    Matrix w = parent ? T * parent->world() : T;
    w.m43 *= MODEL_Z_AXIS;
    return w;
}

// attachTo attaches the current Frame to iFrame* newParent
//
// if a parent existed, recalculates the current position and orientation
// as world values and realigns the current Frame with the new parent, so 
// that the new transformation matrix holds the values relative to the new
// parent and the attachment is a smooth one; 
// if a parent did not exist, assumes that the current position and 
// orientation are relative to the new parent otherwise applies the 
// existing transformation 
//
void Frame::attachTo(iFrame* newParent) {

	// detach from current parent, if any
    if (parent)
        T = world();
    parent = 0;
    // attach to newParent
	parent = newParent;
    if (parent) {
        // convert rotation to a relative rotation wrt parent frame
		Matrix m = parent->rotation();
		m = m.transpose();
		T.rotate(m);
        // express offset in local coordinates wrt to parent frame
        Vector p = (::position(T) - parent->position()) * m;
        T.m41 = p.x;
        T.m42 = p.y;
        T.m43 = p.z;
    }
}

//-------------------------------- BoundedVolume ----------------------------
//
void BoundedVolume::setRadius(float r) { 
    radius = r; 
    sphere = true;
}

void BoundedVolume::setRadius(float x, float y, float z) { 
    setRadius(sqrtf(x*x + y*y + z*z)); 
}

void BoundedVolume::setPlane(Vector n, float d) { 
    plane  = true;
    normal = n;
    radius = d;
}

void BoundedVolume::setAxisAligned(Vector min, Vector max) {
    axisAligned = true;
    minimum     = min;
    maximum     = max;
}

bool collision(const BoundedVolume* f1, const BoundedVolume* f2) {

    bool collide = false;

    if (f1->sphere && f2->sphere) {
        float d = f1->radius + f2->radius;
        Vector separation = f1->position() - f2->position();
        collide = dot(separation, separation) <= d * d;
    }
    else if (f1->sphere && f2->plane) {
       collide = f1->radius + f2->radius > 
        dot(f2->normal, f1->position() - f2->position());
    }
    else if (f1->plane && f2->sphere) {
       collide = f1->radius + f2->radius > 
        dot(f1->normal, f2->position() - f1->position());
    }
    else if (f1->sphere && f2->axisAligned) {
    }
    else if (f1->axisAligned && f2->sphere) {
    }
    else if (f1->axisAligned && f2->axisAligned) {
    }

    return collide;
}
