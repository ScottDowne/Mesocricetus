#ifndef _LIGHT_H_
#define _LIGHT_H_

/* Light Definition - Model Branch
 *
 * Light.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iLight.h"           // for the Light Interface
#include "MathDeclarations.h" // for Colour

//-------------------------------- Light --------------------------------------
//
// The Light class defines the structure of a light source
//
class iCoordinator;
class iAPILight;
class iAPITexture;

class Light : public iLight {

    static iCoordinator* coordinator; // points to the coordinator object

	iAPILight*     apiLight;     // points to the device light

    LightType      type;         // light type
	bool           on;           // light is on?
	bool           turnOn;       // turn on the light?
	bool           turnOff;      // turn off the light?
    Colour         diffuse_;     // diffuse component
    Colour         ambient_;     // ambient component
    Colour         specular_;    // specular component
    float          range_;       // beyond which light ceases
    float          attenuation0; // constant attenuation
    float          attenuation1; // linear attenuation
    float          attenuation2; // quadratic attenuation

    int            lastToggle;   // time of the last toogle

  protected:
    Light(LightType t, Colour d, Colour a, Colour s, float r, bool, float a0, 
	 float a1, float a2);
	Light(const Light&);
	virtual ~Light();

  public:
	friend iLight* CreatePointLight(Colour d, Colour a, Colour s, float r, 
	 bool o, float a0, float a1, float a2);
	friend iLight* CreateDistantLight(Colour d, Colour a, Colour s, bool o);
	static    void isCoordinatedBy(iCoordinator* c) { coordinator = c; }
	Light& operator=(const Light&);
    iLight*   clone() const       { return new Light(*this); }
	// execution
	LightType isType() const      { return type; }
   	Colour    diffuse() const     { return diffuse_; }
	Colour    ambient() const     { return ambient_; }
	Colour    specular() const    { return specular_; }
	float     range() const       { return range_; }
	Vector    spot() const        { return Vector(0, 0, 0);}
	Vector    attenuation() const { return Vector(attenuation0, 
		       attenuation1, attenuation2); }
    bool      isOn() const        { return on; }
	bool      toggle(int now);
	void      update();
    // suspension
	void      suspend();
	void      restore(int now);
	// termination
	void      Delete() const { delete this; }
};

class SpotLight : public Light {

    float  phi;          // angle of spot light in radians
    float  theta;        // angle of inner spot in radians
    float  falloff;      // falloff factor [0,1]

	SpotLight(Colour d, Colour a, Colour s, float r, bool o, float a0, 
	 float a1, float a2, float ph, float th, float f);

  public:
    friend iLight* CreateSpotLight(Colour d, Colour a, Colour s, float r, 
     bool o, float a0, float a1, float a2, float ph, float th, float f);
    iLight* clone() const { return new SpotLight(*this); }
	Vector  spot() const;
};

#endif
