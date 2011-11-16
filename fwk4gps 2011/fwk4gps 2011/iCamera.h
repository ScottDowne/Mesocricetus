#ifndef _I_CAMERA_H_
#define _I_CAMERA_H_

/* Camera Interface - Model Branch
 *
 * iCamera.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Frame.h" // for the BoundedVolume definition
#include "Maze.h"

//-------------------------------- iCamera ------------------------------------
//
// iCamera is the Interface to the Camera class
//
class iContext;

class iCamera : public BoundedVolume {
  public:
	// execution function
    virtual void update(int delta)     = 0;
	// termination function
	virtual void Delete() const        = 0;
};

iCamera* CreateCamera(iContext* c, Maze* m);

#endif
