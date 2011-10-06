/* Object Implementation - Model Branch
 *
 * Object.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCoordinator.h"  // for the Coordinator Interface
#include "iDisplay.h"      // for the Display Interface
#include "iGraphic.h"      // for the Graphic Interface
#include "iTexture.h"      // for the Texture Interface
#include "iUtilities.h"    // for error()

#include "Object.h"        // for the Object class definition
#include "ModelSettings.h" // for coordinator macros

//-------------------------------- Object -------------------------------------
//
// The Object class defines the structure of a single object in the model
//
// The Coordinator manages the Object Instances
//
iCoordinator* Object::coordinator = NULL;
iDisplay*     Object::display     = NULL;

iObject* CreateObject(iGraphic* v, const Reflectivity* r) {

    return new Object(TEST_COLOUR, v, r);
}

// Clone create a clone of Object* src and returns its address
//
iObject* Clone(const iObject* src) {

    return src->clone();
}

// constructor initializes an object with material reflectivity *r
//
Object::Object(Category d, iGraphic* v, const Reflectivity* r) : category(d),
 graphic(v), flags(TEX_DEFAULT) {
    
    if (!coordinator)
        error(L"Object::00 Couldn\'t access the Coordinator");
    else if(!coordinator->add(this))
        error(L"Object::01 Couldn\'t add object to the Coordinator");

    // store reflectivity and texture pointer
    reflectivity = *r;
    texture = NULL;

    // specialize category for translucency only if TEST_COLOUR
    if (category == TEST_COLOUR) {
        category = OPAQUE_OBJECT;
        if (reflectivity.translucent())
            category = TRANSLUCENT_OBJECT;
    }
}

// copy constructor initializes the instance pointer and calls the assignment
// operator
//
Object::Object(const Object& src) {

    if (!coordinator)
        error(L"Object::00 Couldn\'t access the Coordinator");
    else if(!coordinator->add(this))
        error(L"Object::01 Couldn\'t add the object to the Coordinator");

    *this        = src;
}
            
// assignment operator discards old data and creates a copy from
// src - does not copy the text items attached to src - instead 
// initializes them to NULL
//
Object& Object::operator=(const Object& src) {

    if (this != &src) {
        *((BoundedVolume*)this) = src;
        category     = src.category;
        graphic = src.graphic; 
        flags        = src.flags;
        reflectivity = src.reflectivity;
        texture      = src.texture;
    }

    return *this;
}

// attach attaches the texture pointed to by t to the object and sets 
// the filtering flags
//
void Object::attach(iTexture* t, unsigned filter) {

    texture = t;
	if (filter) flags = filter;
}

// draw draws the object using its own technique
//
void Object::draw() { 
	
    display->beginDrawObject(world());
    if (texture) texture->attach(flags);
    display->set(reflectivity);
    graphic->draw();
    if (texture) texture->detach();
}

// destructor deletes the array of texture pointers and material reflectivities
// and removes the object from the Coordinator coordinator
//
Object::~Object() {

    if (coordinator)
        coordinator->remove(this);
    else
        error(L"Object::90 Couldn\'t access the Coordinator");
}

