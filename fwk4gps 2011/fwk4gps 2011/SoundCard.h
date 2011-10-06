#ifndef _SOUND_CARD_H_
#define _SOUND_CARD_H_

/* SoundCard Definition - Direct API Branch
 *
 * SoundCard.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski
 * contributions by: Jon Buckley '11
 * distributed under TPL - see ../Licenses.txt
 */

#include "DirectAPISettings.h" // for selected API headers
#include "iSoundCard.h"        // for the SoundCard Interface

//------------------------------- SoundCard -----------------------------------
//
// The SoundCard class provides the API connectivity to the sound card 
//
class SoundCard : public iSoundCard {

	iContext*               context;         // points to the Context object

    IXAudio2* 				pXAudio2;		 // interface to XAudio2 engine
	X3DAUDIO_HANDLE 		X3DInstance;	 // interface to X3DAudio engine
	X3DAUDIO_LISTENER 		Listener;		 // cameras's position, orientation
	IXAudio2MasteringVoice* pMasteringVoice; // interface to the MasteringVoice
                                             // default audio sink 4 all voices
    SoundCard(iContext*);
    SoundCard(const SoundCard& s);            // prevent copying
    SoundCard& operator=(const SoundCard& s); // prevent assignments
    void    release();
    virtual ~SoundCard();

	// Volume/Frequency conversion functions
	float convertFrequency(int freq);
	float convertVolume(int volume);

  public:
	friend iSoundCard* CreateSoundCard(iContext*);
	// execution function
    void   update();
	// termination
	void   Delete() const { delete this; }
};

#endif
