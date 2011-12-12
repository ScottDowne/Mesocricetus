/* APIText Implementation - Direct API Branch
 *
 * APIText.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iText.h"            // for the Text Interface
#include "iUtilities.h"       // for error()

#include "APIText.h"          // for the APIText class definition
#include "MathDeclarations.h" // for Rect
#include "Configuration.h"    // for Font display colour  

//-------------------------------- APIText ------------------------------------
//
// The APIText class implements a Text item at the API level 
//
// The API connectivity is defined by the Display object
//
LPDIRECT3DDEVICE9 APIText::d3dd    = NULL;
LPD3DXSPRITE      APIText::manager = NULL;
int               APIText::width   = 0;
int               APIText::height  = 0;

// CreateAPIText creates an APIText object
//
iAPIText* CreateAPIText() {

	return new APIText();
}

// constructor initializes the instance variables
//
APIText::APIText() : flags(0) {

    d3dfont = NULL;
}

// copy constructor
//
APIText::APIText(const APIText& src) {

    *this = src;
}

// assignment operator does not copy the font
//
APIText& APIText::operator=(const APIText& src) {

    if (this != &src) {
        d3dfont = NULL;
        flags = src.flags;
    }

    return *this;
}

// setup retrieves an interface to the font COM object 
//
void APIText::setup(const void* text) {

    // define the text alignment flags
	unsigned f = ((iText*)text)->flags();
	if (!f) f = TEXT_DEFAULT;
    flags = 0;
    if (f & TEXT_LEFT)        flags |= DT_LEFT;
    else if (f & TEXT_RIGHT)  flags |= DT_RIGHT;
    else if (f & TEXT_CENTER) flags |= DT_CENTER;
    if (f & TEXT_TOP)         flags |= DT_TOP;
    else if (f & TEXT_BOTTOM) flags |= DT_BOTTOM;
    else if (f & TEXT_MIDDLE) flags |= DT_VCENTER;

    // retrieve an API interface to the font object 
    if (FAILED(
     D3DXCreateFont(d3dd, 40, 0, FW_EXTRABOLD, 0, 0, 
     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
     FF_DONTCARE | DEFAULT_PITCH, L"ARIAL", &d3dfont)))
        error(L"APIText::10 Failed to retrieve the font interface");
}

// draw defines the location of the font and draws text->text() at that 
// location using color TEXT_R, TEXT_G, TEXT_B, TEXT_A
//
void APIText::draw(const void* text) {

    if (!d3dfont) setup(text);

    iText* txt = (iText*)text;

	if (d3dfont && manager && txt->text()) {
		// creates the drawing rectangle in screen coordinates
		RECT rect;
        Rect r = txt->absRect().absolute(width, height);
        SetRect(&rect, r.topLeftX, r.topLeftY, r.bottomRightX, r.bottomRightY);
        // draws the text string str within the drawing rectangle
		d3dfont->DrawText(manager, txt->text(), -1, &rect, flags,
         D3DCOLOR_ARGB(TEXT_A, TEXT_R, TEXT_G, TEXT_B));
	}
}

// suspend detaches the font from the display device
//
void APIText::suspend() {

    // detaches references to the display device
    if (d3dfont)
        d3dfont->OnLostDevice();
}

// restore re-acquires the reference to the display device
//
bool APIText::restore() {

    bool rc = false;

    // re-acquire the reference to the graphics card
    if (d3dfont)
        rc = d3dfont->OnResetDevice() == D3D_OK;

    return rc;
}

// release releases the interface to the font object 
//
void APIText::release() {

	suspend();

    if (d3dfont) {
        d3dfont->Release();
        d3dfont = NULL;
    }
}

// destructor releases the font object
//
APIText::~APIText() {

	release();
}

