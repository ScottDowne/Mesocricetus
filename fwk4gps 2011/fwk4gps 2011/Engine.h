#ifndef _ENGINE_H_
#define _ENGINE_H_

/* Engine Definition
 *
 * Engine.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iEngine.h" // for the Engine Interface

//-------------------------------- Engine -------------------------------------
//
// The Engine controls the flow of execution throughout the Framework
//
class iUserDialog;
class iWindow;
class iDesign;
class iKeyboard;
class iMouse;
class iController;

class Engine : public iEngine {

    static iEngine* address;       // points to this object

    bool            active;        // engine is active?
    int             lastDrawn;     // time that a frame was last drawn
    void*           hwnd;          // points to the main application window

    // windows
	iUserDialog*    userDialog;    // points to the UserDialog object
    iWindow*        window;        // points to the Window object

    // coordinator
    iDesign*        design;        // points to the Design object

    // devices
    iKeyboard*      keyboard;      // points to the Keyboard object
    iMouse*         mouse;         // points to the Mouse object
    iController*    controller;    // points to the Controller object

    Engine(void*, int, iContext*);
	Engine(const Engine& m);            // prevents copying
	Engine& operator=(const Engine& m); // prevents assignment
    virtual ~Engine();
    bool    configure();

  public:
    friend iEngine* CreateEngine(void*, int, iContext*);
    static iEngine* Address() { return address; }
	// configuration
    void   getConfigurations(void*);
	void   reconfigure();
	// execution
    bool   isActive() const   { return active; }
	int    run();
    void   resize() const     { design->resize(); }
	// suspension
    void   suspend();
    void   restore();
	// termination
	void   Delete() const     { delete this; }
};

iEngine* EngineAddress()      { return Engine::Address(); }

#endif