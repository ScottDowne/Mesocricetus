/* Coordinator Implementation - Model Branch
 *
 * Coordinator.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iContext.h"      // for the Context Interface
#include "iUtilities.h"    // for error()

#include "Coordinator.h"   // for the Coordinator class definition
#include "Object.h"        // for Object::isCoordinatedBy
#include "Texture.h"       // for Texture::isCoordinatedBy
#include "Light.h"         // for Light::isCoordinatedBy
#include "Camera.h"        // for Camera::isCoordinatedBy
#include "Sound.h"         // for Sound::isCoordinatedBy
#include "Graphic.h"       // for Graphic::isCoordinatedBy
#include "Text.h"          // for Text::isCoordinatedBy
#include "ModelSettings.h" // for coordinator macros
#include "MathDefinitions.h" // for matrix operations
#include "DirectAPISettings.h" // for graphics api headers

//-------------------------------- Coordinator --------------------------------------
//
// The Coordinator object manages the objects, textures, lights, sounds, vertex 
// lists, and cameras in the model
//
// CreateCoordinator creates the Coordinator Object
//
iCoordinator* CreateCoordinator(iContext* c) {

	return new Coordinator(c);
}

// constructor initializes the object and texture pointers, the reference
// time and attaches the Coordinator class to the Object and Texture classes
//
Coordinator::Coordinator(iContext* c) : context(c) {

	// manages the objects, textures, lights, cameras, sounds
	Object::isCoordinatedBy(this);
	Texture::isCoordinatedBy(this);
	Light::isCoordinatedBy(this);
	Camera::isCoordinatedBy(this);
	Sound::isCoordinatedBy(this); 
	Graphic::isCoordinatedBy(this); 
	Text::isCoordinatedBy(this);

    // coordinator timers
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
	on          = true;
	hudPosX     = HUD_X;
	hudPosY     = HUD_Y;
	validate();
}

// validate keeps the HUD within the client area
//
void Coordinator::validate() {

	if (hudPosX < TL_MIN) 
		hudPosX = TL_MIN;
    else if (hudPosX + HUD_W > TL_MAX) 
		hudPosX = TL_MAX - HUD_W;
    if (hudPosY < TL_MIN) 
		hudPosY = TL_MIN;
    else if (hudPosY + HUD_H > TL_MAX) 
		hudPosY = TL_MAX - HUD_H;
}

// add adds a pointer to Object *o to the coordinator
//
bool Coordinator::add(const iObject* o) {
    unsigned i;
    bool rc = false;

    for (i = 0; i < object.size() && !rc; i++)
        if (!object[i]) {
            object[i] = (iObject*)o;
            rc = true;
        }
    if (!rc)
        object.push_back((iObject*)o);
    return true;
}

// add adds a pointer to iTexture *t to the coordinator
//
bool Coordinator::add(const iTexture* t) {
    unsigned i;
    bool rc = false;

    for (i = 0; i < texture.size() && !rc; i++)
        if (!texture[i]) {
            texture[i] = (iTexture*)t;
            rc = true;
        }
    if (!rc)
        texture.push_back((iTexture*)t);
    return true;
}

// add adds a pointer to Light *l to the coordinator
//
bool Coordinator::add(const iLight* l) {
    unsigned i;
    bool rc = false;

    for (i = 0; i < light.size() && !rc; i++)
        if (!light[i]) {
            light[i] = (iLight*)l;
            rc = true;
        }
    if (!rc)
        light.push_back((iLight*)l);
    return true;
}

// add adds a pointer to Camera *c to the coordinator
//
bool Coordinator::add(const iCamera* c) {
    unsigned i;
    bool rc = false;

    for (i = 0; i < camera.size() && !rc; i++)
        if (!camera[i]) {
            camera[i] = (iCamera*)c;
            rc = true;
        }
    if (!rc)
        camera.push_back((iCamera*)c);
    return true;
}


// add adds a pointer to Sound *s to the coordinator
//
bool Coordinator::add(const iSound* s) {
    unsigned i;
    bool rc = false;

    for (i = 0; i < sound.size() && !rc; i++)
        if (!sound[i]) {
            sound[i] = (iSound*)s;
            rc = true;
        }
    if (!rc)
        sound.push_back((iSound*)s);
    return true;
}

// add adds a pointer to graphic *v to the coordinator
//
bool Coordinator::add(const iGraphic* g) {
    unsigned i;
    bool rc = false;

    for (i = 0; i < graphic.size() && !rc; i++)
        if (!graphic[i]) {
            graphic[i] = (iGraphic*)g;
            rc = true;
        }
    if (!rc)
        graphic.push_back((iGraphic*)g);
    return true;
}

// add adds a pointer to Text *t to the coordinator
//
bool Coordinator::add(const iText* t) {
    unsigned i;
    bool rc = false;

    for (i = 0; i < text.size() && !rc; i++)
        if (!text[i]) {
            text[i] = (iText*)t;
            rc = true;
        }
    if (!rc)
        text.push_back((iText*)t);
     return true;
}

// reset resets the coordinator timers
//
void Coordinator::reset(int now) {

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
void Coordinator::attenuate(int factor) {

    curVolume = curVolume + (factor * incVolume);
    if (curVolume > maxVolume)
        curVolume = maxVolume;
    else if (curVolume < minVolume)
        curVolume = minVolume;
    context->set(GF_AU_CVOL, curVolume);
}

// update updates the Light, Camera, and Sound objects 
//
void Coordinator::update(int now) {

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
        if (current == camera.size())
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
    for (unsigned i = 0; i < sound.size(); i++)
		if (sound[i]) 
			sound[i]->update();

	// toggle and update the light sources
    for (unsigned i = 0; i < light.size(); i++)
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
		hudPosX += dx * HUD_SPEED;
		hudPosY += dy * HUD_SPEED;
		validate();
	}

    // update the reference time
    lastUpdate = now;
}

// render draws the coordinator elements for the specified Category
//
void Coordinator::render(Category category) {
    Plane frustum [6];

    Matrix projection = ::projection(context->get(GF_FR_FOV), context->get(GF_FR_ASP), context->get(GF_FR_NEAR), context->get(GF_FR_FAR));
    Matrix view = ::view(context->get(GF_CA_POSN), context->get(GF_CA_POSN) + context->get(GF_CA_HEAD), context->get(GF_CA_UP));
     
    Matrix viewProjection = view * projection;
    
    switch (category) {
        case ALL_OBJECTS:
            // draw all objects
            for (unsigned i = 0; i < object.size(); i++) {
		        if (object[i])
			        object[i]->draw();
            }

            break;

        case SOUND:
            for (unsigned i = 0; i < sound.size(); i++)
		        if (sound[i]) 
			        sound[i]->implement(lastUpdate);

            break;
        
        case TRANSLUCENT_OBJECT:
        case OPAQUE_OBJECT:
            // Left plane
            frustum[0].n = Vector (viewProjection.m14 + viewProjection.m11, viewProjection.m24 + viewProjection.m21, viewProjection.m34 + viewProjection.m31);
            frustum[0].d = viewProjection.m44 + viewProjection.m41;
 
            // Right plane
            frustum[1].n = Vector (viewProjection.m14 - viewProjection.m11, viewProjection.m24 - viewProjection.m21, viewProjection.m34 - viewProjection.m31);
            frustum[1].d = viewProjection.m44 - viewProjection.m41;
 
            // Top plane
            frustum[2].n = Vector (viewProjection.m14 - viewProjection.m12, viewProjection.m24 - viewProjection.m22, viewProjection.m34 - viewProjection.m32);
            frustum[2].d = viewProjection.m44 - viewProjection.m42;
 
            // Bottom plane
            frustum[3].n = Vector (viewProjection.m14 + viewProjection.m12, viewProjection.m24 + viewProjection.m22, viewProjection.m34 + viewProjection.m32);
            frustum[3].d = viewProjection.m44 + viewProjection.m42;
 
            // Near plane
            frustum[4].n = Vector (viewProjection.m13, viewProjection.m23, viewProjection.m33);
            frustum[4].d = viewProjection.m43;
 
            // Far plane
            frustum[5].n = Vector (viewProjection.m14 - viewProjection.m13, viewProjection.m24 - viewProjection.m23, viewProjection.m34 - viewProjection.m33);
            frustum[5].d = viewProjection.m44 - viewProjection.m43;
 
            // Normalize planes
            for ( int i = 0; i < 6; i++ )
            {
               normalize(frustum[i]);
            }

            for (unsigned i = 0; i < object.size(); i++)
            {
               if (object[i] && object[i]->belongsTo(category))
               {
                  bool inside = TRUE;
                  
                  for ( int j = 0; j < 6; j++ )
                  {
                     if (frustum[j].dotCoord(object[i]->position()) <= -object[i]->getRadius())
                     {
                        inside = false;

                        break;
                     }
                  }

                  if (inside)
                     object[i]->draw();
               }
            }

            break;

        default:
            for (unsigned i = 0; i < object.size(); i++) {
		        if (object[i] && object[i]->belongsTo(category))
			        object[i]->draw();
            }
    }
}

// suspend suspends the textures, lights, sounds, primitive sets, and text
// items in the coordinator
//
void Coordinator::suspend() const {

	for (unsigned i = 0; i < texture.size(); i++)
		if (texture[i])
			texture[i]->suspend();

	for (unsigned i = 0; i < light.size(); i++)
		if (light[i])
			light[i]->suspend();

    for (unsigned i = 0; i < sound.size(); i++)
		if (sound[i]) 
			sound[i]->suspend();

    for (unsigned i = 0; i < graphic.size(); i++)
		if (graphic[i]) 
			graphic[i]->suspend();

    for (unsigned i = 0; i < text.size(); i++)
        if (text[i])
			text[i]->suspend();
}

// restore restores each light, sound, and text item and initializes
// the coordinator timers
//
void Coordinator::restore(int now) {

	for (unsigned i = 0; i < light.size(); i++)
		if (light[i])
			light[i]->restore(now);

	for (unsigned i = 0; i < sound.size(); i++) {
		if (sound[i]) 
			sound[i]->restore(now);
	}
    for (unsigned i = 0; i < text.size(); i++)
        if (text[i])
			text[i]->restore();

    framecount       = 0;
    lastReset        = now;
    lastUpdate       = now;
    lastCameraToggle = now;
    lastHUDToggle    = now;
}

// release releases the textures, vertex lists, and text items in the
// coordinator
//
void Coordinator::release() const {

	for (unsigned i = 0; i < texture.size(); i++)
		if (texture[i])
			texture[i]->release();

    for (unsigned i = 0; i < graphic.size(); i++)
		if (graphic[i]) 
			graphic[i]->release();

    for (unsigned i = 0; i < text.size(); i++)
        if (text[i])
			text[i]->release();
}

// remove removes the pointer to iObject *o from the coordinator
//
bool Coordinator::remove(const iObject* o) {
    bool rc = false;

    for (unsigned i = 0; i < object.size(); i++)
        if (object[i] == o) {
            object[i] = 0;
            rc = true;
        }
    while (object.size() && !object[object.size() - 1])
        object.pop_back();

    return rc;
}

// remove removes the pointer to iTexture *t from the coordinator
//
bool Coordinator::remove(const iTexture* t) {
    bool rc = false;

    for (unsigned i = 0; i < texture.size(); i++)
        if (texture[i] == t) {
            texture[i] = 0;
            rc = true;
        }
    while (texture.size() && !texture[texture.size() - 1])
        texture.pop_back();

    return rc;
}

// remove removes the pointer to iLight *l from the coordinator
//
bool Coordinator::remove(const iLight* l) {
    bool rc = false;

    for (unsigned i = 0; i < light.size(); i++)
        if (light[i] == l) {
            light[i] = 0;
            rc = true;
        }
    while (light.size() && !light[light.size() - 1])
        light.pop_back();

    return rc;
}

// remove removes the pointer to iCamera *l from the coordinator
//
bool Coordinator::remove(const iCamera* c) {
    bool rc = false;

    for (unsigned i = 0; i < camera.size(); i++)
        if (camera[i] == c) {
            camera[i] = 0;
            rc = true;
        }
    while (camera.size() && !camera[camera.size() - 1])
        camera.pop_back();

    return rc;
}

// remove removes the pointer to iSound *s from the coordinator
//
bool Coordinator::remove(const iSound* s) {
    bool rc = false;

    for (unsigned i = 0; i < sound.size(); i++)
        if (sound[i] == s) {
            sound[i] = 0;
            rc = true;
        }
    while (sound.size() && !sound[sound.size() - 1])
        sound.pop_back();

    return rc;
}

// remove removes the pointer to iGraphic *v from the coordinator
//
bool Coordinator::remove(const iGraphic* g) {
    bool rc = false;

    for (unsigned i = 0; i < graphic.size(); i++)
        if (graphic[i] == g) {
            graphic[i] = 0;
            rc = true;
        }
    while (graphic.size() && !graphic[graphic.size() - 1])
        graphic.pop_back();

    return rc;
}

// remove removes the pointer to iText *t from the coordinator
//
bool Coordinator::remove(const iText* t) {
    bool rc = false;

    for (unsigned i = 0; i < text.size(); i++)
        if (text[i] == t) {
            text[i] = 0;
            rc = true;
        }
    while (text.size() && !text[text.size() - 1])
        text.pop_back();

    return rc;
}

// destructor deletes all of the coordinator elements
//
Coordinator::~Coordinator() {

    for (unsigned i = 0; i < object.size(); i++)
        if (object[i]) 
            object[i]->Delete();

    for (unsigned i = 0; i < texture.size(); i++)
        if (texture[i]) 
            texture[i]->Delete();

    for (unsigned i = 0; i < light.size(); i++)
        if (light[i])
            light[i]->Delete();

    for (unsigned i = 0; i < camera.size(); i++)
        if (camera[i])
            camera[i]->Delete();

    for (unsigned i = 0; i < sound.size(); i++)
        if (sound[i])
            sound[i]->Delete();

    for (unsigned i = 0; i < graphic.size(); i++)
        if (graphic[i])
            graphic[i]->Delete();

    for (unsigned i = 0; i < text.size(); i++)
        if (text[i]) 
			text[i]->Delete();
}
