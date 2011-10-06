#ifndef _I_SOUND_CARD_H_
#define _I_SOUND_CARD_H_

/* SoundCard Interface - Direct API Branch
 *
 * iSoundCard.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//------------------------------- iSoundCard ----------------------------------
//
// iSoundCard is the Interface to the SoundCard class 
//
class iContext;

class iSoundCard {
  public:
	// execution
    virtual void update()                      = 0;
	// termination
	virtual void Delete() const                = 0;
};

iSoundCard* CreateSoundCard(iContext*);

#endif
