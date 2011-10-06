#ifndef _I_OBJECT_H_
#define _I_OBJECT_H_

/* Object Interface - Model Branch
 *
 * iObject.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Frame.h" // for the BoundedVolume class definition

//-------------------------------- iObject ------------------------------------
//
// iObject is the Interface to the Object class
//
class  iTexture;
class  iGraphic;
struct Reflectivity;
enum   Category;

class iObject : public BoundedVolume {
  public:
	// initialization
    virtual iObject* clone() const                            = 0;
	virtual void     attach(iTexture* t, unsigned filter = 0) = 0;
	// execution
    virtual void     draw()                                   = 0;
    virtual bool     belongsTo(Category category) const       = 0;
	// termination function
	virtual void     Delete() const                           = 0;
};

iObject* CreateObject(iGraphic* v, const Reflectivity* r); 

iObject* Clone(const iObject*);

#endif
