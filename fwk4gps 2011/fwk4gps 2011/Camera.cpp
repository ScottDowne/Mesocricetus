/* Camera Implementation - Model Branch
 *
 * Camera.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iContext.h"        // for the Context Interface
#include "iCoordinator.h"    // for the Coordinator Interface
#include "iUtilities.h"      // for error(), sprintf()

#include "Maze.h"
#include "Camera.h"          // for Camera class definition
#include "MathDefinitions.h" // for math functions in coordinator coordinates
#include "ModelSettings.h"   // for FLOOR, FORWARD_SPEED, ROT_SPEED, MAX_DESC

//-------------------------------- Camera -------------------------------------
//
// The Camera class defines the structure of one viewpoint on the scene
//
// The Coordinator manages the Camera Instances
//
iCoordinator* Camera::coordinator = NULL;

// CreateCamera creates a Camera object
//
iCamera* CreateCamera(iContext* c, Maze* m) {

	return new Camera(c, m);
}

// constructor adds the Camera Instance to the coordinator
//
Camera::Camera(iContext* c, Maze* m) : context(c) {

	maze = m;
    if (!coordinator)
        error(L"Camera::00 Couldn\'t access the Coordinator");
    else if(!coordinator->add(this))
        error(L"Camera::01 Couldn\'t add the camera to the Coordinator");

    #if MODEL_Z_AXIS == FAR_TO_NEAR
    rotatey(3.14159f);
    #endif
}

// update adjusts the camera's viewpoint and heading according
// to the user's interventions
//
void Camera::update(int delta) {

    int dx = 0, // pitch up/down
        dy = 0, // yaw left/right
        dz = 0; // advance/retreat
    int rx = 0,
        ry = 0,
        rz = 0;
	float bounds = 0.2,
		  boundingX = 0,
		  boundingY = 0;
    bool adjustCamera = !context->pressed(ADJUST_PROJECTOR);

    // controller input
    int jx = context->get(GF_CT_POSX);
    int jy = context->get(GF_CT_POSY);
    int jr = context->get(GF_CT_ROTZ);
    if (jy)
        rx = -(int)(jy * CTR_SPEED);
    if (jr)
        ry = (int)(jr * CTR_SPEED);
    if (jx)
        rz = (int)(jx * CTR_SPEED);

	// mouse input
    int mx = context->get(GF_MS_DSPX);
    int my = context->get(GF_MS_DSPY);
    int mz = context->get(GF_MS_ROTZ);
	if (mx)
		ry += mx * MOUSE_SPEED;
	if (my)
		rx += my * MOUSE_SPEED;
	if (mz)
		dy += mz * MOUSE_SPEED;
		
	// keyboard input
    if (context->pressed(CAM_STRAFE_LEFT))
        dx -= delta;
    if (context->pressed(CAM_STRAFE_RIGHT))
        dx += delta;
    if (context->pressed(CAM_STRAFE_LEFT_ALT))
        dx -= delta;
    if (context->pressed(CAM_STRAFE_RIGHT_ALT))
        dx += delta;
    if (context->pressed(CAM_FLY_DOWN))
        dy -= delta;
    if (context->pressed(CAM_FLY_UP))
        dy += delta;
    if (context->pressed(CAM_ADVANCE))
        dz += delta;
    if (context->pressed(CAM_RETREAT))
        dz -= delta;
    if (context->pressed(CAM_PITCH_UP))
        rx -= delta;
    if (context->pressed(CAM_PITCH_DOWN))
        rx += delta;
    if (context->pressed(CAM_YAW_LEFT))
        ry -= delta;
    if (context->pressed(CAM_YAW_RIGHT))
        ry += delta;
    //if (context->pressed(CAM_ROLL_LEFT))
    //    rz -= delta;
    //if (context->pressed(CAM_ROLL_RIGHT))
    //    rz += delta;

    // adjust camera orientation
	if (adjustCamera && (rx || ry || rz)) {
        // yaw left/right
		if (ry)
            rotatey(-ry * ANG_CAM_SPEED);
		// pitch up/down
        if (rx) 
            rotate(orientation('x'), rx * ANG_CAM_SPEED);
		// roll left/right
        if (rz) 
            rotate(orientation('z'), rz * ANG_CAM_SPEED);
    }
	// adjust camera position
    if (adjustCamera && (dx || dy || dz)) {

        Vector displacement = 
         (float) dx * CAM_SPEED * orientation('x') +
         Vector(0,0,0) + 
         (float) dz * CAM_SPEED * orientation('z');

		Vector pos = position();

		if (displacement.z > 0) {
		
			boundingY = bounds;
		} else {
		
		    boundingY = -bounds;
		}
		if (displacement.x > 0) {
		
			boundingX = bounds;
		} else {
		
		    boundingX = -bounds;
		}

		if (maze->checkCollision((pos.x + displacement.x) / SCALE + boundingX, pos.z / SCALE)) translate(displacement.x, 0, 0);
        if (maze->checkCollision(pos.x / SCALE, (pos.z + displacement.z) / SCALE + boundingY)) translate(0, 0, displacement.z);
    }

    // store the current viewpoint, heading and up direction
    context->set(GF_CA_POSN, position());
    context->set(GF_CA_HEAD, ::normal(orientation('z')));
    context->set(GF_CA_UP,   ::normal(orientation('y')));
}

// destructor removes the Camera Instance from the coordinator
//
Camera::~Camera() {

    if (coordinator)
        coordinator->remove(this);
    else
        error(L"Camera::90 Couldn\'t access the Coordinator");
}
