#ifndef _COORDINATOR_H_
#define _COORDINATOR_H_

/* Coordinator Definition - Model Branch
 *
 * Coordinator.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include <vector>
#include "iCoordinator.h" // for the Coordinator Interface

//-------------------------------- Coordinator --------------------------------------
//
// The Coordinator class coordinates all of the design elements 
//
class Coordinator : public iCoordinator {

	iContext*    context;               // points to Context object

    std::vector<iObject*>  object;      // points to objects
	std::vector<iTexture*> texture;     // points to textures
    std::vector<iLight*>   light;       // points to light sources
    std::vector<iCamera*>  camera;      // points to cameras
    std::vector<iSound*>   sound;       // points to sound sources
    std::vector<iGraphic*> graphic;     // points to graphics
    std::vector<iText*>    text;        // points to text items

    long         freq;                  // current ambient frequency
    long         maxVolume;             // maximum volume
    long         minVolume;             // minimum volume
    long         incVolume;             // volume increment
    long         curVolume;             // current volume

    int          current;               // index - current camera
    int          lastCameraToggle;      // time of most recent toggle
	int			 lastVolumeChange;      // time of last volume change
    int          lastHUDToggle;         // time of most recent toggle
    int          lastUpdate;            // reference time
    int          lastReset;             // last time framecount was reset to 0
    int          framecount;            // number of frames since 'lastReset'
    int          fps;                   // frame rate per sec
    bool         on;                    // is the HUD being displayed?

    float        hudPosX;               // position across
    float        hudPosY;               // position down
	iTexture*    hudTexture;            // points to the HUD texture

    Coordinator(iContext* c);
    Coordinator(const Coordinator& s);            // prevents copying
    Coordinator& operator=(const Coordinator& s); // prevents assignment
    virtual ~Coordinator();
    void   validate();                  // validates HUD size & position
    void   attenuate(int factor);       // adjusts the volume

  public:
	friend iCoordinator* CreateCoordinator(iContext* c);
	// initialization
    bool   add(const iObject*);
    bool   add(const iTexture*);
    bool   add(const iLight*);
    bool   add(const iCamera*);
    bool   add(const iSound*);
    bool   add(const iGraphic*);
    bool   add(const iText* t); 
    void   reset(int now);
	// execution
    void   render(Category category);
	void   update(int now);
    void   hudPos(float& x, float& y) const { x = hudPosX; y = hudPosY; }
    bool   hudIsOn() const                  { return on; }
    void   draw(unsigned flags) const;
    int    frameRate() const                { return fps; }
    const  iCamera* currentCamera() const   { return camera[current]; }
	// suspension
    void   suspend() const;
	void   restore(int now);
    // termination
    void   release() const;
    bool   remove(const iObject*);
    bool   remove(const iTexture*);
    bool   remove(const iLight*);
    bool   remove(const iCamera*);
    bool   remove(const iSound*);
    bool   remove(const iGraphic*);
    bool   remove(const iText* t); 
	void   Delete() const                   { delete this; }
};

#endif
