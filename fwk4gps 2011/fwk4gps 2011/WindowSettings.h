#ifndef _WINDOW_SETTINGS_H_
#define _WINDOW_SETTINGS_H_

/* Header for Configuring the Window Branch
 *
 * WindowSettings.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Configuration.h" // framework-wide configuration settings
#include "ModelSystem.h"   // for MODEL_Z_AXIS, NEAR_TO_FAR, FAR_TO_NEAR

// shared with dialog resource (follows windows conventions)
//
#define IDD_DLG 101
#define IDC_DIS 102
#define IDC_RES 103
#define IDC_GO  104
#define IDC_KEY 105
#define IDC_ACT 106
#define IDC_CNT 107
#define IDC_TRG 108
#define IDC_YRV 109
#define IDC_ZON 110
#define IDC_AUD 111
#define IDC_SFL 112

// Windowing parameters
//
#define CLASS_NAME    L"fwk4gps"
#define RUN_IN_WINDOW -1
#define NO_CONTROLLER -1

// Window captions
//
#if Z_AXIS == NEAR_TO_FAR
#define WND_NAME L"fwk4gps (GAM666/DPS901 - Z Axis Near to Far)"
#elif Z_AXIS == FAR_TO_NEAR
#define WND_NAME L"fwk4gps (GAM666/DPS901 - Z Axis Far to Near)"
#endif
#define DLG_NAME L"Configure fwk4gps - GAM666/DPS901"

// Window styles
//
#define WND_STYLE_W   WS_OVERLAPPEDWINDOW
#define WND_EXSTYLE_W 0
#define WND_STYLE     WS_POPUP
#define WND_EXSTYLE   WS_EX_TOPMOST

// Run in a window combo box line item descriptions
#define RUN_IN_WINDOW_DESC L"Run in a window"
#define RESOLUTION_DESC    L""

#endif
