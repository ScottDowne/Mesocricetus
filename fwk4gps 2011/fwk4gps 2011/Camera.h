#ifndef _CAMERA_H_
#define _CAMERA_H_

/* Camera Definition - Model Branch
 *
 * Camera.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCamera.h" // for the Camera Interface

//-------------------------------- Camera -------------------------------------
//
// The Camera class defines the structure of a viewpoint
//
class iCoordinator;

class Camera : public iCamera {

    static iCoordinator* coordinator; // points to the Coordinator Object

    iContext*            context;     // points to the Context object
	Maze* maze;
    Camera(iContext*, Maze*);
	Camera(const Camera& c);            // prevents copying
	Camera& operator=(const Camera& c); // prevents assignment
    virtual ~Camera();

  public:
	friend iCamera* CreateCamera(iContext*, Maze*);
	static void isCoordinatedBy(iCoordinator* c) { coordinator = c; }
	// execution function
    void   update(int delta);
	// termination function
	void   Delete() const { delete this; }
};

#endif
