#ifndef _I_SOUND_H_
#define _I_SOUND_H_

/* Sound Interface - Model Branch
 *
 * iSound.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Frame.h" // for the Frame class definition

//-------------------------------- iSound -------------------------------------
//
// iSound is the Interface to the Sound class
//
enum  SoundType;

class iSound : public Frame {
  public:
    virtual iSound* clone() const                        = 0;
	// initialization
	virtual const   wchar_t* relFileName() const         = 0;
	virtual const   wchar_t* dirFileName() const         = 0;
	virtual float   coneAngle() const                    = 0;
	virtual void    change(const wchar_t* f)             = 0;
    virtual void    loop(bool on)                        = 0;
	// execution
    virtual bool    isOn() const                         = 0;
	virtual bool    toggle(int now)                      = 0;
	virtual void    update() const                       = 0;
	virtual bool    stop(int now)                        = 0;
	virtual bool    play(int now)                        = 0;
	virtual void    implement(int now)                   = 0;
    // suspension
	virtual void    suspend()                            = 0;
	virtual void    restore(int now)                     = 0;
	// termination
	virtual void    Delete() const                       = 0;
};

iSound* CreateSound(const wchar_t*, SoundType, bool = true, bool = true, 
 float = 0);

iSound* Clone(const iSound*);

#endif
