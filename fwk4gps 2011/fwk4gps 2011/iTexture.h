#ifndef _I_TEXTURE_H_
#define _I_TEXTURE_H_

/* Texture Interface - Model Branch
 *
 * iTexture.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Frame.h" // for the Frame class definition

//-------------------------------- iTexture -----------------------------------
//
// iTexture is the Interface to the Texture class
//
struct RelRect;

class iTexture : public Frame {
  public:
    virtual iTexture* clone() const                                    = 0;
	// execution
	virtual void attach(unsigned) const                                = 0;
	virtual void detach()                                              = 0;
	virtual void draw(const RelRect&, unsigned char) const             = 0;
    // suspension
	virtual void suspend()                                             = 0;
	// termination
	virtual void release()                                             = 0;
	virtual void Delete() const                                        = 0;
};

iTexture* CreateTexture(const wchar_t* file);

iTexture* Clone(const iTexture*);

#endif
