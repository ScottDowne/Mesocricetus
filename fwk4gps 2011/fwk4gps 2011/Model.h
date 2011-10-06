#ifndef _SCENE_H_
#define _SCENE_H_

/* Model Definition - Model Branch
 *
 * Model.h
 * fwk4gps version 2.0
 * gam666/dps901
 * Sept 2 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iModel.h" // for the Model Interface
#include <vector>
using std::vector;

//-------------------------------- Model --------------------------------------
//
// The Model class coordinates all of the elements of the model 
//
class Model : public iModel {

	static const int MAX_OBJECTS      = 50;
	static const int MAX_TEXTURES     = 50;
	static const int MAX_LIGHTS       = 8;
	static const int MAX_CAMERAS      = 8;
	static const int MAX_SOUNDS       = 20;
    static const int MAX_VERTEX_LISTS = 50;
	static const int MAX_TEXTS        = 50;

	iContext*    context;                            // points to Context object

	int          noObjects;                        // number of objects
	int          noTextures;                       // number of textures
	int          noLights;                         // number of lights
	int          noCameras;                        // number of cameras
    int          noSounds;                         // number of sound sources
    int          noPrimitiveSets;                    // number of vertex lists
    int          noTexts;                          // number of text items
    int          current;                          // index - current camera
    vector<iObject*> object;
    //iObject*     object[MAX_OBJECTS];              // points to objects
	iTexture*    texture[MAX_TEXTURES];            // points to textures
    iLight*      light[MAX_LIGHTS];                // points to light sources
    iCamera*     camera[MAX_CAMERAS];              // points to cameras
    iSound*      sound[MAX_SOUNDS];                // points to sound sources
    iPrimitiveSet* primitiveSet[MAX_VERTEX_LISTS]; // points to vertex lists
    iText*       text[MAX_TEXTS];                  // points to text items

    long         freq;                  // current ambient frequency
    long         maxVolume;             // maximum volume
    long         minVolume;             // minimum volume
    long         incVolume;             // volume increment
    long         curVolume;             // current volume

    int          lastCameraToggle;      // time of most recent toggle
	int			 lastVolumeChange;      // time of last volume change
    int          lastHUDToggle;         // time of most recent toggle
    int          lastUpdate;            // reference time
    int          lastReset;             // last time framecount was reset to 0
    int          framecount;            // number of frames since 'lastReset'
    int          fps;                   // frame rate per sec
    bool         on;                    // is the HUD being displayed?

	// relative coordinates [0,1] of HUD with respect to the client area
    Rect*        hudPos;
	iTexture*    hudTexture;            // points to the HUD texture

    Model(iContext* c);
    Model(const Model& s);              // prevents copying
    Model& operator=(const Model& s);   // prevents assignment
    virtual ~Model();
    void   validate();                  // validates HUD size & position
    void   attenuate(int factor);       // adjusts the volume
    bool   add(const void** v, const void* a, int& n, int max);
    bool   remove(const void** v, const void* r, int& n);

  public:
	friend iModel* CreateModel(iContext* c);
	// initialization
    bool   add(const iObject*);
    bool   add(const iTexture*);
    bool   add(const iLight*);
    bool   add(const iCamera*);
    bool   add(const iSound*);
    bool   add(const iPrimitiveSet*);
    bool   add(const iText* t); 
    void   reset(int now);
	// execution
    void   render(Category category);
	void   update(int now);
    const  Rect& hudRect() const          { return *hudPos; }
    bool   hudIsOn() const                { return on; }
    void   draw(unsigned flags) const;
    int    frameRate() const              { return fps; }
    const  iCamera* currentCamera() const { return camera[current]; }
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
    bool   remove(const iPrimitiveSet*);
    bool   remove(const iText* t); 
	void   Delete() const { delete this; }
};

#endif
