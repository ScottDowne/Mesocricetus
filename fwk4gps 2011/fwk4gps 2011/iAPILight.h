#ifndef _I_API_LIGHT_H_
#define _I_API_LIGHT_H_

/* API Light Interface - Direct API Branch
 *
 * iAPILight.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPILight ----------------------------------
//
// iAPILight is the Interface to the APILight class 
//
class iLight;

class iAPILight {
  public:
    virtual iAPILight* clone() const      = 0;
	// execution
	virtual void turnOn(const iLight* l)  = 0;
	virtual void update(const iLight* l)  = 0;
	virtual void turnOff(const iLight* l) = 0;
	// termination
	virtual void suspend(const iLight* l) = 0;
	virtual void Delete() const           = 0;
};

iAPILight* CreateAPILight();

#endif