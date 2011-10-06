#ifndef _SOUND_H_
#define _SOUND_H_

/* Sound Definition - Model Branch
 *
 * Sound.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iSound.h" // for the Sound Interface

//------------------------------- Sound ---------------------------------------
//
// The Sound class defines the structure of a sound in the Model Branch
//
class iCoordinator;
class iAPISound;

class Sound : public iSound {

    static iCoordinator* coordinator; // points to the coordinator

    iAPISound*     apiSound;          // points to the sound at the api level
    wchar_t*       dirFile;           // name of the sound file with the path
	wchar_t*       relFile;           // name of the sound file without the path

    SoundType      type;              // type of sound - local or global
    float          cone;              // angle of sound cone in degrees [0,360]
    bool           on;                // is this sound on?
    bool           setToStart;        // is this sound ready to start playing?
    bool           setToStop;         // is this sound ready to stop playing?
    bool           continuous;        // is this sound continuous?

    int            lastToggle;        // time of the last toggle

    Sound(const wchar_t*, SoundType, bool, bool, float = 0);
	Sound(const Sound&);
    virtual ~Sound();

  public:
	friend iSound* CreateSound(const wchar_t*, SoundType, bool, bool, float);
	static void isCoordinatedBy(iCoordinator* c) { coordinator = c; }
	Sound& operator=(const Sound&);
    iSound* clone() const                        { return new Sound(*this); }
	// initialization
	const   wchar_t* dirFileName() const         { return dirFile; }
	const   wchar_t* relFileName() const         { return relFile; }
	float   coneAngle() const                    { return cone; }
	void    change(const wchar_t* f);
    void    loop(bool on)                        { this->on = on; }
	// execution
    bool    isOn() const                         { return on; }
	bool    toggle(int now);
	void    update() const;
	bool    play(int now);
	bool    stop(int now);
    void    implement(int now);
    // suspension
	void    suspend();
	void    restore(int now);
	// termination function
	void    Delete() const                       { delete this; }
};

#endif
