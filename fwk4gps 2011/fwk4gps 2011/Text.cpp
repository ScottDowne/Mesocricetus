/* Text Implementation - Model Branch
 *
 * Text.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCoordinator.h"     // for the Coordinator Interface
#include "iAPIText.h"         // for the APIText Interface
#include "iUtilities.h"       // for error()

#include "Text.h"             // for the Text class definition
#include "ModelSettings.h"    // for Coordinator Action enumeration constants
#include "MathDeclarations.h" // for Rect

//-------------------------------- Text --------------------------------------
//
// The Text class defines the structure of a text item in the Model Branch
//
// The Coordinator manages the Text Instances
//
iCoordinator* Text::coordinator = NULL;

// CreateText creates a Text object
//
iText* CreateText(RelRect r, const wchar_t* str, unsigned f) {

	return new Text(r, str, f);
}

iText* Clone(const iText* src) {

    return src->clone();
}

// constructor adds the object's address to the coordinator, validates the
// bounds of the text item, allocates memory for the bounds, and initializes
// the text string
//
Text::Text(RelRect r, const wchar_t* text, unsigned flags) : dFlags(flags) {

    if (!coordinator)
        error(L"Text::00 Couldn\'t access the Coordinator");
    else if(!coordinator->add(this))
        error(L"Text::01 Couldn\'t add the text to the Coordinator");

    font = CreateAPIText();
    str  = NULL;

    if (r.topLeftX < 0.01f) r.topLeftX = 0.01f;
    else if (r.topLeftX > 0.99f) r.topLeftX = 0.99f;
    if (r.topLeftY < 0.01f) r.topLeftY = 0.01f;
    else if (r.topLeftY > 0.99f) r.topLeftY = 0.99f;
    if (r.bottomRightX < 0.01f) r.bottomRightX = 0.01f;
    else if (r.bottomRightX > 0.99f) r.bottomRightX = 0.99f;
    if (r.bottomRightY < 0.01f) r.bottomRightY = 0.01f;
    else if (r.bottomRightY > 0.99f) r.bottomRightY = 0.99f;

	rect  = new RelRect();
    *rect = r;


    if (text) set(text);
}

// copy constructor adds the new object's address to the coordinator,
// initializes the instance pointers and calls the assignment operator
//
Text::Text(const Text& src) {

    if (!coordinator)
        error(L"Text::00 Couldn\'t access the Coordinator");
    else if(!coordinator->add(this))
        error(L"Text::01 Couldn\'t add the text to the Coordinator");

    font  = NULL;
    str   = NULL;
	*this = src;
}

// assignment operator discards old data and copies over data from src
// creates a new APIText object for the current Text object
//
Text& Text::operator=(const Text& src) {

	if (this != &src) {
		if (font)
			font->Delete();
        if (src.font)
            font = src.font->clone();
        else
            font = NULL;

        if (str)
			delete [] str;
		int len = strlen(src.str);
		str     = new wchar_t[len + 1];
		strcpy(str, src.str, len);

        if (rect)
            delete [] rect;
        rect    = new RelRect(*src.rect);

        dFlags  = src.dFlags;
	}

	return *this;
}

// rect returns the absolute rectangle for the text in [0,0|1,1] space
//
RelRect Text::absRect() const {
    float x, y;
    coordinator->hudPos(x, y);

    return RelRect(x + HUD_W * rect->topLeftX, y + HUD_H * rect->topLeftY, 
     x + HUD_W * rect->bottomRightX, y + HUD_H * rect->bottomRightY);
}

// set copies the string text into the Text object
//
void Text::set(const wchar_t* text) {

    int len = strlen(text);
    if (str)
        delete [] str;
    str = new wchar_t[len + 1];
    strcpy(str, text, len);
}

// draw draws the text string using the APIText object
//
void Text::draw() {

    if (font)
		font->draw(this);
}

// suspends suspends the font associated with the text object
//
void Text::suspend() {

	if (font)
		font->suspend();
}

// restore restores the font associated with the text object
//
void Text::restore() {

	if (font)
		font->restore();
}

// release releases the font associated with the text object
//
void Text::release() {

	if (font)
		font->release();
}

// destructor destroys the font associated with this text object
// and the text stored in the object and removes the text object
// from the HUD
//
Text::~Text() {

    if (font)
        font->Delete();
    if (str)
        delete [] str;
    if (rect)
        delete rect;
    if (coordinator)
        coordinator->remove(this);
    else
        error(L"Text::90 Couldn\'t access the Coordinator");
}
