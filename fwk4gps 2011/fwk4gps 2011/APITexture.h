#ifndef _API_TEXTURE_H_
#define _API_TEXTURE_H_

/* APITexture Definition - Direct API Branch
 *
 * APITexture.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "DirectAPISettings.h" // for graphics api headers
#include "iAPITexture.h"       // for the APITexture Interface

//-------------------------------- APITexture ---------------------------------
//
// The APITexture class implements a Texture at the API level
//
class APITexture : public iAPITexture {

    static LPDIRECT3DDEVICE9 d3dd;    // points to the display device
    static LPD3DXSPRITE      manager; // points to the sprite manager
    static int               width;   // display width
    static int               height;  // display height

    wchar_t*                 file;    // points to file that holds image

    LPDIRECT3DTEXTURE9 tex;           // interface to texture COM object

    APITexture(const wchar_t*);
	virtual ~APITexture();

	void setSamplerState(int i, unsigned flags) const;
	void setup(int w, int h, int c);

  public:
	friend iAPITexture* CreateAPITexture(const wchar_t*);
    static void connectsThrough(void* d)     { d3dd = (LPDIRECT3DDEVICE9)d; }
    static void isManagedBy(void* m)         { manager = (LPD3DXSPRITE)m; }
    static void hasClientArea(int w, int h)  { width = w; height = h; }
	APITexture(const APITexture&);
	iAPITexture& operator=(const APITexture&);
    iAPITexture* clone() const { return new APITexture(*this); }
	// execution
	void   attach(unsigned flags);
	void   detach();
	void   draw(const RelRect& rect, unsigned char alpha);
    // suspension
    void   suspend();
	// termination
    void   release();
	void   Delete() const { delete this; }
};

#endif