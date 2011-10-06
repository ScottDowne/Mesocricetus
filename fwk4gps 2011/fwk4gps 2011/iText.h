#ifndef _I_TEXT_H_
#define _I_TEXT_H_

/* Text Interface - Model Branch
 *
 * iText.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- Text ---------------------------------------
//
// iText is the interface to the Text class of the model branch
//
struct RelRect;

class iText {
  public:
    virtual iText*   clone() const           = 0;
	// execution
    virtual void     set(const wchar_t* text) = 0;
    virtual RelRect  absRect() const          = 0;
	virtual const    wchar_t* text() const    = 0;
	virtual void     draw()                   = 0;
	virtual unsigned flags() const            = 0;
    // suspension
	virtual void     suspend()                = 0;
    virtual void     restore()                = 0;
	// termination
	virtual void     release()                = 0;
	virtual void     Delete() const           = 0;
};

iText* CreateText(RelRect, const wchar_t* = 0, unsigned = 0);

iText* Clone(const iText*);

#endif
