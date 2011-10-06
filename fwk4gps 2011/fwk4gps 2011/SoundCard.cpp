/* SoundCard Implementation - Direct API Branch
 *
 * SoundCard.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * contributions by: Jon Buckley '11
 * distributed under TPL - see ../Licenses.txt
 */

#include "iContext.h"        // for the Context Interface
#include "iUtilities.h"      // for error()

#include "SoundCard.h"       // for the SoundCard class definition
#include "APISound.h"        // for APISound::connectsThrough()
#include "MathDefinitions.h" // for Vector
#include "ModelSettings.h"	 // for DELTA_VOLUME, SOUND_DISTANCE_FACTOR

#include <cstring>			 // for memset()

//------------------------------- SoundCard -----------------------------------
//
// The SoundCard class manages the API Sound connectivity
//
// CreateSound creates the SoundCard object
//
iSoundCard* CreateSoundCard(iContext* c) {

	return new SoundCard(c);
}

// constructor initializes the instance pointers and COM, attaches the 
// SoundCard object to the APISound class, and sets up the sound card
//
SoundCard::SoundCard(iContext* c) : context(c) {

    pXAudio2        = NULL;
	pMasteringVoice = NULL;
	memset(&Listener, 0, sizeof(X3DAUDIO_LISTENER));

	// Multi-threaded audio coordinator
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // set up the sound card
    HRESULT hr;
	UINT32 flags = 0;

	// enable XAudio2 debugging if we're running in debug mode
	#ifdef _DEBUG
		flags |= XAUDIO2_DEBUG_ENGINE;
	#endif

	if (FAILED(hr = XAudio2Create(&pXAudio2, flags))) {
		error(L"SoundCard::01 Failed to initialize the XAudio2 engine");
		release();
	}
	else if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice))) {
		error(L"SoundCard::02 Failed to create the Mastering Voice");
		release();
	}
    else {
		// Initialize the X3DAudio engine
		XAUDIO2_DEVICE_DETAILS deviceDetails;
		pXAudio2->GetDeviceDetails(0, &deviceDetails);
		DWORD channelMask = deviceDetails.OutputFormat.dwChannelMask;

		X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, X3DInstance);
		
        // set the frequency and volume range of the context
		// http://msdn.microsoft.com/en-us/library/ee415828(v=VS.85).aspx
		context->set(GF_AU_FREQ, DEFAULT_FREQUENCY); 
        context->set(GF_AU_MXVL, MAX_VOLUME);
        context->set(GF_AU_MNVL, MIN_VOLUME);
		context->set(GF_AU_CVOL, DEFAULT_VOLUME);
    }

	// provides the API connectivity for the APISound objects
    APISound::masteringVoice(pMasteringVoice);
    APISound::soundEngine2D(pXAudio2);
    APISound::soundEngine3D(&X3DInstance);
    APISound::listener(&Listener);
}

// update updates the listener to hold the current viewpoint and orientation
//
void SoundCard::update() {

	Vector front    = context->get(GF_CA_HEAD);
	Vector position = context->get(GF_CA_POSN);
    Vector up       = context->get(GF_CA_UP  );

	X3DAUDIO_VECTOR eFront    = {front.x, front.y, front.z};
	X3DAUDIO_VECTOR ePosition = {position.x, position.y, position.z};
	X3DAUDIO_VECTOR eUp       = {up.x, up.y, up.z};
	
	Listener.OrientFront = eFront;
	Listener.Position    = ePosition;
	Listener.OrientTop   = eUp;

	APISound::setVolume(convertVolume(context->get(GF_AU_CVOL)));
	APISound::setFrequency(convertFrequency(context->get(GF_AU_FREQ)));

}

// release disengages the interfaces to the COM objects
//
void SoundCard::release() {

	if (pMasteringVoice) {
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = NULL;
	}

	if (pXAudio2) {
		pXAudio2->Release();
		pXAudio2 = NULL;
	}
}

// destructor releases the connection and uninitializes COM
//
SoundCard::~SoundCard() {

	release();

	CoUninitialize();
}

float SoundCard::convertFrequency(int freq) {
	float rv = 1.0;

	if (freq < DEFAULT_FREQUENCY) // 0-49
		rv = freq * 2.0f / 100; // 0-0.99
	else if (freq > DEFAULT_FREQUENCY) // 51-100
		rv = (freq / 50.0f - 1) * 1024; // 1-1024

	return rv;
}

float SoundCard::convertVolume(int volume) {
	float rv = 1.0;

	if (volume < DEFAULT_VOLUME) // 0-49
		rv = volume * 2.0f / 100; // 0-0.99
	else if (volume > DEFAULT_VOLUME) // 51-100
		rv = (volume / 50.0f - 1) * (XAUDIO2_MAX_VOLUME_LEVEL / 1000000); // 1-2^24

	return rv;
}
