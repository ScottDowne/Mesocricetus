#ifndef _API_LIGHT_H_
#define _API_LIGHT_H_

/* APILight Definition - Direct API Branch
 *
 * APILight.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iAPILight.h"        // for the APILight Interface
#include "DirectAPISettings.h" // for LPDIRECT3DDEVICE9

//-------------------------------- APILight -----------------------------------
//
// The APILight class implements a Light at the API level
//
class APILight : public iAPILight {

    static LPDIRECT3DDEVICE9 d3dd;        // points to the Display device
    static bool* lightIndex;              // list of available device lights
    static int   maxLights;               // maximum number of lights

	int          index;                   // identifier for this light
	bool         isSetup;                 // this api light has been setup?

	APILight();
    APILight(const APILight&);

  public:
	friend iAPILight* CreateAPILight();
    static void connectsThrough(void* d) { d3dd = (LPDIRECT3DDEVICE9)d; }
    static void alloc(int max);
    static void dealloc();
    iAPILight* clone() const { return new APILight(*this); }
    APILight& operator=(const APILight& src);
	// initialization
	bool   setup(const iLight* light);
	// execution
	void   turnOn(const iLight* light);
	void   update(const iLight* light);
	void   turnOff(const iLight* light);
    // suspension
	void   suspend(const iLight* light);
	// termination
	void   Delete() const { delete this; }
};

#endif