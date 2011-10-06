#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/* Display Definition - Direct API Branch
 *
 * Display.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "DirectAPISettings.h" // for graphics api headers
#include "iDisplay.h"          // for the Display Interface
#include "MathDeclarations.h"  // for Matrix

//-------------------------------- Display ------------------------------------
//
// The Display class manages the API connectivity for the Graphics Card 
//
class Display : public iDisplay {

	iContext* context;   // points to the Context object
	HWND      hwnd;      // points to the main application window

    // available configuration dimensions
    int   (*modeDim)[2]; // points to list of available mode widths, heights

    // selected configuration
    int   display;       // display adapter identifier
    int   mode;          // resolution mode identifier
    int   pixel;         // pixel format identifier
    int   width;         // width of the backbuffer
    int   height;        // height of the backbuffer
	float aspect;        // aspect ratio of viewing area
    int   maxLights;     // max no of lights supported by selected adapter
	bool  runInWindow;   // run in resizeable windowed mode

    LPDIRECT3D9 d3d;             // Interface to the Direct3D object
    LPDIRECT3DDEVICE9 d3dd;      // Interface to Direct3D display device
    D3DPRESENT_PARAMETERS d3dpp; // parameters for creating/restoring D3D
                                 // display device
    LPD3DXSPRITE scrManager;     // Interface to the screen manager object

    Matrix projection;

    void setupProjection();      // sets up the projection matrix
    void setupLighting();        // sets up the lighting
	void setupBlending();        // sets up the alpha blending

    Display(iContext* c);
	Display(const Display& d);            // prevents copying
	Display& operator=(const Display& d); // prevents assignments
    virtual ~Display();

  public:
	friend iDisplay* CreateDisplay(iContext* c);
	// configuration 
	bool   interrogate(void*);
    void   configure();
	// initialization
    bool   setup(void*);
	// execution
    void   beginDrawFrame();
    void   beginDrawObject(const Matrix& world);
    void   set(const Reflectivity& reflectivity);
    void   set(RenderState, bool);
    void   beginDraw(unsigned flags);
    void   endDraw();
    void   endDrawFrame();
    void   resize();
    // suspension
    void   suspend();
    bool   restore();
	// termination
    void   release();
	void   Delete() { delete this; }
};

#endif