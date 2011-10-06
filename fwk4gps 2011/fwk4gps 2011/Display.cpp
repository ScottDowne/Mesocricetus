/* Display Implementation - Direct API Branch
 *
 * Display.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iContext.h"        // for the Context Interface
#include "iUtilities.h"      // for error()

#include "Display.h"         // for the Display class definition
#include "Object.h"          // for access to Object::isConnectedTo()
#include "APILight.h"        // for access to APILight::connectsThrough()
#include "APIGraphic.h"      // for access to APIGraphic::connectsThrough()
#include "APITexture.h"      // for access to APITexture::connectsThrough()
#include "APIText.h"         // for access to APIText::connectsThrough()
#include "MathDefinitions.h" // for ::view, ::projection
#include "Configuration.h"   // for MAX_DESC, projection, background settings  

//-------------------------------- Display ------------------------------------
//
// The Display object manages the API Graphics connectivity
//
// CreateDisplay creates the Display object
//
iDisplay* CreateDisplay(iContext* c) {

	return new Display(c);
}

// constructor initializes the instance variables and aligns the Display object
// with the API classes
//
Display::Display(iContext* c) : context(c), hwnd(NULL), width(0), height(0)  {

    // obtain Interface to Direct3D COM object
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d)
        error(L"Display::01 Failed to make Direct3D object");
    d3dd       = NULL;
    scrManager = NULL;
    modeDim    = NULL;

    Object::isConnectedTo(this);
}

// interrogate the host system for the available display configurations
//
bool Display::interrogate(void* hwnd) {

    bool     rc = false;
    int      nAdapters, nModes, nPixelFormats;
	wchar_t  str[MAX_DESC + 1];

    D3DADAPTER_IDENTIFIER9 d3di;
    D3DDISPLAYMODE         mode;
    D3DFORMAT              Format[]  = D3D_DOC_FORMATS;
    // friendly descriptions for each format
    wchar_t*               fmtdesc[] = D3D_FORMAT_DESC;

    // adapter count
    nAdapters = d3d->GetAdapterCount();
    // maximum mode count
    nModes = 0;
    // pixel format count
    // number of pixel formats as described in the Direct3D documentation
    nPixelFormats = D3D_NO_DOC_FORMATS;
 
    // determine maximum number of modes with any adapter
    for (int id = 0; id < nAdapters; id++) {
        if (SUCCEEDED(d3d->GetAdapterIdentifier(id, 0, &d3di))) {
            for (int ip = 0; ip < nPixelFormats; ip++) {
                int i = d3d->GetAdapterModeCount(id, Format[ip]);
                if (i > nModes)
                    nModes = i;
            }
        }
    }
    if (nAdapters > MAX_ADAPTERS) {
        sprintf(str, nAdapters, L" Adapters found - increase MAX_ADAPTERS");
        error(str);
        nAdapters = MAX_ADAPTERS;
    }
    if (nModes > MAX_MODES) {
        sprintf(str, nModes, L" Modes found - increase MAX_MODES");
        error(str);
        nModes = MAX_MODES;
    }
    if (nPixelFormats > MAX_P_FORMATS) {
        sprintf(str, nPixelFormats, L" Pixel Formats found - increase MAX_P_FORMATS");
        error(str);
        nPixelFormats = MAX_P_FORMATS;
    }

    // set dimensions, allocate context memory for descriptions,
    // and allocate private memory for mode dimensions
    context->set(GF_DS_ACNT, nAdapters);
    context->set(GF_DS_MCNT, nModes);
    context->set(GF_DS_PCNT, nPixelFormats);
    if (modeDim) delete [] modeDim;
    modeDim = new int[nAdapters * nModes * nPixelFormats][2];

    // enumerate and set all descriptions
    for (int id = 0; id < nAdapters; id++) {
        if (SUCCEEDED(d3d->GetAdapterIdentifier(id, 0, &d3di))) {
            rc = false;
            for (int ip = 0; ip < nPixelFormats; ip++) {
                // mode count
                int nr = d3d->GetAdapterModeCount(id, Format[ip]);
                nr = (nr > nModes) ? nModes : nr;
                for (int ir = 0; ir < nr; ir++) {
                    if (SUCCEEDED(d3d->EnumAdapterModes(id, Format[ip], ir, &mode))
                     && mode.Width >= WND_WIDTH && mode.Height >= WND_HEIGHT &&
                     (D3D_OK == d3d->CheckDeviceFormat(id, D3DDEVTYPE_HAL, mode.Format,
                     D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE,  D3DFMT_D32) ||   
                     D3D_OK == d3d->CheckDeviceFormat(id, D3DDEVTYPE_HAL, mode.Format,
                     D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16) ||
                     D3D_OK == d3d->CheckDeviceFormat(id, D3DDEVTYPE_REF, mode.Format,
                     D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32) ||
                     D3D_OK == d3d->CheckDeviceFormat(id, D3DDEVTYPE_REF, mode.Format,
                     D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16))) {
                        wchar_t hz[20] = L"";
                        if (mode.RefreshRate)
                            wsprintf(hz, L"(%d Hz)", mode.RefreshRate);
                        wsprintf(str, L"%dx%d %ls %ls bits", mode.Width, mode.Height,
                         hz, fmtdesc[ip]);
                        rc = true;
                        // store the description and dimensions
                        int i = (id * nModes + ir) * nPixelFormats + ip;
                        context->set(GF_MDEDESC,   i, str);
                        modeDim[i][0] = mode.Width;
                        modeDim[i][1] = mode.Height;
                    }
                }
            }
            if (rc) {
                // adapter description
		        strcpyFromMB(str, d3di.Description, MAX_DESC); 
                context->set(GF_ADPDESC, id, str);
            }
        }
    }

    return rc;
}

// configure sets the configuration for setup
//
void Display::configure() {

    display     = context->get(GF_DS_ADPT);
    mode        = context->get(GF_DS_MODE);
    pixel       = context->get(GF_DS_PIXL);
    runInWindow = context->get(GF_DS_RINW) == 1;
    if (runInWindow) {
        width  = WND_WIDTH;
        height = WND_HEIGHT;
    }
    else {
        int nModes  = context->get(GF_DS_MCNT);
        int nPixls  = context->get(GF_DS_PCNT);
        int i       = (display * nModes + mode) * nPixls + pixel;
        width       = modeDim[i][0];
        height      = modeDim[i][1];
    }
    context->set(GF_DS_WDTH, width);
    context->set(GF_DS_HGHT, height);
}

// setup creates the display device according to the context
// parameters and associates the device with the application window
// (HWND)hwnd
//
bool Display::setup(void* hwnd) {

    bool rc = false;

	// store address of main application window
    this->hwnd = (HWND)hwnd;

    // set the D3D presentation parameters
    UINT adapter;
	D3DFORMAT d3dFormat;
    ZeroMemory(&d3dpp, sizeof d3dpp);
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferCount = 1;
    d3dpp.EnableAutoDepthStencil = TRUE;
    D3DDISPLAYMODE d3ddm;
    if (!runInWindow) {
		D3DFORMAT Format[] = D3D_DOC_FORMATS;
		d3dFormat = Format[pixel];
        if (FAILED(d3d->EnumAdapterModes(display, d3dFormat, mode, &d3ddm))) {
            error(L"Display::10 Failed to get selected display mode");
            error(L"Display::11 Defaulting to windowed mode");
            runInWindow = true;
        }
        else {
            adapter = display;
            d3dpp.BackBufferWidth  = d3ddm.Width;
            d3dpp.BackBufferHeight = d3ddm.Height;
            d3dpp.BackBufferFormat = d3ddm.Format;
            d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;
            width  = d3ddm.Width;
            height = d3ddm.Height;
        }
    }
    if (runInWindow) {
        adapter = D3DADAPTER_DEFAULT;
		d3d->GetAdapterDisplayMode(adapter, &d3ddm);
		d3dFormat = d3ddm.Format;
        d3dpp.Windowed = TRUE;
		d3dpp.BackBufferWidth  = WND_WIDTH;
		d3dpp.BackBufferHeight = WND_HEIGHT;
		d3dpp.BackBufferFormat = d3dFormat;
        width  = WND_WIDTH;
        height = WND_HEIGHT;
    }
	aspect = (float) width / height;
    context->set(GF_FR_ASP, aspect);

    // find the best format for depth buffering and stenciling
    //
    D3DDEVTYPE devtype;
    if (D3D_OK == d3d->CheckDeviceFormat(adapter, D3DDEVTYPE_HAL,
     d3dFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32)) {
        d3dpp.AutoDepthStencilFormat = D3DFMT_D32; // depth buffer
        devtype = D3DDEVTYPE_HAL;                  // HAL device   
    }
    else if (D3D_OK == d3d->CheckDeviceFormat(adapter, D3DDEVTYPE_HAL, 
     d3dFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)) {
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;  // depth buffer
        devtype = D3DDEVTYPE_HAL;                   // HAL Device
    }
    // if the above attempts fail, use the REF (software emulation) device
    // with a 32-bit depth buffer rather than the HAL (hardware accelerated) 
	// device
    else if (D3D_OK == d3d->CheckDeviceFormat(adapter, D3DDEVTYPE_REF,
     d3dFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32)) {
        d3dpp.AutoDepthStencilFormat = D3DFMT_D32;   // depth buffer
        devtype = D3DDEVTYPE_REF;                    // REF Device
    }
    // if all else fails, use the REF (software emulation) with a 16-bit
    // depth buffer, hoping that it will work. (If it doesn't, we are out
    // of luck anyway.)
    else {
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;   // depth buffer
        devtype = D3DDEVTYPE_REF;                    // REF Device
    }

    // extract the device capabilities and configure the limits
    D3DCAPS9 caps;
    d3d->GetDeviceCaps(adapter, devtype, &caps);

	// hardware or software vertex processing?
	DWORD behaviorFlags;
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0)
	    behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
	    behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

    // retrieve the Interface to the D3D display device
    if (d3dd)
        error(L"Display::11 Pointer to Direct3D interface is not NULL");
    else if (FAILED(d3d->CreateDevice(adapter, devtype, (HWND)hwnd, 
	 behaviorFlags, &d3dpp, &d3dd)))
        error(L"Display::12 Failed to create Direct3D device");
    else {
        // maximum number of lights supported by the display device
		maxLights = caps.MaxActiveLights ? caps.MaxActiveLights : MAX_ACTIVE_LIGHTS;
        // set anisotropic filtering to the maximum available on the device
        if (FAILED(d3dd->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,
         caps.MaxAnisotropy - 1)))
            error(L"Display::17 Failed to set up anisotropic filtering");

		// create a sprite COM object to manage the drawing of the hud texture
		// and the drawing of the text item fonts
		if (!scrManager && FAILED(D3DXCreateSprite(d3dd, &scrManager)))
			error(L"Display::18 Failed to create the sprite manager");

		// setup successful
        APILight::connectsThrough(d3dd);
        d3dd->AddRef();
        APITexture::connectsThrough(d3dd);
        d3dd->AddRef();
        APIGraphic::connectsThrough(d3dd);
        d3dd->AddRef();
        APIText::connectsThrough(d3dd);
        d3dd->AddRef();
        APITexture::isManagedBy(scrManager);
        scrManager->AddRef();
        APIText::isManagedBy(scrManager);
        scrManager->AddRef();
        APITexture::hasClientArea(width, height);
        APIText::hasClientArea(width, height);
        rc = true;
    }

	// complete the setup
	if (rc) {
        setupProjection();
        setupLighting();
		setupBlending();
	}

    return rc;
}

// setupProjection calculates the transformation from camera space to
// homogeneous clip space and applies the transformation to the display
// device
//
void Display::setupProjection() {

    float  nearcp = context->get(GF_FR_NEAR);
    float  farcp  = context->get(GF_FR_FAR);
    float  fov    = context->get(GF_FR_FOV);
    float  aspect = context->get(GF_FR_ASP);

    projection = ::projection(fov, aspect, nearcp, farcp);

	if (d3dd)
		d3dd->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&projection);
}


// setupLighting sets the lighting parameters on the graphics card
//
void Display::setupLighting() {

    // allow specular highlights (can be slow on some machines)
    if (d3dd) d3dd->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

    APILight::alloc(maxLights);
}

// setupBlending sets up colour dithering and sets the formula for
// alpha blending
//
void Display::setupBlending() {

    if (d3dd) {
        // allow colour dithering (much smoother looking when using lights)
        d3dd->SetRenderState(D3DRS_DITHERENABLE, TRUE);

        // how alpha-blending is done (when drawing transparent things)
        d3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        d3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }
}

// restore re-activates the display device
//
bool Display::restore() {

    bool rc = false;

    if (d3dd) {
        HRESULT hr;
		hr = d3dd->TestCooperativeLevel();
		if (hr == D3DERR_DEVICENOTRESET)
			// reset the display device
			rc = d3dd->Reset(&d3dpp) == D3D_OK;
		else if (hr == S_OK)
			rc = true;
	}
	if (rc) {
		// reacquire sprite manager references to video memory
		if (scrManager) scrManager->OnResetDevice();
    }

	// complete the restoration
	if (rc) {
        setupProjection();
        setupLighting();
		setupBlending();
	}

    return rc;
}

// beginDrawFrame applies the view transformation, sets the ambient lighting,
// and clears the backbuffer
//
void Display::beginDrawFrame() {

    // construct the view transformation
    //
    Vector p = context->get(GF_CA_POSN);
    Vector h = context->get(GF_CA_HEAD);
    Vector u = context->get(GF_CA_UP  );
    Matrix view = ::view(p, p + h, u);
    if (d3dd) d3dd->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&view);

    // set global ambient light
    //
    Vector c = context->get(GF_LT_BKGD);
    if (d3dd) 
        d3dd->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(c.x, c.y, c.z, 1));

    // clear the backbuffer
    //
    if (d3dd) {
        d3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
         D3DCOLOR_XRGB(BGROUND_R, BGROUND_G, BGROUND_B), 1.0, 0);
        d3dd->BeginScene();
    }
}

// set turns on off the specified render state
//
void Display::set(RenderState state, bool b) {

    switch (state) {
        case ALPHA_BLEND:
            d3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, b);
            break;
        case Z_ENABLE:
             d3dd->SetRenderState(D3DRS_ZENABLE, b);
           break;
    }
}

// beginDrawObject initializes the pipeline for drawing an object
//
void Display::beginDrawObject(const Matrix& world) {

    if (d3dd) d3dd->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&world);
}

// set sets the material reflectivity for the drawing process
//
void Display::set(const Reflectivity& reflectivity) { 
    
    D3DMATERIAL9 mat;
    ZeroMemory(&mat, sizeof(mat));
    mat.Ambient   = D3DXCOLOR(reflectivity.ambient.r, reflectivity.ambient.g,
     reflectivity.ambient.b, reflectivity.ambient.a);
    mat.Diffuse   = D3DXCOLOR(reflectivity.diffuse.r, reflectivity.diffuse.g,
     reflectivity.diffuse.b, reflectivity.diffuse.a);
    mat.Specular  = D3DXCOLOR(reflectivity.specular.r, reflectivity.specular.g,
     reflectivity.specular.b, reflectivity.specular.a);
    mat.Power     = reflectivity.power; 
    d3dd->SetMaterial(&mat);
}

// beginDrawHUD ends the drawing of all text items
//
void Display::beginDraw(unsigned flags) {

    DWORD displayFlags = 0;

    if (flags & HUD_ALPHA) 
        displayFlags |= D3DXSPRITE_ALPHABLEND;

    scrManager->Begin(displayFlags);
}

// endDrawHUD ends the drawing of all text items
//
void Display::endDraw() {

    scrManager->End();
}

// endDrawFrame presents the backbuffer to the primary buffer
//
void Display::endDrawFrame() {

    // present the backbuffer to the primary buffer
    //
    if (d3dd) {
        d3dd->EndScene();
        if (FAILED(d3dd->Present(NULL, NULL, NULL, NULL)))
            error(L"Display::40 Failed to flip backbuffer");
    }
}

// resize resets the aspect ratio and recalculates the projection matrix
//
void Display::resize() {

	if (runInWindow) {
        int width  = context->get(GF_WN_WDTH);
        int height = context->get(GF_WN_HGHT);
        APIText::hasClientArea(width, height);

		// reset the aspect ratio
		aspect = float(width) / height;
        context->set(GF_FR_ASP, aspect);
		// reset projection transformation
		setupProjection();
	}
}

// suspend prepares the display device for de-activation
//
void Display::suspend() {

    // detach the sprite manager from video memory
    if (scrManager)
        scrManager->OnLostDevice();
}

// release releases the interfaces to the display device
//
void Display::release() {

    suspend();

    // release the font manager
    if (scrManager) {
        scrManager->Release();
        APIText::isManagedBy(NULL);
        scrManager->Release();
        APITexture::isManagedBy(NULL);
        scrManager->Release();
        scrManager = NULL;
    }
	// release the display device
    if (d3dd) {
        d3dd->Release();
        APILight::connectsThrough(NULL);
        d3dd->Release();
        APITexture::connectsThrough(NULL);
        d3dd->Release();
        APIText::connectsThrough(NULL);
        d3dd->Release();
        d3dd = NULL;
    }

    APILight::dealloc();
}

// destructor releases the display object along with the Interface to the
// reference object
//
Display::~Display() {

    release();

    if (d3d) {
        d3d->Release();
        d3d = NULL;
    }

    // mode dimensions memory
    if (modeDim) {
        delete [] modeDim;
        modeDim = NULL;
    }
}
