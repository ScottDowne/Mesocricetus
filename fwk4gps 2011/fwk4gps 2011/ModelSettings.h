#ifndef _MODEL_SETTINGS_H_
#define _MODEL_SETTINGS_H_

/* Header for Configuring the Model Branch
 *
 * ModelSettings.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Configuration.h" // framework-wide configuration settings

// camera motion
//
// camera speed control factors
#define CAM_SPEED       0.02f
#define TURNING_RADIUS  120.00f
#define ANG_CAM_SPEED   ( CAM_SPEED / TURNING_RADIUS )

// camera parameter
//
// lowest possible y value in world space
#define FLOOR  -50

// motion parameters
//
// factors applied to the time interval
#define FORWARD_SPEED 10.0f / UNITS_PER_SEC
#define ROT_SPEED     0.03f * FORWARD_SPEED
#define CONSTANT_ROLL 10.0f * ROT_SPEED

// input device motion conversion factors - 
//
// mouse motion conversion factors
#define MOUSE_SPEED             10
#define MOUSE_BUTTON_SCALE      10
// controller conversion factors
#define CTR_SPEED               0.005f
#define CTR_DISPLACEMENT_FACTOR 0.015f 
#define CTR_ROTATION_FACTOR     0.015f 

// audio controls
//
// initial sound settings
#define DELTA_VOLUME     200  // increment within [10000,10000]
#define FREQ_RANGE      5000  // in Hertz (cycles per sec)
#define FREQ_DROP_VELOCITY 8  // rate of fall to idle frequency
#define FREQ_RISE_VELOCITY 4  // rate of rise to maximum frequency

// audio controls
//
// initial volume settings
#define MIN_VOLUME		   0
#define DEFAULT_VOLUME	  50
#define MAX_VOLUME		 100
#define STEP_VOLUME        5
// initial frequency settings
#define MIN_FREQUENCY	    0
#define DEFAULT_FREQUENCY  50
#define MAX_FREQUENCY     100
#define STEP_FREQUENCY		5

// hud and text parameters
//
// hud location within window [0,1]
#define HUD_X     0.01f
#define HUD_Y     0.01f
#define HUD_W     0.43f
#define HUD_H     0.43f
// hud background degree of transparency [\x00,\xff]
#define HUD_TEX_ALPHA '\x20'
// hud background image
#define HUD_IMAGE L"hudBackground.bmp"
#define HUD_SPEED (0.5f /* screen units per sec */ / UNITS_PER_SEC) 
// text limits within the hud [0,1]
#define TL_MAX    0.99f
#define TL_MIN    0.01f
#define R_MIN     0.20f

#endif
