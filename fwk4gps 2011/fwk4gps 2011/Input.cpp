/* Input Class' Implementation - Direct API Branch
 *
 * Input.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Input.h"         // for Keyboard, Mouse, and Controller
                           // class definitions

#include "iContext.h"      // for the Context Interface
#include "iUtilities.h"    // for error()
#include "Configuration.h" // for Mappable Keys KEY_?

const GUID GUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

BOOL CALLBACK countControllers(LPCDIDEVICEINSTANCE didesc, void* count);
BOOL CALLBACK enumControllerDesc(LPCDIDEVICEINSTANCE didesc, void* s);

//------------------------------- Keyboard ------------------------------------
//
// The Keyboard class ipmlements the keyboard at the API level
//
// CreateKeyboard creates the Keyboard object
//
iKeyboard* CreateKeyboard(void* hinst, iContext* c) {

	return new Keyboard(hinst, c);
}

// constructor retrieves Interface to DirectInput object and initializes
// the instance variable
//
Keyboard::Keyboard(void* hinst, iContext* c) : context(c) {

    // acquire an Interface to DirectInput object for this application
    di = NULL;
    if (FAILED(DirectInput8Create((HINSTANCE)hinst, DIRECTINPUT_VERSION, 
     IID_IDirectInput8, (void**)&di, NULL))) {
        error(L"Keyboard::00 Failed to obtain an Interface to Direct "
         L"Input");
    }

    keyboard = NULL;
}

// setup accesses the keyboard, sets its data format and cooperative
// level sets the size of the keyboard buffer and acquires the keyboard
//
bool Keyboard::setup(void* hwnd) {

    bool rc = false;

    // release the keyboard to start from scratch
    release();

    // obtain an interface to the keyboard
    if (FAILED(di->CreateDevice(GUID_SysKeyboard, &keyboard, NULL)))
        error(L"Keyboard::10 Failed to obtain an Interface to system "
         L"keyboard");
    // set the data format for the keyboard data
    else if (FAILED(keyboard->SetDataFormat(&c_dfDIKeyboard))) {
        release();
        error(L"Keyboard::11 Failed to set the data format for keyboard");
    }
    // set the cooperative level
    else if (FAILED(keyboard->SetCooperativeLevel((HWND)hwnd, 
     DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        release();
        error(L"Keyboard::12 Failed to set the cooperative level for "
         L"keyboard");
    }
    else {
        // set the size of the keyboard's buffer
        //
        // property struct consists of a header and a data member
        DIPROPDWORD dipdw;
        // property struct header
        // - size of enclosing structure
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        // - always size of DIPROPHEADER
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        // - identifier for property in question - 0 for entire device
        dipdw.diph.dwObj        = 0;
        // - DIPH_DEVICE since entire device is involved
        dipdw.diph.dwHow        = DIPH_DEVICE;
        // property struct data member (takes a single word of data)
        // - the buffer size goes here
        dipdw.dwData            = SAMPLE_BUFFER_SIZE;

        // set the size of the buffer
        if (FAILED(keyboard->SetProperty(DIPROP_BUFFERSIZE, 
		 &dipdw.diph))) {
            release();
            error(L"Keyboard::13 Failed to set size of keyboard buffer");
        }
        // flush buffer: data currently in the buffer will be ignored
        else {
			// try to acquire the keyboard
			if (SUCCEEDED(keyboard->Acquire())) {
				// clear buffer - this data will be ignored
                retrieveInput();
                rc = true;
            }
        }
    }

    return rc;
}

// retrieveInput retrieves the contents of the keyboard's buffer and stores
// key press/release values in keys[] for subsequent polling
//
void Keyboard::retrieveInput() {

    HRESULT hr;
    DWORD items = SAMPLE_BUFFER_SIZE;
    DIDEVICEOBJECTDATA dod[SAMPLE_BUFFER_SIZE];

    if (keyboard) {
        hr = keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), dod, 
		 &items, 0);
		// if keyboard is lost, try to re-acquire it
        if (DIERR_INPUTLOST == hr && SUCCEEDED(keyboard->Acquire()))
            hr = keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 
			 dod, &items, 0);
        if (SUCCEEDED(hr))
            for (DWORD i = 0; i < items; i++) {
                int k = -1;
                // note that not all keys have been included in this table -
                // only those keys that are mappable as described in the 
                // ModelSettings header
                //
               switch (dod[i].dwOfs) {
                  case DIK_A: k = KEY_A; break;
                  case DIK_B: k = KEY_B; break;
                  case DIK_C: k = KEY_C; break;
                  case DIK_D: k = KEY_D; break;
                  case DIK_E: k = KEY_E; break;
                  case DIK_F: k = KEY_F; break;
                  case DIK_G: k = KEY_G; break;
                  case DIK_H: k = KEY_H; break;
                  case DIK_I: k = KEY_I; break;
                  case DIK_J: k = KEY_J; break;
                  case DIK_K: k = KEY_K; break;
                  case DIK_L: k = KEY_L; break;
                  case DIK_M: k = KEY_M; break;
                  case DIK_N: k = KEY_N; break;
                  case DIK_O: k = KEY_O; break;
                  case DIK_P: k = KEY_P; break;
                  case DIK_Q: k = KEY_Q; break;
                  case DIK_R: k = KEY_R; break;
                  case DIK_S: k = KEY_S; break;
                  case DIK_T: k = KEY_T; break;
                  case DIK_U: k = KEY_U; break;
	              case DIK_V: k = KEY_V; break;
                  case DIK_W: k = KEY_W; break;
                  case DIK_X: k = KEY_X; break;
                  case DIK_Y: k = KEY_Y; break;
                  case DIK_Z: k = KEY_Z; break;
                  case DIK_1: k = KEY_1; break;
                  case DIK_2: k = KEY_2; break;
                  case DIK_3: k = KEY_3; break;
                  case DIK_4: k = KEY_4; break;
                  case DIK_5: k = KEY_5; break;
                  case DIK_6: k = KEY_6; break;
                  case DIK_7: k = KEY_7; break;
                  case DIK_8: k = KEY_8; break;
                  case DIK_9: k = KEY_9; break;
                  case DIK_0: k = KEY_0; break;
                  case DIK_F1:  k = KEY_F1;  break;
                  case DIK_F2:  k = KEY_F2;  break;
                  case DIK_F3:  k = KEY_F3;  break;
                  case DIK_F4:  k = KEY_F4;  break;
                  case DIK_F5:  k = KEY_F5;  break;
                  case DIK_F6:  k = KEY_F6;  break;
                  case DIK_F7:  k = KEY_F7;  break;
                  case DIK_F8:  k = KEY_F8;  break;
                  case DIK_F9:  k = KEY_F9;  break;
                  case DIK_F10: k = KEY_F10; break;
                  case DIK_F11: k = KEY_F11; break;
                  case DIK_F12: k = KEY_F12; break;
                  case DIK_SPACE : k = KEY_SPACE; break;
                  case DIK_RETURN: k = KEY_ENTER; break;
                  case DIK_UP    : k = KEY_UP;     break;
                  case DIK_DOWN  : k = KEY_DOWN;   break;
                  case DIK_PRIOR : k = KEY_PGUP;  break;
                  case DIK_NEXT  : k = KEY_PGDN;   break;
                  case DIK_LEFT  : k = KEY_LEFT;   break;
                  case DIK_RIGHT : k = KEY_RIGHT;  break;
                  case DIK_NUMPAD1:  k = KEY_NUM1; break;
                  case DIK_NUMPAD2:  k = KEY_NUM2; break;
                  case DIK_NUMPAD3:  k = KEY_NUM3; break;
                  case DIK_NUMPAD4:  k = KEY_NUM4; break;
                  case DIK_NUMPAD5:  k = KEY_NUM5; break;
                  case DIK_NUMPAD6:  k = KEY_NUM6; break;
                  case DIK_NUMPAD7:  k = KEY_NUM7; break;
                  case DIK_NUMPAD8:  k = KEY_NUM8; break;
                  case DIK_NUMPAD9:  k = KEY_NUM9; break;
                  case DIK_ESCAPE    : k = KEY_ESCAPE; break;
	              case DIK_SEMICOLON : k = KEY_SEMICOLON; break;
	              case DIK_APOSTROPHE: k = KEY_APOSTROPHE; break;
	              case DIK_LBRACKET  : k = KEY_O_BRACKET; break;
	              case DIK_RBRACKET  : k = KEY_C_BRACKET; break;
	              case DIK_BACKSLASH : k = KEY_BACKSLASH; break;
	              case DIK_COMMA     : k = KEY_COMMA; break;
	              case DIK_PERIOD    : k = KEY_PERIOD; break;
	              case DIK_SLASH     : k = KEY_SLASH; break;
	              case DIK_MULTIPLY  : k = KEY_TIMES; break;
	              case DIK_GRAVE     : k = KEY_GRAVE; break;
	              case DIK_MINUS     : k = KEY_MINUS; break;
	              case DIK_UNDERLINE : k = KEY_UNDERSCORE; break;
	              case DIK_EQUALS    : k = KEY_EQUALS; break;
	              case DIK_ADD       : k = KEY_PLUS; break;
                }
                if (k != -1) 
                    context->set(GF_KB_KEYS, k, !!(dod[i].dwData & 0x80));
            }
    }
}

// suspends unacquires the keyboard in preparation for loss of focus
//
void Keyboard::suspend() {

    if (keyboard) keyboard->Unacquire();
}

// restore re-acquires the keyboard on regaining focus
//
bool Keyboard::restore() {

    bool rc = true;

    if (keyboard) {
		HRESULT hr = keyboard->Acquire();
		if (hr != S_OK && hr != S_FALSE && hr != DIERR_OTHERAPPHASPRIO) {
            release();
            error(L"Keyboard::70 Failed to re-acquire the keyboard");
            rc = false;
        }
    }

    return rc;
}

// release suspends the keyboard object and disengages the interface
// to the keyboard device
//
void Keyboard::release() {

    suspend();
	if (keyboard) {
        keyboard->Release();
        keyboard = NULL;
    }
}

// destructor releases the keyboard object and disengages the interface to
// the Direct Input object
//
Keyboard::~Keyboard() {

    release();
    if (di) {
        di->Release();
        di = NULL;
    }
}

//------------------------------- Mouse ---------------------------------------
//
// The Mouse class implements the mouse at the API level
//
// CreateMouse creates the Mouse object
//
iMouse* CreateMouse(void* hinst, iContext* c) {

	return new Mouse(hinst, c);
}

// Constructor retrieves an interface to the DirectInput object and initializes
// instance variable
//
Mouse::Mouse(void* hinst, iContext* c) : context(c) {

    // acquire an Interface to DirectInput object for this application
    di = NULL;
    if (FAILED(DirectInput8Create((HINSTANCE)hinst, DIRECTINPUT_VERSION, 
     IID_IDirectInput8, (void**)&di, NULL))) {
         error(L"Mouse::00 Failed to obtain an Interface to Direct"
		  L"Input");
    }

    mouse = NULL;
}

// setup accesses the system mouse, sets its data format and cooperative 
// level, sets the size of the mouse's buffer and acquires the device
//
bool Mouse::setup(void* hwnd) {

    bool rc = false;

    // release the mouse object to start from scratch
    release();

    // obtain an interface to the system mouse
    if (FAILED(di->CreateDevice(GUID_SysMouse, &mouse, NULL)))
        error(L"Mouse::10 Failed to obtain an Interface to system"
		 L"mouse");
    // set the data format for mouse data
    else if (FAILED(mouse->SetDataFormat(&c_dfDIMouse))) {
        release();
        error(L"Mouse::11 Failed to set the data format for mouse");
    }
    // set the cooperative level
    else if (FAILED(mouse->SetCooperativeLevel((HWND)hwnd, 
	 DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        release();
        error(L"Mouse::12 Failed to set cooperative level for mouse");
    }
    else {
        // set the size of the mouse's buffer
        //
        // proerty structure consists of a header and a data member
        DIPROPDWORD dipdw;
        // property header
        // - size of enclosing structure
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        // - always size of DIPROPHEADER
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        // - identifier for property in question - 0 for entire device
        dipdw.diph.dwObj        = 0;
        // - DIPH_DEVICE since entire device is involved
        dipdw.diph.dwHow        = DIPH_DEVICE;
        // property data member (takes a single word of data)
        // - the buffer size goes here
        dipdw.dwData            = SAMPLE_BUFFER_SIZE;

        // set the buffer size
        if (FAILED(mouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph))){
            release();
            error(L"Mouse::13 Failed to set the buffer size");
        }
        // flush the buffer: data currently in buffer will be ignored
        else {
			// try to acquire the mouse
            HRESULT hr = mouse->Acquire();
			if (SUCCEEDED(hr)) {
				// clear the buffer - this data will be ignored
                retrieveInput();
                rc = true;
            }
            else if (hr == S_FALSE)
                // already acquired
                rc = true;
            context->set(GF_MS_BTNS, 0, false);
            context->set(GF_MS_BTNS, 1, false);
        }
    }

    return rc;
}

// retrieveInput retrieves the contents of the mouse's buffer and accumulates
// the values for subsequent polling
//
void Mouse::retrieveInput() {

    HRESULT hr;
    DWORD items = SAMPLE_BUFFER_SIZE;
    DIDEVICEOBJECTDATA dod[SAMPLE_BUFFER_SIZE];

    if (mouse) {
        hr = mouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), dod, 
		 &items, 0);
		// try to re-acquire if lost
        if (DIERR_INPUTLOST == hr && SUCCEEDED(mouse->Acquire()))
            hr = mouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), dod,
             &items, 0);
        if (SUCCEEDED(hr)) {
            int x = 0, y = 0, z = 0;
            for (DWORD i = 0; i < items; i++) {
                switch (dod[i].dwOfs) {
                  case DIMOFS_BUTTON0:
                      context->set(GF_MS_BTNS, 0, (dod[i].dwData & 0x80) == 0x80);
                      break;
                  case DIMOFS_BUTTON1:
                      context->set(GF_MS_BTNS, 1, (dod[i].dwData & 0x80) == 0x80);
                      break;
                  case DIMOFS_X:
                      x += dod[i].dwData;
                      break;
                  case DIMOFS_Y:
                      y += dod[i].dwData;
                      break;
                  case DIMOFS_Z:
                      z += dod[i].dwData;
                      break;
                }
            }
            context->set(GF_MS_DSPX, x);
            context->set(GF_MS_DSPY, y);
            context->set(GF_MS_ROTZ, z);
        }
    }
}

// suspends unacquires the mouse in preparation for loss of focus
//
void Mouse::suspend() {

    if (mouse) {
        mouse->Unacquire();
        context->set(GF_MS_BTNS, 0, false);
        context->set(GF_MS_BTNS, 1, false);
    }
}

// restore re-acquires the mouse on regaining focus
//
bool Mouse::restore() {

    bool rc = true;

    if (mouse) {
		HRESULT hr = mouse->Acquire();
		if (hr != S_OK && hr != S_FALSE && hr != DIERR_OTHERAPPHASPRIO) {
            release();
            error(L"Mouse::70 Failed to re-acquire the mouse");
            rc = false;
        }
    }

    return rc;
}

// release suspends the mouse object and disengages the interface to
// the mouse device
//
void Mouse::release() {

    suspend();
	if (mouse) {
        mouse->Release();
        mouse = NULL;
    }
}

// destructor releases the mouse object and disengages the Interface to
// the Direct Input object
//
Mouse::~Mouse() {

    release();
    if (di) {
        di->Release();
        di = NULL;
    }
}

//------------------------------- Controller ------------------------------------
//
// The Controller class implements the controller at the API level
//
int ControllerDesc::count = 0;

// CreateController creates the controller object
//
iController* CreateController(void* hinst, iContext* c) {

	return new Controller(hinst, c);
}

// Constructor retrieves an interface to the DirectInput object and initializes
// the instance variables
//
Controller::Controller(void* hinst, iContext* c) : context(c) {

    // acquire an Interface to DirectInput object for this application
    di = NULL;
    if (FAILED(DirectInput8Create((HINSTANCE)hinst, DIRECTINPUT_VERSION, 
     IID_IDirectInput8, (void**)&di, NULL))) {
        error(L"Controller::00 Failed to obtain an Interface to Direct "
         L"Input");
    }

    attached   = NULL;
    controller = NULL;
    axisIsActive[0] = axisIsActive[1] = axisIsActive[2] = axisIsActive[3] = 
     povIsActive = reversey = false;
}

// interrogate populates the descriptions of all attached controllers
//
bool Controller::interrogate(void* hwnd) {

    bool rc    = false;
    int  count = 0;

	// find the number of attached controllers and allocate memory
    di->EnumDevices(DI8DEVCLASS_GAMECTRL, 
	 (LPDIENUMDEVICESCALLBACK)countControllers, (void*)&count, 
	 DIEDFL_ATTACHEDONLY);
    if (attached) 
        delete [] attached;
	attached = new ControllerDesc[count];

    // enumerate controller descriptions
    attached->count = 0;
	di->EnumDevices(DI8DEVCLASS_GAMECTRL, 
	 (LPDIENUMDEVICESCALLBACK)enumControllerDesc, (void*)attached, 
	 DIEDFL_ATTACHEDONLY);
    if (attached->count > MAX_CONTROLLERS) {
        wchar_t str[MAX_DESC + 1];
        sprintf(str, attached->count, 
         L" Controllers found - increase MAX_CONTROLLERS");
        error(str);
        attached->count = MAX_CONTROLLERS;
    }

    // set number of controllers and allocate context memory
    context->set(GF_CT_CCNT, attached->count);

	// store each controller that creates successfully
    DIDEVICEOBJECTINSTANCE didoi;   // holds object info for the controller
	didoi.dwSize = sizeof didoi;
    for (int i = 0; i < attached->count; i++) {
        // create the controller temporarily just to interrogate it
        bool ok = false;
        LPDIRECTINPUTDEVICE8 controller; // points to the controller
        GUID guid = attached[i].guid;
	    if (guid != GUID_NULL && 
         SUCCEEDED(di->CreateDevice(guid, &controller, NULL)) && 
         SUCCEEDED(controller->SetDataFormat(&c_dfDIJoystick2))) { 
		    // examine each of the controller's buttons
            for (int b = 0; b < MAX_C_BUTTONS; b++) {
		        if (SUCCEEDED(controller->GetObjectInfo(&didoi, 
                 DIJOFS_BUTTON(b), DIPH_BYOFFSET))) {
                    int o = i * MAX_C_BUTTONS + b;
                    context->set(GF_CTBDESC, o, didoi.tszName);
		        }
            }
			ok = true;
		    controller->Release();
		    controller = NULL;
        }
        if (ok) {
            rc = true;
            context->set(GF_CTRDESC, i, attached[i].desc);
        }
    }

    return rc || count == 0;
}

// countControllers increments the counter stored in address a
//
BOOL CALLBACK countControllers(LPCDIDEVICEINSTANCE didesc, void* a) {

	(*(int*)a)++;

    return DIENUM_CONTINUE;
}

// enumControllerDesc saves the description of an enumerated controller
//
BOOL CALLBACK enumControllerDesc(LPCDIDEVICEINSTANCE didesc, void* a) {
    
	ControllerDesc* attached = (ControllerDesc*)a;
	
	int i = attached->count++;
    strcpy(attached[i].desc, didesc->tszInstanceName, MAX_DESC);
	attached[i].guid = didesc->guidInstance;

    return DIENUM_CONTINUE;
}

// setup accesses the controller, sets up its data format and cooperative
// level, sets the buffer size, and acquires the controller
//
bool Controller::setup(void* hwnd) {

    bool rc = false;

    // release the controller object to start from scratch
    release();

    // retrieve the controller context
    int  ic      = context->get(GF_CT_INDX);
    int  button  = context->get(GF_CT_TGRB);
    int  flags   = context->get(GF_CT_FLGS);
    bool none    = flags & 1;
    bool zAxisOn = !!(flags & 2);
    reversey     = !!(flags & 4);
    GUID guid    = none ? GUID_NULL : attached[ic].guid;

    // no controller selected
  //  if (none)
  //      rc = true;
  //  // obtain an interface to the controller
  //  else if (FAILED(di->CreateDevice(guid, &controller, NULL)))
  //      error(L"Controller::10 Failed to create an Interface to controller");
  //  // set the data format for the controller
  //  else if (FAILED(controller->SetDataFormat(&c_dfDIJoystick2))) {
  //      release();
  //      error(L"Controller::11 Failed to set the data format for controller");
  //  }
  //  // set the cooperative level
  //  else if (FAILED(controller->SetCooperativeLevel((HWND)hwnd, 
	 //DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
  //      release();
  //      error(L"Controller::12 Failed to set the behavior for controller");
  //  }
  //  else {
  //      // retrieve the axes that are active on this device
  //      DIDEVICEOBJECTINSTANCE didoi;
  //      didoi.dwSize = sizeof didoi;
  //      if (SUCCEEDED(controller->GetObjectInfo(&didoi, DIJOFS_X,
  //       DIPH_BYOFFSET)))
  //          axisIsActive[0] = true;
  //      if (SUCCEEDED(controller->GetObjectInfo(&didoi, DIJOFS_Y,
  //       DIPH_BYOFFSET)))
  //          axisIsActive[1] = true;
  //      if (SUCCEEDED(controller->GetObjectInfo(&didoi, DIJOFS_Z,
  //       DIPH_BYOFFSET)))
  //          axisIsActive[2] = true;
  //      if (SUCCEEDED(controller->GetObjectInfo(&didoi, DIJOFS_RZ,
  //       DIPH_BYOFFSET)))
  //          axisIsActive[3] = true;
  //      // ignore what GetObjectInfo returned if we don't want z axis
  //      if (!zAxisOn) {
  //          axisIsActive[2] = false;
  //          axisIsActive[3] = false;
  //      }

  //      // Set the range, deadzone, and saturation for each axis

  //      DIPROPRANGE range;

  //      range.diph.dwSize = sizeof range;
  //      range.diph.dwHeaderSize = sizeof range.diph;
  //      range.diph.dwObj = DIJOFS_X;
  //      range.diph.dwHow = DIPH_BYOFFSET;
  //      range.lMin = -100;
  //      range.lMax =  100;

  //      DIPROPDWORD dead,
  //                  sat;

  //      dead.diph.dwSize = sizeof dead;
  //      dead.diph.dwHeaderSize = sizeof dead.diph;
  //      dead.diph.dwObj = DIJOFS_X;
  //      dead.diph.dwHow = DIPH_BYOFFSET;
  //      dead.dwData = 300; // hundredths of a percent [0,10000]

  //      sat = dead;
  //      sat.dwData = 9800;

  //      if (axisIsActive[0]) {
  //          controller->SetProperty(DIPROP_RANGE, &range.diph);
  //          controller->SetProperty(DIPROP_DEADZONE, &dead.diph);
  //          controller->SetProperty(DIPROP_SATURATION, &sat.diph);
  //      }

  //      if (axisIsActive[1]) {
  //          range.diph.dwObj = DIJOFS_Y;
  //          dead.diph.dwObj  = DIJOFS_Y;
  //          sat.diph.dwObj   = DIJOFS_Y;
  //          controller->SetProperty(DIPROP_RANGE, &range.diph);
  //          controller->SetProperty(DIPROP_DEADZONE, &dead.diph);
  //          controller->SetProperty(DIPROP_SATURATION, &sat.diph);
  //      }

  //      if (axisIsActive[2]) {
  //          range.diph.dwObj = DIJOFS_Z;
  //          dead.diph.dwObj  = DIJOFS_Z;
  //          sat.diph.dwObj   = DIJOFS_Z;
  //          controller->SetProperty(DIPROP_RANGE, &range.diph);
  //          controller->SetProperty(DIPROP_DEADZONE, &dead.diph);
  //          controller->SetProperty(DIPROP_SATURATION, &sat.diph);
  //      }

  //      if (axisIsActive[3]) {
  //          range.diph.dwObj = DIJOFS_RZ;
  //          dead.diph.dwObj  = DIJOFS_RZ;
  //          sat.diph.dwObj   = DIJOFS_RZ;
  //          controller->SetProperty(DIPROP_RANGE, &range.diph);
  //          controller->SetProperty(DIPROP_DEADZONE, &dead.diph);
  //          controller->SetProperty(DIPROP_SATURATION, &sat.diph);
  //      }

		//// try to acquire the controller
		//if (SUCCEEDED(controller->Acquire()))
  //          rc = true;
  //  }

    return true;
}

// retrieve retrieves the current state of the controller and stores
// the axes' and button values for subsequent polling
//
void Controller::retrieveInput() {

    HRESULT hr;
    DIJOYSTATE2 state;

    if (controller) {
        // make the current state available
        controller->Poll();
        // retrieve the state of the controller
        hr = controller->GetDeviceState(sizeof(DIJOYSTATE2), &state);
        if (DIERR_INPUTLOST == hr && SUCCEEDED(controller->Acquire()))
            hr = controller->GetDeviceState(sizeof(DIJOYSTATE2), &state);
        if (SUCCEEDED(hr)) {
            // current state components
            int x = 0, y = 0, z = 0, r = 0;
            if (axisIsActive[0])
                x = state.lX;
            if (axisIsActive[1])
                y = reversey ? -state.lY : state.lY;
            if (axisIsActive[2])
                z = state.lZ;
            if (axisIsActive[3])
                r = state.lRz;
            if (povIsActive)
                for (int i = 0; i < 4; i++)
                    pointOfView[0] = state.rgdwPOV[i];
            // buttons currently pressed
           for (int i = 0; i < MAX_C_BUTTONS; i++)
                context->set(GF_CT_BTNS, i, (state.rgbButtons[i] & 0x80) != 0);
            context->set(GF_CT_POSX, x);
            context->set(GF_CT_POSY, y);
            context->set(GF_CT_DSPZ, z);
            context->set(GF_CT_ROTZ, r);
        }
    }
}

// suspends unacquires the controller in preparation for loss of focus
//
void Controller::suspend() {

    if (controller) controller->Unacquire();
}

// restore re-acquires the controller
//
bool Controller::restore() {

    bool rc = true;

    if (controller) {
		HRESULT hr = controller->Acquire();
		if (hr != S_OK && hr != S_FALSE && hr != DIERR_OTHERAPPHASPRIO) {
            release();
            error(L"Controller::70 Failed to re-acquire the controller");
            rc = false;
        }
    }

    return rc;
}

// release suspends the controller object and detaches the interface
// to the controller device
//
void Controller::release() {

    suspend();
	if (controller) {
        controller->Release();
        controller = NULL;
    }
}

// destructor releases the controller object, disengages the interface
// to the Direct Input object, and deallocates the memory for the
// controller descriptions
//
Controller::~Controller() {

    release();
    if (di) {
        di->Release();
        di = NULL;
    }
    if (attached)
        delete [] attached;
}
