#ifndef _OBJECT_H_
#define _OBJECT_H_

/* Object Definition - Model Branch
 *
 * Object.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iObject.h" // for the Object Interface

//-------------------------------- Object -------------------------------------
//
// The Object class defines an independent object in the Model Branch
//
class iCoordinator;
class iDisplay;
class iGraphic;

class Object : public iObject {

	static iCoordinator* coordinator; // points to the coordinator object
    static iDisplay*     display;     // points to the display object

    Category       category;       // drawing category
    iGraphic*      graphic;        // points to the object's vertex list
    Reflectivity   reflectivity;   // material reflectivity
	iTexture*      texture;        // points to attached texture
	unsigned       flags;          // texture sampling flags

  protected:
    Object(Category category, iGraphic* v, const Reflectivity* r);
    iTexture* getTexture(int i, int j) const { return texture; }
    virtual   ~Object();

  public:
	friend iObject* CreateObject(iGraphic* v, const Reflectivity* r);
	static void isCoordinatedBy(iCoordinator* s) { coordinator = s; }
	static void isConnectedTo(iDisplay* d)       { display     = d; }
    Object(const Object& o);            
    Object& operator=(const Object& o); 
    iObject* clone() const                       { return new Object(*this); }
	// initialization
	void   attach(iTexture* t, unsigned filter = 0);
	// execution
    bool   belongsTo(Category c) const           { return c == category; }
    void   draw();
	// termination function 
	void   Delete() const                        { delete this; }
};

#endif
