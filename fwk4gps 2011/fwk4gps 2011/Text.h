#ifndef _TEXT_H_
#define _TEXT_H_

/* Text Definition - Model Branch
 *
 * Text.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iText.h" // for the Text Interface

//-------------------------------- Text ---------------------------------------
//
// The Text class defines the structure of a text item in the Model Branch
//
class iCoordinator;
class iAPIText;

class Text : public iText {

    static iCoordinator* coordinator; // points to the coordinator object

    iAPIText* font;                   // points to the font at the API level
    wchar_t*  str;                    // points to the text string
    RelRect*  rect;                   // relative rectangle within the hud
	unsigned  dFlags;                 // text display flags

    Text(RelRect r, const wchar_t* str, unsigned f);
    Text(const Text& src);
    virtual ~Text();

  public:
	friend iText* CreateText(RelRect r, const wchar_t* str, unsigned f);
	static void isCoordinatedBy(iCoordinator* c) { coordinator = c; }
    Text&   operator=(const Text& src);
    iText*  clone() const                        { return new Text(*this); }
	// execution
    void    set(const wchar_t* text);
    RelRect absRect() const;
	const   wchar_t* text() const                { return str; }
	unsigned flags() const                       { return dFlags; }
	void    draw();
    // suspension
	void    suspend();
	void    restore();
	// termination
	void    release();
	void    Delete() const                       { delete this; }
};

#endif
