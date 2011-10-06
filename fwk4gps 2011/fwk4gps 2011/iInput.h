#ifndef _I_INPUT_H_
#define _I_INPUT_H_

/* Input Interface - Direct API Branch
 *
 * consists of iKeyboard Interface
 *             iMouse Interface
 *             iController Interface
 *
 * Input.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//------------------------------- iKeyboard -----------------------------------
//
// iKeyboard is the Interface to the Keyboard class
//
class iContext;

class iKeyboard {
  public:
	// initialization
    virtual bool  setup(void* hwnd)                   = 0;
	// execution
    virtual void  retrieveInput()                     = 0;
    // suspend execution
    virtual void  suspend()                           = 0;
    virtual bool  restore()                           = 0;
	// termination
    virtual void  release()                           = 0;
    virtual void  Delete() const                      = 0;
};

iKeyboard* CreateKeyboard(void*, iContext*);

//------------------------------- iMouse --------------------------------------
//
// iMouse is the Interface to the Mouse class
//
class iMouse {
  public:
	// initialization
    virtual bool setup(void* hwnd)                                   = 0;
	// execution
    virtual void retrieveInput()                                     = 0;
    // suspend execution
    virtual void suspend()                                           = 0;
    virtual bool restore()                                           = 0;
	// termination
    virtual void release()                                           = 0;
    virtual void Delete() const                                      = 0;
};

iMouse* CreateMouse(void*, iContext*);

//---------------------------------- iController --------------------------------
//
// iController is the Interface to the Controller class
//
class iController {
  public:
	// interrogation
    virtual bool interrogate(void*)                                = 0;
	// initialization
    virtual bool setup(void*)                                      = 0;
	// execution
    virtual void retrieveInput()                                   = 0;
    // suspend execution
    virtual void suspend()                                         = 0;
    virtual bool restore()                                         = 0;
	// termination
    virtual void release()                                         = 0;
    virtual void Delete() const                                    = 0;
};

iController* CreateController(void*, iContext*);

#endif
