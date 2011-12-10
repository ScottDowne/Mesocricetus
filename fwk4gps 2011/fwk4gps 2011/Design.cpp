/* Design Implementation - Model Branch
 *
 * Design.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */
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
#include "Maze.h"
#include <ctime>

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

	// pointers to the lights
    pointLight   = NULL;

    // pointers to the sounds

    // pointers to the text items
	timerText = NULL;

	midi = NULL;

    //maze
	maze;
	gameOver = false;

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
	startTime = now;
	timeLeft = TIME_LIMIT;

    // projection parameters
    //
    context->set(GF_FR_NEAR, NEAR_CLIPPING);
    context->set(GF_FR_FAR, FAR_CLIPPING);
    context->set(GF_FR_FOV, FIELD_OF_VIEW);
	
    // maze ---------------------------------------------------------
	maze = new Maze();

    // cameras ----------------------------------------------------------------

    // camera at a distance - in lhs coordinates
    (camera = CreateCamera(context, maze))->translate((19 + 0.5) * SCALE, 0.5 * SCALE, 1.5 * SCALE);
    camera->setRadius(17.8f);
	
    // coordinator ------------------------------------------------------------------

    Colour wallColor(0.6f, 0.6f, 0.45f);
    Colour yellow(0.7f, 0.7f, 0.0f);
    Colour grey(0.7f, 0.7f, 0.7f);
    Colour turquoise(0, 0.8f, 0.6f);
    Colour black(0, 0, 0);

    Reflectivity greyish = Reflectivity(grey);

	// create textures
    
    // create vertex lists

    // lighting ---------------------------------------------------------------

    // global ambient light: a little bit of light on everything, for when all 
	// "lights are out"
    context->set(GF_LT_BKGD, Vector(.2f, .2f, .2f));

    // Create far away point-source light
	 //pointLight = CreatePointLight(grey, grey, white, 20000.0f, true);
    //pointLight->translate(500.f, 1000.f, 100.f * MODEL_Z_AXIS);
    iLight * spotLight = CreateSpotLight(wallColor, wallColor, wallColor, 200.0f, true, 1, 0.00005f,
      0.00001f, .60f, 0, 0.9f);
    spotLight->translate((19 + 0.5) * SCALE, 0.5 * SCALE, (1.5 * SCALE) - 5 );
    spotLight->attachTo(camera);
	// audio ------------------------------------------------------------------

	midi = CreateSound( L"Crickets (by reinsamba) .xwma", LOCAL_SOUND, true, true, 360 );
	midi->translate((1.5) * SCALE, 0.5 * SCALE, 21.5 * SCALE);

	//midi->rotatey(3.14f);

    // Heads Up Display -------------------------------------------------------
	
	timerText = CreateText(RelRect(0, 0, 10.9f, 10.1f), L"");

	// reference time
    lastUpdate = now;
}

// reset changes the context if any change has been requested and
// reinitializes the reference time
//
void Design::reset(int now) {

    coordinator->reset(now);

	// audio ------------------------------------------------------------------
	
	// reset reference time ---------------------------------------------------
    //
    lastUpdate = now;
}

// update updates the position and orientation of each object for time "now"
// according to the keys pressed
//
void Design::update(int now) {

    coordinator->update(now);

	timeLeft = (int)(TIME_LIMIT - (difftime(lastUpdate, startTime)/1000));

	int condition = maze->checkCondition();

	if (!gameOver) {

		if (timeLeft < 0 && condition <= 0) {

			gameOver = true;
		  strcpy(timerString, L"You lose...", 11);
		} else {
	
			if (condition == 1) {
			  gameOver = true;
			  strcpy(timerString, L"You win!", 8);
			} else if (condition == -1) {

				// reset timer when user is at start point
				startTime = now;
				sprintf(timerString, timeLeft, L"");
			} else {
		
			  sprintf(timerString, timeLeft, L"");
			}
		}
	}

	// audio ------------------------------------------------------------------

    midi->update();

    // lighting ---------------------------------------------------------------

    // coordinator ------------------------------------------------------------------
	    if (context->pressed(TOGGLE_COLLISION) || context->pressed(X_BUTTON)) {
          maze->toggleCollision(false);
		} else {
			maze->toggleCollision(true);
		}

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

        if (timerText) {

            timerText->set(timerString);
            timerText->draw();
        }
        display->endDraw();
    }
    display->endDrawFrame();
}

// suspends suspends the coordinator
//
void Design::suspend() {

    coordinator->suspend();
	midi->suspend();
	display->suspend();
}

// restore re-initializes the reference time
//
void Design::restore(int now) {

	display->restore();
	midi->restore(now);
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
	
	if (midi) midi->Delete();
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
