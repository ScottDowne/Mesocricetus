#ifndef _APISOUND_H_
#define _APISOUND_H_

/* APISound Definition - Direct API Branch
 *
 * APISound.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * contributions by: Jon Buckley '11
 * distributed under TPL - see ../Licenses.txt
 */

#include "DirectAPISettings.h" // for IXAudio2, ...
#include "iAPISound.h"        // for the APISound Interface

//-------------------------------- APISound -----------------------------------
//
// The APISound class implements a Sound at the API level 
//
class iSound;
class SoundCard;

class APISound : public iAPISound {

    static IXAudio2*               pXAudio2;		// XAudio2 engine
	static X3DAUDIO_HANDLE*        pX3DInstance;	// X3DAudio engine
	static X3DAUDIO_LISTENER*      pListener;		// camera's frame
	static IXAudio2MasteringVoice* pMasteringVoice; // masteringVoice
                                   // is the default audio sink for all voices
    static float volume;
    static float frequency;

    IXAudio2SourceVoice* pSourceVoice; // sound source
	BYTE* pDataBuffer;				   // Stores WAVE buffer
	UINT32* pDpdsBuffer;			   // Stores xWMA buffer
	X3DAUDIO_EMITTER Emitter;		   // Represents the frame in 3D space
	X3DAUDIO_DSP_SETTINGS DSPSettings; // Stores 3D audio settings
	X3DAUDIO_CONE soundCone;		   // Stores the sound cone settings
	float matrix [64];				   // Stores calculated speaker volumes

    APISound();
    virtual ~APISound();

	// RIFF File handling
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize,
     DWORD & dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, 
     DWORD bufferoffset);

  public:
	friend iAPISound* CreateAPISound();
    static void soundEngine2D(void* e)  { pXAudio2 = (IXAudio2*)e; }
    static void soundEngine3D(void* e)  { pX3DInstance = (X3DAUDIO_HANDLE*)e; }
    static void listener(void* l)       { pListener = (X3DAUDIO_LISTENER*)l; }
    static void masteringVoice(void* m) { pMasteringVoice = (IXAudio2MasteringVoice*)m; }
    static void setVolume(float v)      { volume = v; }
    static void setFrequency(float f)   { frequency = f; }
    APISound(const APISound& s);
    APISound& operator=(const APISound& s);
    iAPISound* clone() const { return new APISound(*this); }
	// initialization
    bool setup(const iSound* sound, bool global, bool continuous);
	// execution
    void update(const iSound* sound);
    void play(const iSound* sound, bool global, bool continuous);
    void stop();
	// termination
	void release();
	void Delete() const { delete this; }
};

#endif
