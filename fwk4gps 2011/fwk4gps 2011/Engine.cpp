/* Engine Implementation  
 *                       
 * Engine.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iUserDialog.h"   // for the UserDialog Interface
#include "iWindow.h"       // for the Window Interface
#include "iInput.h"        // for the Input Interfaces
#include "iDesign.h"       // for the Design Interface
#include "iUtilities.h"    // for error()

#include "Engine.h"        // for the Engine class definition
#include "Configuration.h" // for NULL, MIN_ELAPSED_TIME, UNITS_PER_SEC

//-------------------------------- Engine -------------------------------------
//
// The Engine manages the flow of control throughout the Framework
//
iEngine* Engine::address = 0;

// CreateEngine creates the Engine Object and returns its address
//
iEngine* CreateEngine(void* hinst, int s, iContext* c) {

    return Engine::address ? Engine::address : new Engine(hinst, s, c);
}

// Engine constructor creates the User Dialog, the Window, the Input Devices, 
// and the Coordinator
//
Engine::Engine(void* hinst, int show, iContext* context) {

    address    = this;
 
	userDialog = CreateUserDialog(hinst, context);
    window     = CreateMainWindow(hinst, show, context);
    keyboard   = CreateKeyboard(hinst, context);
    mouse      = CreateMouse(hinst, context);
    controller = CreateController(hinst, context);
    design     = CreateDesign(context);

    hwnd       = NULL;

    // configure the application
    if (configure()) {
        // initialize the coordinator
        lastDrawn = window->time();
        design->initialize(lastDrawn);
        window->moveToForeground();
    }
    else
        lastDrawn = 0;
}

// getConfigurations interrogates the devices for available configurations 
//
void Engine::getConfigurations(void* hwnd) {

    design->interrogate(hwnd);
    controller->interrogate(hwnd);
}

// configure configures the context with the user's choices, sets up the
// main application window, sets up the input devices and the coordinator

bool Engine::configure() {

    bool rc = false;

    // configure the context with the user's choices
    if(userDialog->configureContext()) {

        // release the device interfaces
        design->release();
        window->release();

        // set the configuration for the user's choices
        design->configure();

        // create the main application window
        if (!(hwnd = window->create()))
		    error(L"Engine::10 Failed to setup the window object");

        else {
	        // setup the mouse - optional
	        if (!mouse->setup(hwnd))
		        error(L"Engine::11 Failed to setup the mouse object");

	        // setup the controller - optional
	        if (!controller->setup(hwnd))
		        error(L"Engine::12 Failed to setup the controller object");

	        // setup the keyboard - necessary
	        if (!keyboard->setup(hwnd))
		        error(L"Engine::13 Failed to setup the keyboard object");

	        // setup the coordinator - necessary
	        else if (!design->setup(hwnd))
		        error(L"Engine::14 Failed to set up the design object");

	        else 
                rc = true;
        }
    }

    active = rc;

    return rc;
}

// reconfigure reconfigures the application
//
void Engine::reconfigure() {

    // configure the application
    if (configure()) {
		// reset the coordinator
        lastDrawn = window->time();
        design->reset(lastDrawn);
        window->moveToForeground();
	}
}

// run handles system messages, retrieves user input, updates the coordinator 
// components, renders the new frame, and plays/stops any sounds that 
// are ready to start/stop
//
// asks the Window object if there is a message in the message queue
// if there is, lets the Window object handle the message
// if there isn't and the Engine is inactive, adopts a waiting state
// if there isn't and the Engine is active, performs an event cycle
//
int Engine::run() {

	int  rc        = 0;
    bool keepgoing = active;

	while (keepgoing) {
        // all system messages have priority
		if (window->getMessage(rc, keepgoing)) 
			; // intentional
        // shift into a waiting state
		else if (!active) 
			window->wait(); 
        // room to create a frame
		else {
	        // if sufficient time has elapsed since the last frame
            int now = window->time();
	        if (now - lastDrawn >= MIN_ELAPSED_TIME) {
		        // create a new frame
		        //
			    lastDrawn = now;
                // retrieve user input, if any
	            keyboard->retrieveInput();
	            mouse->retrieveInput();
	            controller->retrieveInput();

                // update the coordinator
                design->update(now);

	            // render the design
                design->render();
	        }
        }
	}

    return rc;
}

// suspend suspends the coordinator and the input devices
//
void Engine::suspend() {

	// coordinator
	design->suspend();

    // devices
	controller->suspend();
	mouse->suspend();
	keyboard->suspend();

    active = false;
}

// restore restores the input devices and the coordinator
//
void Engine::restore() {

	// devices
    keyboard->restore();
	mouse->restore();
	controller->restore();

    // coordinator
    lastDrawn = window->time();
    design->restore(lastDrawn);
	active     = true;
}

// destructor deletes the coordinator, the devices, and the windows
//
Engine::~Engine() {

	// coordinator
    design->Delete();

    // devices
    controller->Delete();
    mouse->Delete();
    keyboard->Delete();

    // windows
	window->Delete();
	userDialog->Delete();

    address = 0;
}

