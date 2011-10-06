#ifndef _I_API_SOUND_H_
#define _I_API_SOUND_H_

/* APISound Interface - Direct API Branch
 *
 * iAPISound.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- APISound ------------------------------------
//
// iAPISound is the Interface to the APISound class
//
class iSound;

class iAPISound {
  public:
    virtual iAPISound* clone() const                                = 0;
	// execution
    virtual void update(const iSound* sound)                        = 0;
    virtual void play(const iSound* sound, bool global, bool cont)  = 0;
    virtual void stop()                                             = 0;
	// termination
	virtual void release()                                          = 0;
	virtual void Delete() const                                     = 0;
};

iAPISound* CreateAPISound();

#endif
