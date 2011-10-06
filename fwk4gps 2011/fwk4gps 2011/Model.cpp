/* Model Implementation - Model Branch
 *
 * Model.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * Sept 2 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iContext.h"        // for the Context Interface
#include "iUtilities.h"      // for error()

#include "Model.h"           // for the Model class definition
#include "Object.h"          // for Object::isCoordinatedBy
#include "Texture.h"         // for Texture::isCoordinatedBy
#include "Light.h"           // for Light::isCoordinatedBy
#include "Camera.h"          // for Camera::isCoordinatedBy
#include "Sound.h"           // for Sound::isCoordinatedBy
#include "PrimitiveSet.h"    // for PrimitiveSet::isCoordinatedBy
#include "Text.h"            // for Text::isCoordinatedBy
#include "ModelSettings.h"   // for model macros

//-------------------------------- Model --------------------------------------
//
// The Model object manages the objects, textures, lights, sounds, vertex 
// lists, and cameras in the model
//
// CreateModel creates the Model Object
//
iModel* CreateModel(iContext* c) {

	return new Model(c);
}

// constructor initializes the object and texture pointers, the reference
// time and attaches the Model class to the Object and Texture classes
//
Model::Model(iContext* c) : context(c) {

	// manages the objects, textures, lights, cameras, sounds
	Object::isCoordinatedBy(this);
	Texture::isCoordinatedBy(this);
	Light::isCoordinatedBy(this);
	Camera::isCoordinatedBy(this);
	Sound::isCoordinatedBy(this); 
	PrimitiveSet::isCoordinatedBy(this); 
	Text::isCoordinatedBy(this);

    // initialize pointers to model elements
    noObjects = 0;
    //for (int i = 0; i < MAX_OBJECTS; i++)
    //    object[i] = 0;
	noTextures = 0;
    for (int i = 0; i < MAX_TEXTURES; i++)
        texture[i] = 0;
    noLights = 0;
	for (int i = 0; i < MAX_LIGHTS; i++)
        light[i] = 0;
    noCameras = 0;
	for (int i = 0; i < MAX_CAMERAS; i++)
        camera[i] = 0;
    noSounds = 0;
    for (int i = 0; i < MAX_SOUNDS; i++)
        sound[i] = NULL;
    noPrimitiveSets = 0;
    for (int i = 0; i < MAX_VERTEX_LISTS; i++)
        primitiveSet[i] = NULL;
	noTexts = 0;
    for (int i = 0; i < MAX_TEXTS; i++)
        text[i] = 0;

    // model timers
    lastCameraToggle = 0;
    lastHUDToggle    = 0;
    lastUpdate       = 0;
    lastReset        = 0;
    framecount       = 0;
    fps              = 0;

    // current camera
    current = 0;

    // volume and frequency settings
    freq             = DEFAULT_FREQUENCY;
    maxVolume        = context->get(GF_AU_MXVL);
    minVolume        = context->get(GF_AU_MNVL);
    incVolume        = STEP_VOLUME;
    curVolume        = context->get(GF_AU_CVOL);
	lastVolumeChange = 0;

    // HUD size and position
    hudPos      = new Rect();
	on          = false;
	hudPos->tlx = HUD_X;
	hudPos->tly = HUD_Y;
	hudPos->rw  = HUD_W;
	hudPos->rh  = HUD_H;
	validate();
}

// validate keeps the HUD within the client area
//
void Model::validate() {

	if (hudPos->tlx < TL_MIN) 
		hudPos->tlx = TL_MIN;
    else if (hudPos->tlx > TL_MAX) 
		hudPos->tlx = TL_MAX;
    if (hudPos->tly < TL_MIN) 
		hudPos->tly = TL_MIN;
    else if (hudPos->tly > TL_MAX) 
		hudPos->tly = TL_MAX;
    if (hudPos->rw < R_MIN) 
		hudPos->rw = R_MIN;
    else if (hudPos->rw + hudPos->tlx > TL_MAX && hudPos->rw < TL_MAX) 
		hudPos->tlx = TL_MAX - hudPos->rw;
    else if (hudPos->rw + hudPos->tlx > TL_MAX) 
		hudPos->rw = TL_MAX - hudPos->tlx;
    if (hudPos->rh < R_MIN) 
		hudPos->rh = R_MIN;
    else if (hudPos->rh + hudPos->tly > TL_MAX && hudPos->rh < TL_MAX) 
		hudPos->tly = TL_MAX - hudPos->rh;
    else if (hudPos->rh + hudPos->tly > TL_MAX) 
		hudPos->rh = TL_MAX - hudPos->tly;
}

// add adds a pointer to void* v to the model
// and returns true if successful, false otherwise
//
bool Model::add(const void** v, const void* a, int& n, int max) {

    int i;
    bool rc = false;

    for (i = 0; i < n; i++)
        if (!v[i]) {
            v[i] = a;
            i = n + 1;
            rc = true;
        }
    if (i == n && n < max) {
        v[n++] = a;
        rc = true;
    }

    return rc;
}

// add adds a pointer to Object *o to the model
//
bool Model::add(const iObject* o) {
    unsigned i;
    bool rc = false;

    for (i = 0; i < object.size() && !rc; i++)
        if (!object[i]) {
            object[i] = (iObject*)o;
            rc = true;
        }
    if (!rc)
        object.push_back((iObject*)o);
    noObjects = object.size();
    return true;
}

// add adds a pointer to iTexture *t to the model
//
bool Model::add(const iTexture* t) {
    return add((const void**)texture, t, noTextures, MAX_TEXTURES);
}

// add adds a pointer to Light *l to the model
//
bool Model::add(const iLight* l) {
    return add((const void**)light, l, noLights, MAX_LIGHTS);
}

// add adds a pointer to Camera *c to the model
//
bool Model::add(const iCamera* c) {
    return add((const void**)camera, c, noCameras, MAX_CAMERAS);
}

// add adds a pointer to Sound *s to the model
//
bool Model::add(const iSound* s) {
    return add((const void**)sound, s, noSounds, MAX_SOUNDS);
}

// add adds a pointer to primitiveSet *v to the model
//
bool Model::add(const iPrimitiveSet* v) {
    return add((const void**)primitiveSet, v, noPrimitiveSets, MAX_VERTEX_LISTS);
}

// add adds a pointer to Text *t to the model
//
bool Model::add(const iText* t) {
    return add((const void**)text, t, noTexts, MAX_TEXTS);
}

// reset resets the model timers
//
void Model::reset(int now) {

    framecount       = 0;
    fps              = 0;
    lastReset        = now;
    lastUpdate       = now;
    lastCameraToggle = now;
    lastHUDToggle    = now;
}

// attenuate attenuates the volume of the ambient sounds by factor *
// incVolume - positive factor increases the volume, negative factor
// decreases the volume
//
void Model::attenuate(int factor) {

    curVolume = curVolume + (factor * incVolume);
    if (curVolume > maxVolume)
        curVolume = maxVolume;
    else if (curVolume < minVolume)
        curVolume = minVolume;
    context->set(GF_AU_CVOL, curVolume);
}

// update updates the Light, Camera, and Sound objects 
//
void Model::update(int now) {

	int delta = now - lastUpdate;

    // update the framecount/frame rate
	if (now - lastReset <= UNITS_PER_SEC) 
		framecount++;
	else {
        // recalculate the frame rate
        fps        = (framecount * UNITS_PER_SEC) / (now - lastReset);
		framecount = 0;
		lastReset  = now;
	}

	// toggle and update the current camera
	if (context->pressed(CAMERA_SELECT) && now - lastCameraToggle > KEY_LATENCY) {
        lastCameraToggle = now;
        current++;
        if (current == noCameras)
            current = 0;
    }
    if (camera[current])
        camera[current]->update(delta);

    // toggle and update the sounds
    // update the volume and the frequency
    long dz = 0;
    int iy = context->get(GF_CT_POSY);
    if (iy)
        dz = (long)(iy * CTR_DISPLACEMENT_FACTOR);

	if (now - lastVolumeChange > KEY_LATENCY) {

		if (context->pressed(AUD_VOLUME_DEC))
			attenuate(-(now - lastVolumeChange));
		if (context->pressed(AUD_VOLUME_INC))
			attenuate(now - lastVolumeChange);
		
        lastVolumeChange = now;

		// drop frequency if AUD_SLOW_DOWN
        if (context->pressed(AUD_SLOW_DOWN)) {
            int newfreq = freq - STEP_FREQUENCY;
            freq = newfreq < MIN_FREQUENCY ? MIN_FREQUENCY : newfreq;
            context->set(GF_AU_FREQ, freq);
        }
        // increase frequency if AUD_SPEED_UP
        else if (context->pressed(AUD_SPEED_UP)) {
            int newfreq = freq + STEP_FREQUENCY;
            freq = newfreq < MAX_FREQUENCY ? MAX_FREQUENCY : newfreq;
            context->set(GF_AU_FREQ, freq);
        }
	}	
    // toggle and update the sound sources
    for (int i = 0; i < noSounds; i++)
		if (sound[i]) 
			sound[i]->update();

	// toggle and update the light sources
    for (int i = 0; i < noLights; i++)
		if (light[i])
			light[i]->update();

	// toggle and update the heads up display
	if (context->pressed(HUD_DISPLAY) && now - lastHUDToggle > KEY_LATENCY) {
        lastHUDToggle = now;
        on            = !on;
    }
	// update the HUD only if it is on
	if (on) {
        // translate the HUD
	    int dx = 0, dy = 0;
		if (context->pressed(HUD_RIGHT))
			dx += delta;
		if (context->pressed(HUD_LEFT))
			dx -= delta;
		if (context->pressed(HUD_UP))
			dy -= delta;
		if (context->pressed(HUD_DOWN))
			dy += delta;
		hudPos->tlx += dx * HUD_SPEED;
		hudPos->tly += dy * HUD_SPEED;
		validate();
	}

    // update the reference time
    lastUpdate = now;
}

// render draws the model elements for the specified Category
//
void Model::render(Category category) {

    switch (category) {
        case ALL_OBJECTS:
            // draw all objects
            for (int i = 0; i < noObjects; i++) {
		        if (object[i])
			        object[i]->draw();
            }
            break;
        case SOUND:
            for (int i = 0; i < noSounds; i++)
		        if (sound[i]) 
			        sound[i]->implement(lastUpdate);
            break;
        default:
            for (int i = 0; i < noObjects; i++) {
		        if (object[i] && object[i]->belongsTo(category))
			        object[i]->draw();
            }
    }
}

// suspend suspends the textures, lights, sounds, primitive sets, and text
// items in the model
//
void Model::suspend() const {

	for (int i = 0; i < noTextures; i++)
		if (texture[i])
			texture[i]->suspend();

	for (int i = 0; i < noLights; i++)
		if (light[i])
			light[i]->suspend();

    for (int i = 0; i < noSounds; i++)
		if (sound[i]) 
			sound[i]->suspend();

    for (int i = 0; i < noPrimitiveSets; i++)
		if (primitiveSet[i]) 
			primitiveSet[i]->suspend();

    for (int i = 0; i < noTexts; i++)
        if (text[i])
			text[i]->suspend();
}

// restore restores each light, sound, and text item and initializes
// the model timers
//
void Model::restore(int now) {

	for (int i = 0; i < noLights; i++)
		if (light[i])
			light[i]->restore(now);

	for (int i = 0; i < noSounds; i++) {
		if (sound[i]) 
			sound[i]->restore(now);
	}
    for (int i = 0; i < noTexts; i++)
        if (text[i])
			text[i]->restore();

    framecount       = 0;
    lastReset        = now;
    lastUpdate       = now;
    lastCameraToggle = now;
    lastHUDToggle    = now;
}

// release releases the textures, vertex lists, and text items in the
// model
//
void Model::release() const {

	for (int i = 0; i < noTextures; i++)
		if (texture[i])
			texture[i]->release();

    for (int i = 0; i < noPrimitiveSets; i++)
		if (primitiveSet[i]) 
			primitiveSet[i]->release();

    for (int i = 0; i < noTexts; i++)
        if (text[i])
			text[i]->release();
}

// remove removes the pointer to void* r from the model and
// returns true if successful, false otherwise
//
bool Model::remove(const void** v, const void* r, int& n) {

    bool rc = false;

    for (int i = 0; i < n; i++)
        if (v[i] == r) {
            v[i] = 0;
            rc = true;
        }
    while (n && !v[n - 1])
        n--;

    return rc;
}

// remove removes the pointer to iObject *o from the model
//
bool Model::remove(const iObject* r) {
    bool rc = false;

    for (int i = 0; i < noObjects; i++)
        if (object[i] == r) {
            object[i] = 0;
            rc = true;
        }
    while (object.size() && !object[object.size() - 1])
        object.pop_back();

    return rc;
}

// remove removes the pointer to iTexture *t from the model
//
bool Model::remove(const iTexture* t) {
    return remove((const void**)texture, t, noTextures);
}

// remove removes the pointer to iLight *l from the model
//
bool Model::remove(const iLight* l) {
    return remove((const void**)light, l, noLights);
}

// remove removes the pointer to iCamera *l from the model
//
bool Model::remove(const iCamera* c) {
    return remove((const void**)camera, c, noCameras);
}

// remove removes the pointer to iObject *o from the model
//
bool Model::remove(const iSound* s) {
    return remove((const void**)sound, s, noSounds);
}

// remove removes the pointer to iPrimitiveSet *v from the model
//
bool Model::remove(const iPrimitiveSet* v) {
    return remove((const void**)primitiveSet, v, noPrimitiveSets);
}

// remove removes the pointer to Text *t from the model
//
bool Model::remove(const iText* t) {
    return remove((const void**)text, t, noTexts);
}

// destructor deletes all of the model elements
//
Model::~Model() {

    for (int i = 0; i < noObjects; i++)
        if (object[i]) 
            object[i]->Delete();

    for (int i = 0; i < noTextures; i++)
        if (texture[i]) 
            texture[i]->Delete();

    for (int i = 0; i < noLights; i++)
        if (light[i])
            light[i]->Delete();

    for (int i = 0; i < noCameras; i++)
        if (camera[i])
            camera[i]->Delete();

    for (int i = 0; i < noSounds; i++)
        if (sound[i])
            sound[i]->Delete();

    for (int i = 0; i < noPrimitiveSets; i++)
        if (primitiveSet[i])
            primitiveSet[i]->Delete();

    for (int i = 0; i < noTexts; i++)
        if (text[i]) 
			text[i]->Delete();

    delete hudPos;
}
