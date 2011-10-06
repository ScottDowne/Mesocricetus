/* Light Implementation - Model Branch
 *
 * Light.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCoordinator.h"    // for the Coordinator Interface
#include "iAPILight.h"       // for the APILight Interface
#include "iUtilities.h"      // for error()

#include "Light.h"           // for Light class definitions
#include "MathDefinitions.h" // for math functions in coordinator coordinates
#include "ModelSettings.h"   // for POINT_LIGHT, DIRECTIONAL_LIGHT, SPOT_LIGHT

//-------------------------------- Light --------------------------------------
//
// The Light class defines the structure of a light source
//
// The coordinator coordinates the Light Instances
//
iCoordinator* Light::coordinator = NULL;

// CreateDistantLight creates a distant Light object
//
iLight* CreateDistantLight(Colour d, Colour a, Colour s, bool o) {

	return new Light(DIRECTIONAL_LIGHT, d, a, s, 0, o, 1, 0, 0);
}

// CreatePointLight creates a point Light object
//
iLight* CreatePointLight(Colour d, Colour a, Colour s, float r, bool o,
 float a0, float a1, float a2) {

	return new Light(POINT_LIGHT, d, a, s, r, o, a0, a1, a2);
}

iLight* Clone(const iLight* src) {

    return src->clone();
}

// constructor stores the properties of the light, creates the APILight
// object and initializes the reference time
//
Light::Light(LightType t, Colour d, Colour a, Colour s, float r, bool o, 
 float a0, float a1, float a2) : type(t), on(false), diffuse_(d), ambient_(a),
 specular_(s), range_(r), attenuation0(a0), attenuation1(a1), attenuation2(a2),
 turnOn(o), turnOff(false) {

    if (!coordinator)
        error(L"Light::00 Couldn\'t access the Coordinator");
    else if(!coordinator->add(this))
        error(L"Light::01 Couldn\'t add the light to Coordinator");

    apiLight = CreateAPILight();

	// reference time
	lastToggle  = 0;
}

// copy constructor initializes the APILight pointer and copies over the
// instance variables from src
//
Light::Light(const Light& src) {

    if (!coordinator)
        error(L"Light::02 Couldn\'t access the Coordinator");
    else if(!coordinator->add(this))
        error(L"Light::03 Couldn\'t add the light to the Coordinator");

	apiLight = NULL;
	*this = src;
}

// assignment operator discards the old APILight and copies over properties
// from src
//
Light& Light::operator=(const Light& src) {

	if (this != &src) {
        *((Frame*)this) = src;
		if (apiLight) 
			apiLight->Delete();
        if (src.apiLight)
            apiLight = src.apiLight->clone();
        else
            apiLight = NULL;
		type            = src.type;   
		on              = false;     
		turnOn          = src.on;    
		turnOff         = false;   
		diffuse_        = src.diffuse_;  
		ambient_        = src.ambient_;  
		specular_       = src.specular_;  
		range_          = src.range_;       
		attenuation0    = src.attenuation0; 
		attenuation1    = src.attenuation1;
		attenuation2    = src.attenuation2; 
		lastToggle      = 0;
	}

	return *this;
}

// toggle toggles the light's state if outside the latency period
// and returns the future state
//
bool Light::toggle(int now) {

	bool rc = on;

    if (now - lastToggle > KEY_LATENCY) {
		if (on)
			turnOff = true;
		else
			turnOn  = true;
		rc          = !rc;
        lastToggle  = now;
    }

	return rc;
}

// update updates or turns the api light on or off
//
void Light::update() {

	if (turnOn) {
		apiLight->turnOn(this);
		turnOn = false;
		on     = true;
	}
	else if (turnOff) {
		apiLight->turnOff(this);
		turnOff = false;
		on      = false;
	}
    else if (on) {
		apiLight->update(this);
    }
}

// suspend suspends the api light and prepares for turning on the light
// upon restoration
//
void Light::suspend() {

	if (apiLight) 
		apiLight->suspend(this);
	turnOn  = on || turnOn;
	turnOff = false;
}

// restore reinitializes the time of the last toggle
//
void Light::restore(int now) {

	lastToggle = now;
}

// destructor deletes the light source from the coordinator
//
Light::~Light() {

    if (coordinator)
        coordinator->remove(this);
    else
        error(L"Light::90 Couldn\'t access the Coordinator");
}

//-------------------------------- SpotLight ----------------------------------
//
// CreateSpotLight creates a spot Light object
//
iLight* CreateSpotLight(Colour d, Colour a, Colour s, float r, bool o, 
 float a0, float a1, float a2, float ph, float th, float f) {

	return new SpotLight(d, a, s, r, o, a0, a1, a2, ph, th, f);
}

// constructor stores the properties of the light
//
SpotLight::SpotLight(Colour d, Colour a, Colour s, float r, bool o, float a0, 
 float a1, float a2, float ph, float th, float f) : Light(SPOT_LIGHT,
 d, a, s, r, o, a0, a1, a2), phi(ph), theta(th), falloff(f) { }

// spot returns the spot factors in a Vector
//
Vector SpotLight::spot() const {

    return Vector(phi, theta, falloff);
}
