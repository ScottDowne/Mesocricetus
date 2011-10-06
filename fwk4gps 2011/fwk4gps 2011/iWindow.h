#ifndef _I_WINDOW_H_
#define _I_WINDOW_H_

/* Window Interface - Window API Branch
 *
 * iWindow.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iWindow -------------------------------
//
// iWindow is the Interface to the Window class
//
class iContext;

class iWindow {
  public:
	// initialization
    virtual void* create()                                   = 0;
	// execution
	virtual bool  getMessage(int& rc, bool& done) const      = 0;
	virtual void  wait() const                               = 0;
	virtual void  resize() const                             = 0;
	virtual void  moveToForeground() const                   = 0;
    virtual void  messageBox(const wchar_t*) const           = 0;
	virtual int   time() const                               = 0;
    // termination
    virtual void  release()                                  = 0;
    virtual void  Delete() const                             = 0;
};

extern "C"
iWindow* CreateMainWindow(void*, int, iContext*);

extern "C"
iWindow* WindowAddress();

const wchar_t* itowc(wchar_t* s, int a);
#endif