#ifndef _INPUT_H_
#define _INPUT_H_

/* Input Definition - Direct API Branch
 *
 * consists of Keyboard   definition
 *             Mouse      definition
 *             Controller definition
 *
 * Input.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define  WIN32_LEAN_AND_MEAN
#define  INITGUID          // enables access to GUIDs used by DirectInput
#define  DIRECTINPUT_VERSION 0x0800
#include <dinput.h>        // for basic Direct Input
#undef   INITGUID

#include "iInput.h"        // for Keyboard, Mouse, and Controller Interfaces

#include "Configuration.h" // for MAX_DESC

//------------------------------- Keyboard ------------------------------------
//
// The Keyboard class implements the keyboard at the API level
//
class Keyboard : public iKeyboard {

	static const int SAMPLE_BUFFER_SIZE =  30;

    iContext*            context;      // points to the Context object
    LPDIRECTINPUT8       di;           // points to the Direct Input object
    LPDIRECTINPUTDEVICE8 keyboard;     // points to the Direct Input Keyboard

    Keyboard(void*, iContext*);
	Keyboard(const Keyboard& k);            // prevents copying
	Keyboard& operator=(const Keyboard& k); // prevents assignment
    virtual ~Keyboard();

  public:
	friend iKeyboard* CreateKeyboard(void*, iContext*);
	// initialization
    bool   setup(void*);
	// execution
    void   retrieveInput();
    // suspend execution
    void   suspend();
    bool   restore();
	// termination
    void   release();
	void   Delete() const { delete this; }
};

//------------------------------- Mouse ---------------------------------------
//
// The Mouse class implements the mouse at the API level
//
class Mouse : public iMouse {

	static const int SAMPLE_BUFFER_SIZE = 30;

    iContext*            context;  // points to the Context object
	LPDIRECTINPUT8       di;       // points to the Direct Input object
    LPDIRECTINPUTDEVICE8 mouse;    // points to the Direct Input mouse

    Mouse(void*, iContext*);
	Mouse(const Mouse& m);            // prevents copying
	Mouse& operator=(const Mouse& m); // prevents assignment
    virtual ~Mouse();

  public:
	friend iMouse* CreateMouse(void*, iContext*);
	// initialization
    bool   setup(void*);
	// execution
    void   retrieveInput();
    // suspend execution
    void   suspend();
    bool   restore();
    // termination
    void   release();
	void   Delete() const { delete this; }
};

//---------------------------------- controller ---------------------------------
//
// The Controller class implements the controller at the API level
//
struct ControllerDesc {
	GUID    guid;
	wchar_t desc[MAX_DESC + 1];
    static int count;
};

class Controller : public iController {

    iContext*            context;    // points to the Context object
    LPDIRECTINPUT8       di;         // points to the Direct Input object
    LPDIRECTINPUTDEVICE8 controller; // points to the Direct Input controller

	// interrogation
	ControllerDesc*      attached;   // points to descriptions of all 
								     // attached controllers
    // execution
    bool axisIsActive[4];            // is axis active?
    bool povIsActive;                // point of view is active?
    int  pointOfView[4];             // point of view
    bool reversey;                   // reverse direction of y axis

    Controller(void*, iContext*);
	Controller(const Controller& j);            // prevents copying
	Controller& operator=(const Controller& j); // prevents assignment
    virtual ~Controller();

  public:
	friend iController* CreateController(void*, iContext*);
	// interrogation
    bool   interrogate(void*);
    // initialization
    bool   setup(void*);
	// execution
    void   retrieveInput();
    // suspend execution
    void   suspend();
    bool   restore();
    // termination
    void   release();
	void   Delete() const { delete this; }
};

#endif
