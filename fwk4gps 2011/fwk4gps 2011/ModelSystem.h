#ifndef _MODEL_SYSTEM_H_
#define _MODEL_SYSTEM_H_

/* Header for Defining the Coordinate System used in the Model Branch
 *
 * ModelSystem.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

// Table of Available Z Directions
#define NEAR_TO_FAR ( 1)
#define FAR_TO_NEAR (-1)

// make your selections below 
//

// the direction of the z-axis used in the coordinator 
//
#define MODEL_Z_AXIS NEAR_TO_FAR
//#define MODEL_Z_AXIS FAR_TO_NEAR

// the direction of the z-axis with respect to the camera heading
//
#define Z_AXIS NEAR_TO_FAR
//#define Z_AXIS FAR_TO_NEAR

#endif
