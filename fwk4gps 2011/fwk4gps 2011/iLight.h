#ifndef _I_LIGHT_H_
#define _I_LIGHT_H_

/* Light Interface - Model Branch
 *
 * iLight.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Frame.h" // for the Frame class definition

//-------------------------------- iLight -------------------------------------
//
// iLight is the Interface to the Light class
//
enum   LightType;
struct Vector;
struct Colour;

class iLight : public Frame {
  public:
    virtual iLight*   clone() const         = 0;
	// execution
	virtual LightType isType() const        = 0;
	virtual Colour    diffuse() const       = 0;
	virtual Colour    ambient() const       = 0;
	virtual Colour    specular() const      = 0;
	virtual float     range() const         = 0;
	virtual Vector    attenuation() const   = 0;
	virtual Vector    spot() const          = 0;
    virtual bool      isOn() const          = 0;
	virtual bool      toggle(int now)       = 0;
	virtual void      update()              = 0;
	// suspension
	virtual void      suspend()             = 0;
	virtual void      restore(int now)      = 0;
	// termination
	virtual void      Delete() const        = 0;
};

iLight* CreateDistantLight(Colour, Colour, Colour, bool = true);

iLight* CreatePointLight(Colour, Colour, Colour, float, bool, float = 1, 
 float = 0, float = 0);

iLight* CreateSpotLight(Colour, Colour, Colour, float, bool, float = 1, 
 float = 0, float = 0, float = 0, float = 0, float = 0);

iLight* Clone(const iLight*);

#endif
