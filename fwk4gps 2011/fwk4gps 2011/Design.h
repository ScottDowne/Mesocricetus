#ifndef _DESIGN_H_
#define _DESIGN_H_

/* Design Definition - Model Branch
 *
 * Design.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iDesign.h" // for the Design Interface

//-------------------------------- Design -------------------------------------
//
// The Design class defines the structure of the game design
//
class iCoordinator;
class iDisplay;
class iSoundCard;
class iObject;
class iLight;
class iSound;
class iText;

class Design : public iDesign {

	iContext*     context;     // points to the Context object
    iCoordinator* coordinator; // points to the Coordinator object
    iDisplay*     display;     // points to the Display object
    iSoundCard*   soundCard;   // points to the SoundCard object

	iSound*   background;      // points to the background sound
	iSound*   discrete;        // points to the discrete sound
	iSound*   locall;          // points to local sound on the left
	iSound*   localr;          // points to local sound on the right

    iTexture* hudTexture;      // points to the hud texture

	iLight*   pointLight;      // points to the point light
	iLight*   spotLight;       // points to the spot light
	iLight*   distantLight;    // points to the directional light
    iLight*   cloneLight;      // points to the cloned light
    iLight*   topLight;        // points to the spot on the top box

    iText*    cameraTextA;     // points to camera text part a
    iText*    cameraTextB;     // points to camera text part b
    iText*    cameraPosition;  // points to camera position text
    iText*    cameraHeading;   // points to camera heading text
    iText*    fpsText;         // points to frames per sec text

    iText*    pointLightText;    // points to point light text
    iText*    distantLightText;  // points to distant light text
    iText*    spotLightText;     // points to spot light text
    iText*    topLightText;      // points to top light text
    iText*    cloneLightText;    // points to clone light text
    iText*    pointLightState;   // points to point light status text
    iText*    distantLightState; // points to distant light status text
    iText*    spotLightState;    // points to spot light status text
    iText*    topLightState;     // points to top light status text
    iText*    cloneLightState;   // points to clone light status text

    iText*    backgroundText;  // points to background sound text
    iText*    discreteText;    // points to discrete sound text
    iText*    locallText;      // points to local sound left text
    iText*    localrText;      // points to locak sound right text
    iText*    backgroundState; // points to background sound status text
    iText*    discreteState;   // points to discrete sound status text
    iText*    locallState;     // points to local sound left status text
    iText*    localrState;     // points to locak sound right status text

    iText*    rollLeftTextX;   // points to left spinner x axis text
    iText*    rollLeftTextY;   // points to left spinner y axis text
    iText*    rollLeftTextZ;   // points to left spinner z axis text
    iText*    rollLeftDataX;   // points to left spinner x data text
    iText*    rollLeftDataY;   // points to left spinner y data text
    iText*    rollLeftDataZ;   // points to left spinner z data text

    iObject*  rollLeft;        // points to left spinner
    iObject*  rollRight;       // points to right spinner
	iObject*  spinTop;         // points to top spinner
	iSound*   objectSnd;       // points to the object sound
    iSound*   cloneSound;      // points to the cloned sound

    iCamera*  camera;          // points to the camera

    int       lastUpdate;      // time that the coordinator was last updated

    Design(iContext* c);
    Design(const Design& s);            // prevents copying
    Design& operator=(const Design& s); // prevents assignment
    virtual ~Design();

  public:
	friend iDesign* CreateDesign(iContext* c);
	// initialization
    void   interrogate(void*);
    bool   setup(void*);
    void   initialize(int now);
	void   reset(int now);
    void   configure();
	// execution function
    void   update(int now);
    void   resize();
    void   render();
    // suspension
    void   suspend();
    void   restore(int now);
	// termination
    void   release();
	void   Delete() const { delete this; }
};

#endif
