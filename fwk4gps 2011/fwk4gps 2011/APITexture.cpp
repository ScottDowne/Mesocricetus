/* APITexture Implementation - Direct API Branch
 *
 * APITexture.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iTexture.h"      // for the Texture Interface
#include "iUtilities.h"    // for error()

#include "APITexture.h"    // for the APITexture class definition
#include "Configuration.h" // for TEX_? and SPRITE_? 

//-------------------------------- APITexture ------------------------------
//
// The APITexture class implements a texture at the API level
//
// The API connectivity is defined by the Display object
//
LPDIRECT3DDEVICE9   APITexture::d3dd    = NULL;
LPD3DXSPRITE        APITexture::manager = NULL;
int                 APITexture::width   = 0;
int                 APITexture::height  = 0;

iAPITexture* CreateAPITexture(const wchar_t* f) {

	return new APITexture(f);
}

// constructor initializes the texture identifier
//
APITexture::APITexture(const wchar_t* f) {

	if (f) {
        int len = strlen(TEXTURE_DIRECTORY) + strlen(f) + 1;
	    file = new wchar_t[len + 1];
	    ::nameWithDir(file, TEXTURE_DIRECTORY, f, len);
    }
    else
        file = NULL;

    tex = NULL;
}

APITexture::APITexture(const APITexture& src) {

    file  = NULL;
    tex   = NULL;
    *this = src;    
}

iAPITexture& APITexture::operator=(const APITexture& src) {

    if (this != &src) {
        if (file)
            delete [] file;
        if (src.file) {
            int len = strlen(src.file);
            file = new wchar_t[len + 1];
            strcpy(file, src.file, len);
        }
        else
            file = NULL;
        suspend();
        tex = NULL;
    }

    return *this;
}

// setup creates the api texture from the texture file
//
void APITexture::setup(int width, int height, int key) {

    // create a texture COM object from the texture file
    //
	if (manager && file && FAILED(D3DXCreateTextureFromFileEx(d3dd, file, 
     width, height, D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
     D3DX_DEFAULT, D3DX_DEFAULT, key, NULL, NULL, &tex))) {
		error(L"APITexture::11 Failed to create texture COM object from file");
		tex = NULL;
	}
}

// draw draws the api texture directly
//
void APITexture::draw(const RelRect& rect, unsigned char alpha) {

	if (!tex) setup((int)(width * (rect.bottomRightX - rect.topLeftX)), 
	 (int)(height * (rect.bottomRightY - rect.topLeftY)), 0xFF000000);

	if (manager && tex) {
		D3DXVECTOR3 topLeft(width * rect.topLeftX, height * rect.topLeftY, 0);
		manager->Draw(tex, NULL, NULL, &topLeft, 
		 D3DCOLOR_RGBA(SPRITE_R, SPRITE_G, SPRITE_B, alpha));
	}
}

// attach attaches the api texture to sampling stage i
//
void APITexture::attach(unsigned flags) {

	if (!tex) setup(0, 0, 0);

    if (tex) {
        d3dd->SetTexture(0, tex);
		setSamplerState(0, flags);
    }
}

// setSamplerState sets the sampling state for the texture according 
// to the filtering flags
//
void APITexture::setSamplerState(int i, unsigned flags) const {

    if (flags & TEX_MIN_POINT)
        d3dd->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    else if (flags & TEX_MIN_LINEAR)
        d3dd->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    else if (flags & TEX_MIN_ANISOTROPIC)
        d3dd->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC); 
    if (flags & TEX_MAG_POINT)
        d3dd->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    else if (flags & TEX_MAG_LINEAR)
        d3dd->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

// detach detaches the api texture from sampling stage i
//
void APITexture::detach() {

    if (tex) {
        d3dd->SetTexture(0, NULL);
    }
}

// suspend releases the api texture
//
void APITexture::suspend() {

    // release the Interface to the texture COM object
    if (tex) {
        tex->Release();
        tex = NULL;
    }
}

// releases suspends the api texture
//
void APITexture::release() {

    suspend();
}

// destructor releases the api texture 
//
APITexture::~APITexture() {

   release();
}
