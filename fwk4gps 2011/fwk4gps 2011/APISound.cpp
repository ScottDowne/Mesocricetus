/* APISound Implementation - Direct API Branch
 *
 * APISound.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * contributions by: Jon Buckley '11
 * distributed under TPL - see ../Licenses.txt
 */

#include "iContext.h"		 // for the Context Interface
#include "iSound.h"          // for the Sound Interface
#include "iUtilities.h"      // for error()

#include "APISound.h"        // for the APISound class definition
#include "MathDefinitions.h" // for M_PI
#include "Configuration.h"   // for INTERIOR_CONE, ATTENUATION_DISTANCE_FACTOR

/* WAVE/xWMA files are classified as 'RIFF' files by Microsoft. They share
 * the same basic file format. Read about it here:
 * http://msdn.microsoft.com/en-us/library/ee415713(v=VS.85).aspx
 * Windows running on x86/x64 is little-endian, which is why the FOURCC
 * identifiers are reversed
 */
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT  ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

//-------------------------------- APISound ------------------------------------
//
// The APISound class implements a Sound at the API level
//
// The API connectivity is defined by the SoundCard object
//
IXAudio2* 				APISound::pXAudio2        = NULL; // XAudio2 engine
X3DAUDIO_HANDLE* 		APISound::pX3DInstance    = NULL; // X3DAudio engine
X3DAUDIO_LISTENER* 		APISound::pListener       = NULL; // cameras's frame
IXAudio2MasteringVoice* APISound::pMasteringVoice = NULL; // mastering voice
                                     // is default audio sink for all voices
float APISound::volume    = 0;
float APISound::frequency = 0;

// CreateAPISound creates a APISound object
//
iAPISound* CreateAPISound() {

	return new APISound();
}

// constructor initializes the instance pointers
//
APISound::APISound() {

    pSourceVoice = NULL;
	pDataBuffer  = NULL;
	pDpdsBuffer  = NULL;
}

// copy constructor initializes the instance pointers
//
APISound::APISound(const APISound& src) {

    pSourceVoice = NULL;
	pDataBuffer  = NULL;
	pDpdsBuffer  = NULL;
    *this = src;
}

// assignment operator releases old interfaces and initializes
// new ones
//
APISound& APISound::operator=(const APISound& src) {

    if (this != &src) {
        release();
		pSourceVoice = NULL;
		pDataBuffer  = NULL;
		pDpdsBuffer  = NULL;
    }

    return *this;
}

// setup creates the sound segment from the sound file and in the
// case of a non-global sound creates a 3d audio path and extracts 
// the 3d buffer from that path 
//
bool APISound::setup(const iSound* sound, bool global, bool continuous) {

    bool rc = false;
	HANDLE file;
	DWORD chunkSize = 0, chunkDataPosition = 0, fileType = 0;

	WAVEFORMATEXTENSIBLE wfx = {0};
	XAUDIO2_BUFFER buffer = {0};
	XAUDIO2_BUFFER_WMA wmaBuffer = {0};

	file = CreateFile(sound->dirFileName(), GENERIC_READ, FILE_SHARE_READ, 
     NULL, OPEN_EXISTING, 0, NULL);
	if (file == INVALID_HANDLE_VALUE)
		error(L"APISound::10 Failed to open audio file");
	else if (FAILED(FindChunk(file, fourccRIFF, chunkSize, chunkDataPosition)))
		error(L"APISound::11 Failed to find RIFF segment");
	else if (FAILED(ReadChunkData(file, &fileType, sizeof(DWORD), 
     chunkDataPosition)))
		error(L"APISound::12 Failed to read RIFF segment");
	else if ( fileType != fourccWAVE && fileType != fourccXWMA )
		error(L"APISound::13 File is not a valid WAVE or XWMA file");
    else {
		// No more file-related error handling from this point forward
		// Read in WAVEFORMATEXTENSIBLE from 'fmt ' chunk
		FindChunk(file, fourccFMT, chunkSize, chunkDataPosition);
		ReadChunkData(file, &wfx, chunkSize, chunkDataPosition);

		// Fill out audio data buffer with contents of the fourccDATA chunk
		FindChunk(file, fourccDATA, chunkSize, chunkDataPosition);
		pDataBuffer = new BYTE[chunkSize];
		ReadChunkData(file, pDataBuffer, chunkSize, chunkDataPosition);

		// Populate XAUDIO2_BUFFER, set looping here
		buffer.AudioBytes = chunkSize;
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.pAudioData = pDataBuffer;
		buffer.PlayBegin = 0;
		buffer.PlayLength = 0;
		buffer.LoopBegin = 0;
		buffer.LoopLength = 0;
		buffer.LoopCount = continuous ? XAUDIO2_LOOP_INFINITE : 0;

		// If the file is a XWMA file, then load in the additional buffer
		if (fileType == fourccXWMA) {
			FindChunk(file, fourccDPDS, chunkSize, chunkDataPosition);
			// Divide by 4 to get a DWORD packet count 
            // http://forums.create.msdn.com/forums/t/11568.aspx (Dugan)
			wmaBuffer.PacketCount = chunkSize / 4;

			pDpdsBuffer = new UINT32[chunkSize];
			ReadChunkData(file, pDpdsBuffer, chunkSize, chunkDataPosition);
			wmaBuffer.pDecodedPacketCumulativeBytes = pDpdsBuffer;
		}

		if(FAILED(pXAudio2->CreateSourceVoice( &pSourceVoice, 
         (WAVEFORMATEX*)&wfx, XAUDIO2_VOICE_USEFILTER , 
         XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL)))
			error(L"APISound::14 Failed to create Source Voice");
		else if (fileType == fourccXWMA) {
			if(FAILED(pSourceVoice->SubmitSourceBuffer(&buffer, &wmaBuffer)))
				error(L"APISound::15 Failed to submit XWMA Source Buffer");
		}
		else {
			if(FAILED(pSourceVoice->SubmitSourceBuffer(&buffer)))
				error(L"APISound::16 Failed to submit WAVE Source Buffer ");
            else
                rc = true;
		}
		
		// Setup X3DAUDIO_DSP_SETTINGS and X3DAUDIO_EMITTER if this is not a global sound
		// http://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.x3daudio.x3daudio_dsp_settings(v=VS.85).aspx
		// http://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.x3daudio.x3daudio_emitter(v=VS.85).aspx
		if (!global) {
			XAUDIO2_DEVICE_DETAILS deviceDetails;
			pXAudio2->GetDeviceDetails(0,&deviceDetails);

			// Clear out the structs
			X3DAUDIO_EMITTER      clear2 = {0};
			X3DAUDIO_DSP_SETTINGS clear3 = {0};
			X3DAUDIO_CONE         clear4 = {0};

			for (int i = 0; i < 64; i++)
				matrix[i] = 0;
			Emitter      = clear2;
			DSPSettings = clear3;

			DSPSettings.SrcChannelCount = wfx.Format.nChannels;
			DSPSettings.DstChannelCount = deviceDetails.OutputFormat.Format.nChannels;
			DSPSettings.pMatrixCoefficients = matrix;

			Emitter.ChannelCount = 1;
            // No idea if this is the right setting
			Emitter.CurveDistanceScaler = ATTENUATION_DISTANCE_FACTOR; 
            // If the sound is omni-directional, we don't need a sound cone
			if (sound->coneAngle() < 360) { 
				soundCone = clear4;
				soundCone.OuterAngle = float(sound->coneAngle() * M_PI / 180);
				soundCone.OuterVolume = 0.0f; // Silent outside the Outer Cone
				soundCone.InnerAngle = soundCone.OuterAngle * INTERIOR_CONE;
				soundCone.InnerVolume = 1.0f; // Volume doesn't change within Inner Cone
				Emitter.pCone = &soundCone;
			}
            update(sound);
		}
	}
	
	// Close the file if we have a valid handle to it
	if (file != INVALID_HANDLE_VALUE)
		CloseHandle(file);

    return rc;
}

// update updates the position and the orientation of the api sound
//
void APISound::update(const iSound* sound) {

    if (pSourceVoice) {
		Vector front = sound->orientation('z');
		X3DAUDIO_VECTOR eFront = {front.x, front.y, front.z};

		Vector position = sound->position();
		X3DAUDIO_VECTOR ePosition = {position.x, position.y, position.z};
		
		Emitter.OrientFront = eFront;
		Emitter.Position    = ePosition;
		// What about Velocity (Doppler) and OrientTop (apparently only for multi-channel audio?)

		// X3DAudioCalculate() http://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.x3daudio.x3daudiocalculate(v=VS.85).aspx
		X3DAudioCalculate(*pX3DInstance, pListener, &Emitter, X3DAUDIO_CALCULATE_MATRIX,
			&DSPSettings );

		pSourceVoice->SetVolume(volume);
		pSourceVoice->SetFrequencyRatio(frequency);
		pSourceVoice->SetOutputMatrix(pMasteringVoice, DSPSettings.SrcChannelCount, 
         DSPSettings.DstChannelCount, DSPSettings.pMatrixCoefficients);
	}
}

// play starts playing the voice, sets it up if necessary
//
void APISound::play(const iSound* sound, bool global, bool continuous) {

    // create the voice if it doesn't yet exist
    if (!pSourceVoice) 
        setup(sound, global, continuous);

	if (pSourceVoice)
		pSourceVoice->Start(NULL);
}

// stop stops playing the voice
//
void APISound::stop() {

	if(pSourceVoice)
		pSourceVoice->Stop(NULL);
}

// release stops and destroys the voice, deletes the buffers
//
void APISound::release() {

    if (pSourceVoice) {
		pSourceVoice->Stop(NULL);
		pSourceVoice->DestroyVoice();
		pSourceVoice = NULL;
	}

	if (pDataBuffer) {
		delete [] pDataBuffer;
		pDataBuffer = NULL;
	}

	if (pDpdsBuffer) {
		delete [] pDpdsBuffer;
		pDpdsBuffer = NULL;
	}
}

// destructor releases the voice
//
APISound::~APISound() {

    release();
}

// FindChunk 
//
HRESULT APISound::FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition) {

    HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if( 0 == ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        if( 0 == ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if( 0 == ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL ) )
                hr = HRESULT_FROM_WIN32( GetLastError() );
            break;

        default:
            if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, dwChunkDataSize, NULL, FILE_CURRENT ) )
            return HRESULT_FROM_WIN32( GetLastError() );            
        }

        dwOffset += sizeof(DWORD) * 2;
        
        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        
        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    
    return S_OK;
    
}

// ReadChunkData
//
HRESULT APISound::ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferoffset, NULL, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );
    DWORD dwRead;
    if( 0 == ReadFile( hFile, buffer, buffersize, &dwRead, NULL ) )
        hr = HRESULT_FROM_WIN32( GetLastError() );
    return hr;
}
