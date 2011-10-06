/* Sound Implementation - Model Branch
 *
 * Sound.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCoordinator.h"     // for the Coordinator Interface
#include "iAPISound.h"        // for the APISound Interface
#include "iUtilities.h"       // for nameWithDir(), error()

#include "Sound.h"            // for Sound class definition
#include "MathDeclarations.h" // for Vector
#include "ModelSystem.h"      // for MODEL_Z_AXIS
#include "ModelSettings.h"    // for AUDIO_DIRECTORY

//------------------------------- Sound ------------------------------------
//
// The Sound class defines the structure of a sound in the model
//
// The Coordinator manages the Sound Instances
//
iCoordinator* Sound::coordinator = NULL;

// CreateSound creates a sound object associated with file f in directory
// AUDIO_DIRECTORY with the following properites: type t, continuous c, 
// on o, sound cone q in degrees
//
iSound* CreateSound(const wchar_t* f, SoundType t, bool c, bool o, 
 float q) {

	return new Sound(f, t, c, o, q);
}

iSound* Clone(const iSound* src) { 
    
    return src->clone();
}

// constructor adds the sound to the coordinator, creates the apiSound, stores 
// the name of the sound file relative to the execution directory and
// initializes the reference time
//
Sound::Sound(const wchar_t* f, SoundType t, bool c, bool o, float q) : 
 type(t), continuous(c), on(o), cone(q) {

    if (!coordinator)
        error(L"Sound::00 Couldn\'t access the Coordinator");
    else if(!coordinator->add(this))
        error(L"Sound::01 Couldn\'t add the sound to the Coordinator");

    // sound apiSound on the sound device
	apiSound = CreateAPISound();

	// store filename (without/with path)
	int len = strlen(f);
	relFile = new wchar_t[len + 1];
	strcpy(relFile, f, len);
	// add the directory to create the relative filename
	len += strlen(AUDIO_DIRECTORY) + 1;
	dirFile = new wchar_t[len + 1];
	nameWithDir(dirFile, AUDIO_DIRECTORY, f, len);

	// prepare to start playing
    setToStart = continuous && on;
    setToStop  = false;

	// initialize reference time
	lastToggle = 0;
}

// copy constructor initializes the instance variables and calls the
 // assignment operator
//
Sound::Sound(const Sound& src) {

    if (!coordinator)
        error(L"Sound::00 Couldn\'t access the Coordinator");
    else if(!coordinator->add(this))
        error(L"Sound::01 Couldn\'t add the sound to the Coordinator");

	apiSound = NULL;
	relFile  = NULL;
	dirFile  = NULL;
	*this    = src;
}

// assignment operator discard old sound apiSound and copies over
// data from src - does not copy over the APISound object from src
// but instead creates a new APISound object for the current sound
// object - does not copy over the Frame or the Text objects attached
// to the current object but instead initializes them to NULL
//
Sound& Sound::operator=(const Sound& src) {

	if (this != &src) {
        *((Frame*)this) = src;
		if (apiSound)
			apiSound->Delete();
		if (relFile)
			delete [] relFile;
		if (dirFile)
			delete [] dirFile;
        if (src.apiSound)
		    apiSound = src.apiSound->clone();
        else    
            apiSound = NULL;
		int len    = strlen(src.relFile);
		relFile    = new wchar_t[len + 1];
		strcpy(relFile, src.relFile, len);
		len        = strlen(src.dirFile);
		dirFile    = new wchar_t[len + 1];
		strcpy(dirFile, src.dirFile, len);
		type       = src.type == MOBILE_SOUND ? LOCAL_SOUND : src.type;
		on         = false;
		continuous = src.continuous;
		cone       = src.cone;
		setToStart = src.on;
		setToStop  = false;
		lastToggle = 0;
	}

	return *this;
}

// change changes the source file for the sound
//
void Sound::change(const wchar_t* f) {

	if (f) {
		//stop the current sound
		if (apiSound) {
			apiSound->stop();
			apiSound->release();
		}
		// store filename (without/with path)
		int len = strlen(f);
		if (relFile) 
			delete [] relFile;
		relFile = new wchar_t[len + 1];
		strcpy(relFile, f, len);
		// add the directory to create the relative filename
		if (dirFile) 
			delete [] dirFile;
		len += strlen(AUDIO_DIRECTORY) + 1;
		dirFile = new wchar_t[len + 1];
		nameWithDir(dirFile, AUDIO_DIRECTORY, f, len);
	}
}

// toggle toggles the sound if the latency period has elapsed
//
bool Sound::toggle(int now) {

	bool rc = false;

    if (now - lastToggle > KEY_LATENCY) {
        if (on)
            setToStop = true;
        else
            setToStart = true;
        lastToggle = now;
		rc = true;
    }

	return rc;
}

// plays the sound if the latency period has elapsed
//
bool Sound::play(int now) {

	bool rc = false;

    if (now - lastToggle > KEY_LATENCY) {
        setToStart = true;
        lastToggle = now;
		rc = true;
    }

	return rc;
}

// plays the sound if the latency period has elapsed
//
bool Sound::stop(int now) {

	bool rc = false;

    if (now - lastToggle > KEY_LATENCY) {
        setToStop = true;
        lastToggle = now;
		rc = true;
    }

	return rc;
}

// update updates the position and orientation of the Sound Instance
//
void Sound::update() const {

	if (apiSound)
		if (type == MOBILE_SOUND || type == LOCAL_SOUND) 
			apiSound->update(this);
}

// implements schedule changes to the sound apiSound
//
void Sound::implement(int now) {

	if (setToStop) {
		if (apiSound) 
            apiSound->stop();
		setToStop  = false;
		on         = false;
	}
	if (setToStart) {
		if (apiSound) 
			apiSound->play(this, type == GLOBAL_SOUND, continuous);
		setToStart = false;
		setToStop  = false;
		on         = true;
	}
}

// suspend stops the playing of the sound apiSound and prepares the sound
// to start playing upon restoration if the sound is continuous and set 
// to start or on
//
void Sound::suspend() {

	if (apiSound) 
		apiSound->stop();
	setToStart = continuous && (setToStart || on);
	on         = false;
}

// restore reinitializes the time of the last toggle
//
void Sound::restore(int now) {

	// reference time
	lastToggle = now;
}

// destructor deletes the sound apiSound and removes the sound
// from the coordinator
//
Sound::~Sound() {

	if (dirFile)
		delete [] dirFile;
	if (relFile)
		delete [] relFile;
    if (apiSound) 
		apiSound->Delete();
    if (coordinator)
        coordinator->remove(this);
    else
        error(L"Sound::90 Couldn\'t access the Coordinator");
}


