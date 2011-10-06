#ifndef _I_API_TEXTURE_H_
#define _I_API_TEXTURE_H_

/* APITexture Interface - Direct API Branch
 *
 * iAPITexture.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPITexture --------------------------------
//
// iAPITexture is the Interface to the APITexture class
//
struct RelRect;

class iAPITexture {
  public:
    virtual iAPITexture* clone() const                                = 0;
	// execution
	virtual void attach(unsigned flags)                               = 0;
	virtual void detach()                                             = 0;
	virtual void draw(const RelRect&, unsigned char)                  = 0;
    // suspension
	virtual void suspend()                                            = 0;
	// termination
	virtual void release()                                            = 0;
	virtual void Delete() const                                       = 0;
};

iAPITexture* CreateAPITexture(const wchar_t*);

#endif