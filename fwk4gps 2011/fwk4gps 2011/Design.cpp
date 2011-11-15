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
#include <cstdlib>
#include <ctime>
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

	// pointers to the lights
    pointLight   = NULL;

    // pointers to the sounds

    // pointers to the text items

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

void Design::generateMazeWallH(int x1, int y1, int x2, int y2) {
	
  int wallLocation = 0;
  do {
  
    wallLocation = (rand() % (y2-y1)) + y1;
  } while (wallLocation % 2 != 0);

  int holeLocation = 0;
  do {
  
    holeLocation = (rand() % (x2-x1)) + x1;
  } while (holeLocation % 2 ==0);

  for (int x = x1; x < x2; x++) {
  
    if (x != holeLocation) MAZE_ARRAY[wallLocation][x] = 0;
  }
  
  if (x2 - x1 >= 3) {

    if (wallLocation - y1 > 1) generateMazeWallV(x1, y1, x2, wallLocation);
    if (y2 - wallLocation > 1) generateMazeWallV(x1, wallLocation+1, x2, y2);
  }
}

void Design::generateMazeWallV(int x1, int y1, int x2, int y2) {

  int wallLocation = 0;
  do {
  
    wallLocation = (rand() % (x2-x1)) + x1;
  } while (wallLocation % 2 != 0);

  int holeLocation = 0;
  do {
  
    holeLocation = (rand() % (y2-y1)) + y1;
  } while (holeLocation % 2 == 0);

  for (int y = y1; y < y2; y++) {
  
    if (y != holeLocation) MAZE_ARRAY[y][wallLocation] = 0;
  }

  if (y2 - y1 >= 3) {

    if (wallLocation - x1 > 1) generateMazeWallH(x1, y1, wallLocation, y2);
    if (x2 - wallLocation > 1) generateMazeWallH(wallLocation+1, y1, x2, y2);
  }
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

	// randomly generate a maze
	srand((unsigned)time(0));
	generateMazeWallH(1, 1, MAZE_ARRAY_ROW-1, MAZE_ARRAY_COL-1 );

	//MAZE_ARRAY[5][5] = 0;

	// render maze
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

             if (y == 0 || MAZE_ARRAY[y-1][x] > 0) {
            
                back = 1;
             }

             if (y == MAZE_ARRAY_ROW-1 || MAZE_ARRAY[y+1][x] > 0) {
            
                front = 1;
             }

		     CreateObject(CreateBox(x, 0, MAZE_ARRAY_COL - y, x+1, 1, (MAZE_ARRAY_COL - y) + 1, front, right, back, left, 0, 1), &greyish);
          }
          else {

		     CreateObject(CreateBox(x, -1, MAZE_ARRAY_COL - y, x+1, 0, (MAZE_ARRAY_COL - y) + 1, 0, 0, 0, 0, 0, 1), &greyish);
          }
          
       }
    }

	// create textures

    // create vertex lists

    // lighting ---------------------------------------------------------------

    // global ambient light: a little bit of light on everything, for when all 
	// "lights are out"
    context->set(GF_LT_BKGD, Vector(.2f, .2f, .2f));

    // Create far away point-source light
	pointLight = CreatePointLight(grey, grey, white, 20000.0f, true);
    pointLight->translate(500.f, 1000.f, 100.f * MODEL_Z_AXIS);

	// audio ------------------------------------------------------------------

    // Heads Up Display -------------------------------------------------------

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

	// audio ------------------------------------------------------------------

    // lighting ---------------------------------------------------------------

    // coordinator ------------------------------------------------------------------

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
        // change the world
        
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

