/* Design Implementation - Model Branch
 *
 * Design.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */
#include <list>
#include "iContext.h"        // for the Context Interface
#include "iText.h"           // for the Text Interface
#include "iSound.h"          // for the Sound Interface
#include "iLight.h"          // for the Light Interface
#include "iObject.h"         // for the Object Interface
#include "iTexture.h"        // for the Texture Interface
#include "iCamera.h"         // for the Camera Interface
#include "iGraphic.h"        // for the Graphic Interface
#include "iCoordinator.h"    // for the Coordinator Interface
#include "iDisplay.h"        // for the Display Interface
#include "iSoundCard.h"      // for the SoundCard Interface
#include "iUtilities.h"      // for strcpy()

#include "Design.h"          // for the Design class definition
#include "MathDefinitions.h" // for MODEL_Z_AXIS
#include "ModelSettings.h"   // for FLOOR, MOUSE_BUTTON_SCALE, ROLL_SPEED

const wchar_t* orient(wchar_t* str, const iFrame* frame, char c, int f = 1);
const wchar_t* position(wchar_t* str, const iFrame* frame);

//-------------------------------- Design -------------------------------------
//
// The Design class implements the game design proper within the model branch
//
// CreateDesign creates the Design Object
//
iDesign* CreateDesign(iContext* c) {

	return new Design(c);
}

// constructor creates the coordinator object, the display object, and
// the sound card object and initializes the instance pointers and the
// reference time
//
Design::Design(iContext* c) : context(c) {

    coordinator = CreateCoordinator(context);
    display     = CreateDisplay(context);
    soundCard   = CreateSoundCard(context);

	// pointers to the objects
    rollLeft     = NULL;
    rollRight    = NULL;
	spinTop      = NULL;

	// pointers to the lights
    pointLight   = NULL;
	spotLight    = NULL;
	distantLight = NULL;
    topLight     = NULL;
    cloneLight   = NULL;

    // pointers to the sounds
	background   = NULL;
	discrete     = NULL;
	locall       = NULL;
	localr       = NULL;
	objectSnd    = NULL;
    cloneSound   = NULL;

    // pointers to the text items
    hudTexture        = NULL;
    cameraTextA       = NULL;
    cameraTextB       = NULL;
    cameraPosition    = NULL;
    cameraHeading     = NULL;
    fpsText           = NULL;
    rollLeftTextX     = NULL;
    rollLeftTextY     = NULL;
    rollLeftTextZ     = NULL;
    rollLeftDataX     = NULL;
    rollLeftDataY     = NULL;
    rollLeftDataZ     = NULL;
    pointLightText    = NULL;
    distantLightText  = NULL;
    spotLightText     = NULL;
    topLightText      = NULL;
    cloneLightText    = NULL;
    pointLightState   = NULL;
    distantLightState = NULL;
    spotLightState    = NULL;
    topLightState     = NULL;
    cloneLightState   = NULL;
    backgroundText    = NULL;
    discreteText      = NULL;
    locallText        = NULL;
    localrText        = NULL;
    backgroundState   = NULL;
    discreteState     = NULL;
    locallState       = NULL;
    localrState       = NULL;

    //maze
    maze = NULL;

    // reference time
	lastUpdate = 0;
}

// interrogate interrogates the host system for the available configurations
//
void Design::interrogate(void* hwnd) {

    display->interrogate(hwnd);
}

// configure sets the configuration for the display and the sound
// card
//
void Design::configure() {

    display->configure();
}

// setup sets up the host system for the selection configuration
//
bool Design::setup(void* hwnd) {

    bool rc = false;

	// setup the graphics card
	if (!display->setup(hwnd))
		error(L"Design::10 Failed to set up the Display object");

    else
        rc = true;

    return rc;
}

// initialize initializes the coordinator, creates the primitive sets, textures,
// objects, lights, sounds, cameras, and text items for the initial coordinator,
// and initializes the reference time
//
void Design::initialize(int now) {

    coordinator->reset(now);

    // projection parameters
    //
    context->set(GF_FR_NEAR, NEAR_CLIPPING);
    context->set(GF_FR_FAR, FAR_CLIPPING);
    context->set(GF_FR_FOV, FIELD_OF_VIEW);

    // cameras ----------------------------------------------------------------

    // camera at a distance - in lhs coordinates
    (camera = CreateCamera(context))->translate(-20, 0, -80);
    camera->setRadius(17.8f);

    // camera attached to the left box object
    iCamera* objectCamera = CreateCamera(context);

    // coordinator ------------------------------------------------------------------

    Colour white(1, 1, 1);
    Colour yellow(0.7f, 0.7f, 0.0f);
    Colour grey(0.7f, 0.7f, 0.7f);
    Colour turquoise(0, 0.8f, 0.6f);
    Colour black(0, 0, 0);

    Reflectivity greyish = Reflectivity(grey);

    // maze ---------------------------------------------------------

   int left, right, back, front;

   for (int y = 0; y < MAZE_ARRAY_COL; y++) {

      for (int x = 0; x < MAZE_ARRAY_ROW; x++) {
      
         if ( MAZE_ARRAY[y][x] == 0) {
         
            left = right = back = front = 0;

            if (x == 0 || MAZE_ARRAY[y][x-1] > 0) {
            
               left = 1;
            }

            if (x == MAZE_ARRAY_COL-1 || MAZE_ARRAY[y][x+1] > 0) {
            
               right = 1;
            }

            if (y == 0 || MAZE_ARRAY[y+1][x] > 0) {
            
               back = 1;
            }

            if (y == MAZE_ARRAY_ROW-1 || MAZE_ARRAY[y+1][x] > 0) {
            
               front = 1;
            }

            //primitiveSets.push_back(CreateBox(x, colCount - y, 0, x+1, (colCount - y) + 1, 1,
            //                                  front, right, back, left, 0, 1));
			CreateObject(CreateBox(x, 0, MAZE_ARRAY_COL - y, x+1, 1, (MAZE_ARRAY_COL - y) + 1, front, right, back, left, 0, 1), &greyish);
         }
         else
         {
            //primitiveSets.push_back(CreateBox(x, colCount - y - 1, 0, x+1, (colCount - y) + 1 - 1, 1,
            //                                  0, 0, 0, 0, 0, 1));
			 CreateObject(CreateBox(x, -1, MAZE_ARRAY_COL - y, x+1, 0, (MAZE_ARRAY_COL - y) + 1, 0, 0, 0, 0, 0, 1), &greyish);
         }
 
      }
   }

    /*std::list<iGraphic *> mazeGraphics = CreateMaze(MAZE_ARRAY, MAZE_ARRAY_COL, MAZE_ARRAY_ROW);

    maze = new std::list<iObject *>();
    std::list<iGraphic *>::iterator mazeGraphicsItr = mazeGraphics.begin();

    for (std::list<iGraphic *>::iterator itr = mazeGraphics.begin(); itr != mazeGraphics.end(); itr++)
    {
       maze->push_back(CreateObject(*itr, &greyish));
    }*/

	// create textures
	iTexture* checkbmp = CreateTexture(L"check.bmp");
	iTexture* checktga = CreateTexture(L"check.tga");
	iTexture* checkdsy = CreateTexture(L"daisy.bmp");
    iTexture* clouds   = CreateTexture(L"clouds.png");

    // create vertex lists
    iGraphic* box   = CreateBox(-10, -10, -10 * MODEL_Z_AXIS, 
     10, 10, 10 * MODEL_Z_AXIS);
    iGraphic* plate = CreateBox(-50, -10, 0 * MODEL_Z_AXIS, 
     50, 10, 100 * MODEL_Z_AXIS);
    iGraphic* grid  = CreateGrid(-25, 0, 25, 10);

    
    rollRight = CreateObject(box, &greyish);
	rollRight->attach(checkdsy);
    rollRight->translate(20, -20, 40 * MODEL_Z_AXIS);
    rollRight->setPlane(Vector(-1,0,0), 10);

    Reflectivity greenish = Reflectivity(Colour(0.1f, 0.8f, 0.1f, 0.5f));
    rollLeft = CreateObject(box, &greenish);
	rollLeft->attach(checktga);
    rollLeft->translate(-23, 13, 30 * MODEL_Z_AXIS);
    rollLeft->setRadius(17.8f);
    objectCamera->attachTo(rollLeft);

    Reflectivity bluish = Reflectivity(Colour(0.0f, 0.1f, 0.9f));
    iObject* floor = CreateObject(plate, &bluish);
	floor->attach(checkdsy, TEX_MIN_ANISOTROPIC | TEX_MAG_ANISOTROPIC);
	floor->translate(-10, FLOOR - 13, 180 * MODEL_Z_AXIS);

    Reflectivity redisher = Reflectivity(Colour(0.9f, 0.1f, 0.1f));
    spinTop = CreateObject(box, &redisher);
	spinTop->attach(checkbmp, TEX_MIN_ANISOTROPIC | TEX_MAG_ANISOTROPIC);
    spinTop->translate(-10, 30, 50 * MODEL_Z_AXIS);
	spinTop->rotatex(1.57f * MODEL_Z_AXIS);

    Reflectivity whiteish = Reflectivity(Colour(0.9f, 0.9f, 0.9f));
    //iObject* xz = CreateObject(grid, &whiteish);
    //xz->setRadius(71);
    //iObject* xy = Clone(xz);
    //iObject* yz = Clone(xz);
    //xz->translate(25, 0, 25 * Z_AXIS);
    //xy->rotatex(1.5708f);
    //xy->translate(25, 25, 0);
    //yz->rotatez(1.5708f);
    //yz->translate(0, 25, 25 * Z_AXIS);

    // lighting ---------------------------------------------------------------

    // global ambient light: a little bit of light on everything, for when all 
	// "lights are out"
    context->set(GF_LT_BKGD, Vector(.2f, .2f, .2f));

    // Create far away point-source light
	pointLight = CreatePointLight(grey, grey, white, 20000.0f, true);
    pointLight->translate(500.f, 1000.f, 100.f * MODEL_Z_AXIS);

    // create overhead spotlight
	spotLight = CreateSpotLight(white, white, white, 300.0f, true, 1, 0.00005f, 
	 0.00001f, .60f, .25f, 0.9f);
    spotLight->translate(-20.f, 10.f, -50.f * MODEL_Z_AXIS);
    spotLight->rotatex(.7f);

    // create directional light
 	distantLight = CreateDistantLight(turquoise, turquoise, black, false);
    distantLight->translate(10, 1000, 0);
    distantLight->rotatex(1.57f);
    distantLight->rotatez(0.20f);

     // create a spotlight attached to the top object
    topLight = CreateSpotLight(white, white, white, 200.0f, true, 1, 0.0005f, 
     0, .30f, .25f, 0.1f);
    topLight->rotatex(.7f);
    topLight->attachTo(spinTop);

	// audio ------------------------------------------------------------------
    
    // create background sound
	if (context->get(GF_SNDFILE, SND_BKGRD))
		background = CreateSound(context->get(GF_SNDFILE, SND_BKGRD), 
         GLOBAL_SOUND, true, false);

	// create discrete sound
	if (context->get(GF_SNDFILE, SND_DISCR))
		discrete = CreateSound(context->get(GF_SNDFILE, SND_DISCR), 
         GLOBAL_SOUND, false, false);

	// create local sound on the left
	if (context->get(GF_SNDFILE, SND_LOCAL_L)) {
		locall = CreateSound(context->get(GF_SNDFILE, SND_LOCAL_L), 
         LOCAL_SOUND, true, true, 90);
		locall->translate(-30, 5, 40 * MODEL_Z_AXIS);
		locall->rotatey(3.14f);
	}

	// create local sound on the right
	if (context->get(GF_SNDFILE, SND_LOCAL_R)) {
		localr = CreateSound(context->get(GF_SNDFILE, SND_LOCAL_R), 
         LOCAL_SOUND, true, true, 90);
		localr->translate(30, 5, 40 * MODEL_Z_AXIS);
		localr->rotatey(3.14f);
	}

	// create a local sound attached to right object
	if (context->get(GF_SNDFILE, SND_OBJECT)) {
		objectSnd = CreateSound(context->get(GF_SNDFILE, SND_OBJECT), 
         MOBILE_SOUND, true, true, 90);
        objectSnd->attachTo(rollRight);
    }

    // Heads Up Display -------------------------------------------------------

	// HUD texture
    hudTexture = CreateTexture(HUD_IMAGE);

    // timer data
	fpsText    = CreateText(RelRect(0, 0, 0.9f, 0.1f), L"", TEXT_RIGHT);

    // camera data
	cameraTextA    = CreateText(RelRect(0.0f, 0.1f, 0.25f, 0.2f), 
     L" Camera: at ", TEXT_CENTER);
    cameraTextB    = CreateText(RelRect(0.50f, 0.1f, 0.65f, 0.2f), 
     L" heading ", TEXT_CENTER);
	cameraPosition = CreateText(RelRect(0.25f, 0.1f, 0.45f, 0.2f));
	cameraHeading  = CreateText(RelRect(0.68f, 0.1f, 0.90f, 0.2f));

    // object data
    if (rollLeft) {
        rollLeftTextX = CreateText(RelRect(0, 0.3f, 0.45f, 0.4f), 
         L" Spinner x orientation ");
        rollLeftDataX = CreateText(RelRect(0.45f, 0.3f, 0.7f, 0.4f));
        rollLeftTextY = CreateText(RelRect(0, 0.4f, 0.45f, 0.5f), 
         L" Spinner y orientation ");
        rollLeftDataY = CreateText(RelRect(0.45f, 0.4f, 0.7f, 0.5f));
        rollLeftTextZ = CreateText(RelRect(0, 0.5f, 0.45f, 0.6f), 
         L" Spinner z orientation ");
        rollLeftDataZ = CreateText(RelRect(0.45f, 0.5f, 0.7f, 0.6f));
    }

    // lighting data
    if (pointLight) {
	    pointLightText  = CreateText(RelRect(0.06f, 0.6f, 0.5f, 0.67f), 
         L"point light");
        pointLightState = CreateText(RelRect(0.02f, 0.6f, 0.06f, 0.67f));
    }
    if (spotLight) {
	    spotLightText  = CreateText(RelRect(0.06f, 0.67f, 0.5f, 0.74f), 
         L"spot light");
        spotLightState = CreateText(RelRect(0.02f, 0.67f, 0.06f, 0.74f));
    }
    if (distantLight) {
 	    distantLightText  = CreateText(RelRect(0.06f, 0.74f, 0.5f, 0.81f), 
         L"directional light");
        distantLightState = CreateText(RelRect(0.02f, 0.74f, 0.06f, 0.81f));
    }
    if (topLight) {
	    topLightText  = CreateText(RelRect(0.06f, 0.81f, 0.5f, 0.88f), 
         L"top light");
        topLightState = CreateText(RelRect(0.02f, 0.81f, 0.06f, 0.88f));
    }

    // sound data
    if (background) {
		backgroundText  = CreateText(RelRect(0.56f, 0.6f, 1, 0.69f), 
         L"background");
        backgroundState = CreateText(RelRect(0.52f, 0.6f, 0.56f, 0.69f));
    }
    if (discrete) {
		discreteText  = CreateText(RelRect(0.56f, 0.7f, 1, 0.79f), 
         L"discrete");
        discreteState = CreateText(RelRect(0.52f, 0.7f, 0.56f, 0.79f));
    }
    if (locall) {
		locallText  = CreateText(RelRect(0.56f, 0.8f, 1, 0.89f), 
         L"local left");
        locallState = CreateText(RelRect(0.52f, 0.8f, 0.56f, 0.89f));
    }
    if (localr) {
		localrText  = CreateText(RelRect(0.56f, 0.9f, 1, 0.99f), 
         L"local left");
        localrState = CreateText(RelRect(0.52f, 0.9f, 0.56f, 0.99f));
    }

	// reference time
    lastUpdate = now;
}

// reset changes the context if any change has been requested and
// reinitializes the reference time
//
void Design::reset(int now) {

    coordinator->reset(now);

	// audio ------------------------------------------------------------------
    
	// change background sound
	if (background && context->get(GF_SNDFILE, SND_BKGRD) && 
	 strcmp(context->get(GF_SNDFILE, SND_BKGRD), background->relFileName()))
		background->change(context->get(GF_SNDFILE, SND_BKGRD));
    else if (!background && context->get(GF_SNDFILE, SND_BKGRD))
		background = CreateSound(context->get(GF_SNDFILE, SND_BKGRD), 
         GLOBAL_SOUND, true, false);

	// change discrete sound
	if (discrete && context->get(GF_SNDFILE, SND_DISCR) && 
	 strcmp(context->get(GF_SNDFILE, SND_DISCR), discrete->relFileName()))
		discrete->change(context->get(GF_SNDFILE, SND_DISCR));
    else if (!discrete && context->get(GF_SNDFILE, SND_DISCR))
		discrete = CreateSound(context->get(GF_SNDFILE, SND_DISCR), 
         GLOBAL_SOUND, false, false);

	// change local sound on the left
	if (locall && context->get(GF_SNDFILE, SND_LOCAL_L) && discrete && 
	 strcmp(context->get(GF_SNDFILE, SND_LOCAL_L), discrete->relFileName()))
		locall->change(context->get(GF_SNDFILE, SND_LOCAL_L));
	else if (!locall && context->get(GF_SNDFILE, SND_LOCAL_L)) 
		locall = CreateSound(context->get(GF_SNDFILE, SND_LOCAL_L), 
         LOCAL_SOUND, true, true, 90);

	// change local sound on the right
	if (localr && context->get(GF_SNDFILE, SND_LOCAL_R) && discrete && 
	 strcmp(context->get(GF_SNDFILE, SND_LOCAL_R), discrete->relFileName()))
		localr->change(context->get(GF_SNDFILE, SND_LOCAL_R));
	else if (!localr && context->get(GF_SNDFILE, SND_LOCAL_R))
		localr = CreateSound(context->get(GF_SNDFILE, SND_LOCAL_R), 
         LOCAL_SOUND, true, true, 90);

	// change background sound
	if (objectSnd && context->get(GF_SNDFILE, SND_OBJECT) &&
	 strcmp(context->get(GF_SNDFILE, SND_OBJECT), objectSnd->relFileName()))
		objectSnd->change(context->get(GF_SNDFILE, SND_OBJECT));
	else if (!objectSnd && context->get(GF_SNDFILE, SND_OBJECT)) 
		objectSnd = CreateSound(context->get(GF_SNDFILE, SND_OBJECT), 
         MOBILE_SOUND, true, true, 90);

	// reset reference time ---------------------------------------------------
    //
    lastUpdate = now;
}

// update updates the position and orientation of each object for time "now"
// according to the keys pressed
//
void Design::update(int now) {

    coordinator->update(now);

    int delta = now - lastUpdate;
    int dr = 0;  // roll the right box around its x axis
    int ds = 0;  // spin the right box around its y axis
    int dt = 0;  // roll the top   box around its z axis
    int dw = 0;  // roll the spot light around the x axis

	// audio ------------------------------------------------------------------
    
    if (context->pressed(AUD_BKGRD))
        background->toggle(now);
    if (context->pressed(AUD_IMPLS) || context->pressed())
        discrete->toggle(now);

    // lighting ---------------------------------------------------------------

    if (context->pressed(LIGHT_POINT) && pointLight)
		pointLight->toggle(now);
	if (context->pressed(LIGHT_SPOT) && spotLight)
		spotLight->toggle(now);
	if (context->pressed(LIGHT_DISTANT) && distantLight)
		distantLight->toggle(now);
	if (context->pressed(LIGHT_CLONE) && cloneLight)
		cloneLight->toggle(now);

    // coordinator ------------------------------------------------------------------

    // add changes introduced by mouse input
    dr = context->pressed(LEFT_BUTTON)  * delta * MOUSE_BUTTON_SCALE;
    ds = context->pressed(RIGHT_BUTTON) * delta * MOUSE_BUTTON_SCALE;

    // add changes introduced by keyboard input
    if (context->pressed(MDL_ROLL_BOXES))
        dr += delta;
    if (context->pressed(BUTTON_4))
        dt += delta;
    if (context->pressed(MDL_SPIN_LEFT))
        ds += delta;
    if (context->pressed(MDL_SPIN_TOP))
        dt += delta;
    if (context->pressed(MDL_SPIN_SPOT_P))
        dw += delta;
    if (context->pressed(MDL_SPIN_SPOT_N))
        dw -= delta;

	// adjust the boxes' positions and orientations for user input
    if (rollRight) 
	    rollRight->rotatex(dr * ROT_SPEED + CONSTANT_ROLL);
    if (rollLeft) 
	    rollLeft->rotatex(dr * ROT_SPEED + CONSTANT_ROLL);
    if (dt && spinTop) 
		spinTop->rotatez(dt * ROT_SPEED * MODEL_Z_AXIS);
    if (ds && rollLeft)
        rollLeft->rotatey(ds * ROT_SPEED);
    if (dw && spotLight)
        spotLight->rotatex(dw * ROT_SPEED);
    
    // clone object, sound, light - restrict to one clone each 
    static bool co = true;
    static bool cs = true;
    static bool cl = true;
    if (context->pressed(MDL_NEW_OBJECT) && co) {
        co = false;
        Clone(rollRight)->translate(-40, -20, 30 * MODEL_Z_AXIS);
    }
    if (context->pressed(MDL_NEW_LIGHT) && cl) {
        cl = false;
        (cloneLight = Clone(topLight))->attachTo(rollLeft);
     	cloneLightText  = CreateText(RelRect(0.06f, 0.88f, 0.5f, 0.95f), 
         L"cloned light");
        cloneLightState = CreateText(RelRect(0.02f, 0.88f, 0.06f, 0.95f));
    }
    if (context->pressed(MDL_NEW_SOUND) && cs) {
        cs = false;
        (cloneSound = Clone(objectSnd))->translate(-60, 0, 0);
    }

    // respond to collision with left object
    if (collision(camera, rollLeft) && spotLight)
        spotLight->toggle(now);

    // respond to collision with right object
    if (collision(camera, rollRight) && distantLight)
        distantLight->toggle(now);

    // reference time
    lastUpdate = now;
}

// resize resizes the display
//
void Design::resize() {

	display->resize();
}

// render renders the coordinator by category
//
void Design::render() {

	// update the listener
    soundCard->update();

    // draw the scene and optionally the HUD
    display->beginDrawFrame();

    coordinator->render(OPAQUE_OBJECT);
    display->set(ALPHA_BLEND, true);
    coordinator->render(TRANSLUCENT_OBJECT);
    display->set(ALPHA_BLEND, false);
    coordinator->render(SOUND);
    if (coordinator->hudIsOn()) {
        display->beginDraw(HUD_ALPHA);
		// draw the HUD background
		if (hudTexture) {
            float x, y;
            coordinator->hudPos(x, y);
			hudTexture->draw(RelRect(x, y, x + HUD_W, y + HUD_H), 
             HUD_TEX_ALPHA);
        }
        wchar_t str[MAX_DESC + 1];
        if (fpsText) {
            sprintf(str, coordinator->frameRate(), L" fps");
	        fpsText->set(str);
            fpsText->draw();
        }
        const iCamera* camera = coordinator->currentCamera();
        if (camera) {
            cameraTextA->draw();
            cameraPosition->set(position(str, camera)); 
            cameraPosition->draw();
            cameraTextB->draw();
            cameraHeading->set(orient(str, camera, 'z', 100)); 
            cameraHeading->draw();
        }
        if (rollLeft) {
            rollLeftTextX->draw();
            rollLeftDataX->set(orient(str, rollLeft, 'x', 100));
            rollLeftDataX->draw();
            rollLeftTextY->draw();
            rollLeftDataY->set(orient(str, rollLeft, 'y', 100));
            rollLeftDataY->draw();
            rollLeftTextZ->draw();
            rollLeftDataZ->set(orient(str, rollLeft, 'z', 100));
            rollLeftDataZ->draw();
        }
        if (pointLight) {
            pointLightText->draw();
            pointLightState->set(pointLight->isOn() ? TEXT_ON : TEXT_OFF);
            pointLightState->draw();
        }
        if (distantLight) {
            distantLightText->draw();
            distantLightState->set(distantLight->isOn() ? TEXT_ON : TEXT_OFF);
            distantLightState->draw();
        }
        if (spotLight) {
            spotLightText->draw();
            spotLightState->set(spotLight->isOn() ? TEXT_ON : TEXT_OFF);
            spotLightState->draw();
        }
        if (topLight) {
            topLightText->draw();
            topLightState->set(topLight->isOn() ? TEXT_ON : TEXT_OFF);
            topLightState->draw();
        }
        if (cloneLight) {
            cloneLightText->draw();
            cloneLightState->set(cloneLight->isOn() ? TEXT_ON : TEXT_OFF);
            cloneLightState->draw();
        }
        if (background) {
            backgroundText->draw();
            backgroundState->set(background->isOn() ? TEXT_ON : TEXT_OFF);
            backgroundState->draw();
        }
        if (discrete) {
            discreteText->draw();
            discreteState->set(discrete->isOn() ? TEXT_ON : TEXT_OFF);
            discreteState->draw();
        }
        if (locall) {
            locallText->draw();
            locallState->set(locall->isOn() ? TEXT_ON : TEXT_OFF);
            locallState->draw();
        }
        if (localr) {
            localrText->draw();
            localrState->set(localr->isOn() ? TEXT_ON : TEXT_OFF);
            localrState->draw();
        }
        
        display->endDraw();
    }
    display->endDrawFrame();
}

// suspends suspends the coordinator
//
void Design::suspend() {

    coordinator->suspend();
	display->suspend();
}

// restore re-initializes the reference time
//
void Design::restore(int now) {

	display->restore();
    coordinator->restore(now);
    lastUpdate = now;
}

// release releases the coordinator
//
void Design::release() {

    coordinator->release();
    display->release();
}

// destructor destroys the coordinator
//
Design::~Design() {

    coordinator->Delete();
    display->Delete();
    soundCard->Delete();
}

//-------------------------------- Text Generating Function -------------------
//
// orient returns a text string with the orientation of the local c axis
// of iFrame* frame
//
const wchar_t* orient(wchar_t* str, const iFrame* frame, char c, int f) {

	if (frame) {
        Vector v = frame->orientation(c);
		sprintf(str, (int)(f*v.x), (int)(f*v.y), (int)(f*v.z));
	}

	return str;
}

// position returns a text string with the world position of frame
//
const wchar_t* position(wchar_t* str, const iFrame* frame) {

	if (frame) {
        Vector v = frame->position();
		sprintf(str, (int)(v.x), (int)(v.y), (int)(v.z));
	}

	return str;
}

