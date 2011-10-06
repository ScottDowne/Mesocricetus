#ifndef _APITEXT_H_
#define _APITEXT_H_

/* APIText Definition - Direct API Branch
 *
 * APIText.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "DirectAPISettings.h" // for LPD3DXFONT
#include "iAPIText.h"          // for the APIText Interface

//-------------------------------- APIText ------------------------------------
//
// The APIText class implements a Text item at the API level
//
class APIText : public iAPIText {

    static LPDIRECT3DDEVICE9 d3dd;    // points to the display device
    static LPD3DXSPRITE      manager; // points to the font manager 
    static int               width;   // width of the client area
    static int               height;  // height of the client area

	unsigned                 flags;   // font display flags
    LPD3DXFONT               d3dfont; // points to the D3D font Interface

    APIText();
    virtual ~APIText();
    void    setup(const void* text);

  public:
	friend iAPIText* CreateAPIText();
    static void connectsThrough(void* d)   { d3dd = (LPDIRECT3DDEVICE9)d; }
    static void isManagedBy(void* m)     { manager = (LPD3DXSPRITE)m; }
    static void hasClientArea(int w, int h) { width = w; height = h; }
    APIText(const APIText& v);
    APIText& operator=(const APIText& v);
    iAPIText* clone() const              { return new APIText(*this); }
	// execution function
    void   draw(const void* text);
    // suspension
    void   suspend();
    bool   restore();
	// termination
	void   release();
	void   Delete() const                { delete this; }
};

#endif