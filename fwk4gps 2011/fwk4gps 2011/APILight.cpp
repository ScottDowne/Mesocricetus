/* APILight Implementation  - Direct API Branch
 *
 * APILight.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iLight.h"           // for the Light Interface
#include "iUtilities.h"       // for error()

#include "APILight.h"         // for the APILight class definition
#include "MathDeclarations.h" // for Matrix, Vector and Colour
#include "Configuration.h"    // for LightType enumeration  

//-------------------------------- APILight -----------------------------------
//
// The APILight class implements a Light at the API level
//
// The API connectivity is defined by the Display object
//
LPDIRECT3DDEVICE9 APILight::d3dd  = NULL;
//
// The lightIndex array keeps track of the available light indeces
//
bool* APILight::lightIndex = NULL;
int   APILight::maxLights  = 0;
//
// CreateAPILight creates an APILight object
//
iAPILight* CreateAPILight() {

	return new APILight;
}

// constructor initializes the api light as needing to be setup
//
APILight::APILight() : isSetup(false), index(0) {}

// copy constructor

APILight::APILight(const APILight& src) {

    *this = src;
}

APILight& APILight::operator=(const APILight& src) {

    if (this != &src) {
        isSetup       = false;
        index         = 0;
    }

    return *this;
}

void APILight::alloc(int max) {

    if (lightIndex)
        delete [] lightIndex;
    maxLights = max;
    lightIndex = new bool[maxLights];
    for (int i = 0; i < maxLights; i++)
        lightIndex[i] = true;
}

void APILight::dealloc() {

    if (lightIndex)
        delete [] lightIndex;
    lightIndex = NULL;
}

// setup finds an index for the api light and creates the api light using
// light's properties - returns true if successful, false otherwise
//
bool APILight::setup(const iLight* light) {

	bool rc = false;

	// find an index for this api light and assign it
    index = -1;
    for (int i = 0; i < maxLights; i++) {
        if (lightIndex[i]) {
            lightIndex[i] = false;
            index = i;
            i = maxLights;
        }
    }
	if (index == -1)
		error(L"APILight::21 No more room for lights on this device");
	else {
		Colour d           = light->diffuse();
		Colour a           = light->ambient();
		Colour s           = light->specular();
		Vector spot        = light->spot();
		LightType type     = light->isType();
		Vector attenuation = light->attenuation();
		float  range       = light->range();
		Vector p           = light->position();
		Vector o           = light->orientation('z');

		D3DLIGHT9 d3dLight;
		ZeroMemory(&d3dLight, sizeof d3dLight);
		switch (type) {
		  case POINT_LIGHT:
			d3dLight.Type = D3DLIGHT_POINT;
			break;
		  case SPOT_LIGHT:
			d3dLight.Type = D3DLIGHT_SPOT;
			break;
		  case DIRECTIONAL_LIGHT:
			d3dLight.Type = D3DLIGHT_DIRECTIONAL;
			break;
		}
		d3dLight.Diffuse      = D3DXCOLOR(d.r, d.g, d.b, d.a);
		d3dLight.Ambient      = D3DXCOLOR(a.r, a.g, a.b, a.a);
		d3dLight.Specular     = D3DXCOLOR(s.r, s.g, s.b, s.a);
		d3dLight.Attenuation0 = attenuation.x;
		d3dLight.Attenuation1 = attenuation.y;
		d3dLight.Attenuation2 = attenuation.z;
		d3dLight.Phi          = spot.x;
		d3dLight.Theta        = spot.y;
		d3dLight.Falloff      = spot.z;
		d3dLight.Range        = range;
		d3dLight.Position     = D3DXVECTOR3(p.x, p.y, p.z);
		d3dLight.Direction    = D3DXVECTOR3(o.x, o.y, o.z);

		if (!d3dd || FAILED(d3dd->SetLight(index, &d3dLight)))
			error(L"APILight::22 Failed to create device light");
		else
			rc = true;
	}

	return rc;
}

// turnOn sets up the api light if it needs to be setup,
// adjusts its position and orientation if it is mobile and
// turns on the device light 
//
void APILight::turnOn(const iLight* light) {

	if (!isSetup) isSetup = setup(light);

	if (isSetup) {
		Vector p = light->position();
		Vector o = light->orientation('z');
		D3DLIGHT9 d3dLight;
		if (FAILED(d3dd->GetLight(index, &d3dLight)))
			error(L"APILight::23 Failed to find a device light");
		else {
			d3dLight.Position  = D3DXVECTOR3(p.x, p.y, p.z);
			d3dLight.Direction = D3DXVECTOR3(o.x, o.y, o.z);
			if (FAILED(d3dd->SetLight(index, &d3dLight)))
				error(L"APILight:24 Failed to update position");
		}
		d3dd->LightEnable(index, true);
	}
}

// update sets up the api light if it needs to be setup and adjusts
// its position and orientation 
//
void APILight::update(const iLight* light) {

	if (!isSetup) isSetup = setup(light);

	if (isSetup) {
		Vector p = light->position();
		Vector o = light->orientation('z');
		D3DLIGHT9 d3dLight;
		if (FAILED(d3dd->GetLight(index, &d3dLight)))
			error(L"APILight::23 Failed to find a device light");
		else {
			d3dLight.Position  = D3DXVECTOR3(p.x, p.y, p.z);
			d3dLight.Direction = D3DXVECTOR3(o.x, o.y, o.z);
			if (FAILED(d3dd->SetLight(index, &d3dLight)))
				error(L"APILight:24 Failed to update position");
		}
		d3dd->LightEnable(index, true);
    }
}

// turnOff turns off the api light
//
void APILight::turnOff(const iLight* light) {

    if (isSetup) {
         if (d3dd)
            d3dd->LightEnable(index, false);
    }
}

// suspend turns off the api light and makes its index available
// for future restoration
//
void APILight::suspend(const iLight* light) {

    turnOff(light);

	if (isSetup) {
		lightIndex[index] = true;
		isSetup = false;
	}
}
