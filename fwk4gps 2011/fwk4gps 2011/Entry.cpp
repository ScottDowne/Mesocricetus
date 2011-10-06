/* An Open Instructional Framework for Game Programming Students
 *
 * Entry.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>   // for WinMain and Windows Types

#include "Framework.h" // for the Framework Definition

// Entry point for the Application
//
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hprev, LPSTR cp, int show) {

    Framework framework(hinst, show);

    return framework.run();
}
