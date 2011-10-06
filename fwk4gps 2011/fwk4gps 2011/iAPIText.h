#ifndef _I_API_TEXT_H_
#define _I_API_TEXT_H_

/* APIText Interface - Direct API Branch
 *
 * iAPIText.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPIText -----------------------------------
//
// iAPIText is the Interface to the APIText class
//
class iAPIText {
  public:
    virtual iAPIText* clone() const      = 0;
	// execution function
    virtual void draw(const void* text)  = 0;
	// suspension
    virtual void suspend()               = 0;
    virtual bool restore()               = 0;
	// termination
	virtual void release()               = 0;
	virtual void Delete() const          = 0;
};

iAPIText* CreateAPIText();

#endif