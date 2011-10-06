#ifndef _TEXTURE_H_
#define _TEXTURE_H_

/* Texture Definition - Model Branch
 *
 * Texture.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iTexture.h" // for the Texture Interface

//-------------------------------- Texture ------------------------------------
//
// The Texture class defines the structure of a texture in the Model Branch
//
class iCoordinator;
class iAPITexture;

class Texture : public iTexture {

	static iCoordinator* coordinator; // points to the coordinator object

	iAPITexture*   apiTexture;        // points to the api texture

	Texture(const wchar_t* file);
	Texture(const Texture&);
	virtual ~Texture();

  public:
	friend iTexture* CreateTexture(const wchar_t* file);
	static void isCoordinatedBy(iCoordinator* c) { coordinator = c; }
	Texture& operator=(const Texture&);
    iTexture* clone() const                      { return new Texture(*this); }
	// execution
	void attach(unsigned flags) const;
	void detach();
	void draw(const RelRect& hudPos, unsigned char a) const;
    // suspension
	void suspend();
	// termination
    void release();
	void Delete() const                          { delete this; }
};

#endif
