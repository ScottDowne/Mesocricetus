/* Window Implementation - Window API Branch
 *
 * Window.cpp
 * fwk4gps version 2.0
 * gam666/dps901
  * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>       // for timeGetTime

#include "iEngine.h"        // for the Engine Interface
#include "iContext.h"       // for the Context Interface
#include "iUtilities.h"     // for error()

#include "Window.h"         // for the Window class definition
#include "WindowSettings.h" // for WND_WIDTH, WND_HEIGHT, RUN_IN_WINDOW

bool registerWindowClass(HINSTANCE hinst);
LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
const wchar_t* itowc(wchar_t* s, int a);

//------------------------------ Window ---------------------------------------
//
// The Window class manages the main application window
//
iWindow* Window::address = 0;

// CreateMainWindow creates the Window Object
//
iWindow* CreateMainWindow(void* h, int s, iContext* c) {

	return Window::address ? Window::address : new Window(h, s, c);
}

// constructor initializes the instance variables and registers the window
// class for the application
//
Window::Window(void* h, int s, iContext* c) : hinst(h), show(s), 
 context(c), hwnd(NULL) {

     address = this;

     width  = WND_WIDTH;
     height = WND_HEIGHT;

	::registerWindowClass((HINSTANCE)hinst);
}

// release destroys the main application window
//
void Window::release() {

	// destroy the current application window, if one exists
	if (hwnd) {
		DestroyWindow((HWND)hwnd);
		show = SW_SHOW;
        hwnd = NULL;
	}
}

// create (re)creates the main application window
//
void* Window::create() {

    // window creation parameters
    if (context->get(GF_DS_RINW)) {
		// obtain width and height including window frame + titleBar
        RECT rect;
        rect.left   = 0;
        rect.top    = 0;
        rect.right  = width;  // desired width of client area
        rect.bottom = height; // desired height of client area
        AdjustWindowRectEx(&rect, wndStyle, FALSE, wndExStyle);
        width       = rect.right  - rect.left;
        height      = rect.bottom - rect.top;
		// set the window styles
		wndStyle    = WND_STYLE_W;
		wndExStyle  = WND_EXSTYLE_W;
    } else {
        // retrieve window dimensions from context
        width  = context->get(GF_DS_WDTH);
        height = context->get(GF_DS_HGHT);
        // set the window styles
		wndStyle   = WND_STYLE;
		wndExStyle = WND_EXSTYLE;
    }
	// re-define the context for the Window object
    context->set(GF_WN_WDTH, width);
    context->set(GF_WN_HGHT, height);
	// (re)create the main application window
	if (!(hwnd = CreateWindowEx(wndExStyle, CLASS_NAME, WND_NAME,
	 wndStyle, 0, 0, width, height, NULL, NULL, (HINSTANCE)hinst, NULL)))
		error(L"Window::10 Failed to create application window");
	else {
		// how to display the window
		ShowWindow((HWND)hwnd, show);
		// send the first WM_PAINT message to the window
		UpdateWindow((HWND)hwnd);
	}

    return hwnd;
}

// resize resets the dimensions of the client area to reflect the new size 
// of the area and saves the width and height of the resized window 
//
void Window::resize() const {

	// find the new size of the client area
	RECT rect;
	GetClientRect((HWND)hwnd, &rect);
	AdjustWindowRectEx(&rect, wndStyle, FALSE, wndExStyle);
	int width  = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	// check constraints
	if (width == 0) width = 1;
	if (height == 0) height = 1;

    // store the new dimensions of the application window
    context->set(GF_WN_WDTH, width);
    context->set(GF_WN_HGHT, height);
}

// getMessage checks extracts a message from the message queue and
// processes that message if one exists
// if the message is to quit, sets rc and keepgoing and returns them
//
bool Window::getMessage(int& rc, bool& keepgoing) const {

	bool retrievedMessage = false;

	MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            rc = msg.wParam;
            keepgoing = false;
        }
        else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		retrievedMessage = true;
    }

	return retrievedMessage;
}

// wait instructs the CPU to wait for the next message and
// allow the processing of other applications while waiting
//
void Window::wait() const {

	WaitMessage();
}

// moveToForeground moves hwnd to the foreground
//
void Window::moveToForeground() const {

	if (hwnd)
		PostMessage((HWND)hwnd, WM_ACTIVATEAPP, 1, 0);
}

// messageBox pops up a MessageBox in window hwnd with the message str
//
void Window::messageBox(const wchar_t* str) const {

    if (hwnd)
        MessageBox((HWND)hwnd, str, L"Error", MB_OK);
}

// time returns the current system time
//
int Window::time() const { 
    return timeGetTime();
}

// registerWindowClass registers the class for the main application window
//
bool registerWindowClass(HINSTANCE hinst) {

    // Define the "class" (type) of window we want to have
    // - a necessary step before we can create the main window

    WNDCLASS wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ::wndProc; // this function will be called...
    wc.cbClsExtra = 0;           // ...to handle messages
    wc.cbWndExtra = 0;
    wc.hInstance = hinst;
    wc.hIcon = LoadIcon(hinst, IDI_APPLICATION);
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;

    // Register the "class" for all subsequent calls to create
    // the main application window
    //
    return RegisterClass(&wc) != 0;
}

// wndProc handles the message "msg" sent to the main application window "hwnd"
//
LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

    static bool quit   = false;
    iEngine*    engine = EngineAddress();
    iWindow*    window = WindowAddress();

    switch (msg) {
      case WM_CREATE:    // called once when the window is first created
      case WM_SETCURSOR: // called whenever the mouse is moved to ...
        SetCursor(NULL); // ...lets us change the cursor if we want to
        break;

      // Sent when the user switches applications.  If focus is about to
	  // be regained, we need to restore the Framework; if focus is about to 
	  // be lost, we need to suspend the Framework
      case WM_ACTIVATEAPP:
        // window is about to be activated
		if (wp)
			engine->restore();
        // window is about to be de-activated
        else
            engine->suspend();
        break;

      case WM_KEYDOWN:
        switch (wp) {
          case VK_ESCAPE: 
            // close the main window and ...
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            // prepare to quit
            quit = true;
            break;

          case VK_F1:
            // select the new context 
			// - deactivate and post a user message
            PostMessage(hwnd, WM_ACTIVATEAPP, 0, 0);
            PostMessage(hwnd, WM_USER, 0, 0);
            break;
		}
		break;

      case WM_USER:
        engine->reconfigure();
        break;

      case WM_SIZE:
        // resize the current window
        if (engine->isActive()) {
            window->resize();
            engine->resize();
        }
        break;

	  case WM_CLOSE:
	    quit = true;
      case WM_DESTROY:
        // we need to respond to WM_DESTROY for the main window to shutdown properly
        if (quit)
            PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wp, lp);
}

// destructor destroys the main application window
//
Window::~Window() {

	if (hwnd)
        DestroyWindow((HWND)hwnd);
}

// itowc converts a into a wide character string
//
const wchar_t* itowc(wchar_t* s, int a) {

    wsprintf(s, L"%d", a); 
    return s;
}

