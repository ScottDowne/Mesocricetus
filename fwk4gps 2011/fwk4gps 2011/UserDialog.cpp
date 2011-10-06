/* UserDialog Implementation - Window API Branch
 *
 * UserDialog.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "iEngine.h"        // for the Engine Interface
#include "iContext.h"       // for the Context Interface
#include "iUtilities.h"     // for logError()

#include "UserDialog.h"     // for UserDialog class definition
#include "WindowSettings.h" // for IDC_

BOOL CALLBACK dlgProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

//-------------------------------- UserDialog ---------------------------------
//
// The UserDialog object collects the configuration information from the user
//
iUserDialog* UserDialog::address = NULL;

// CreateUserDialog creates the UserDialog Object
//
iUserDialog* CreateUserDialog(void* h, iContext* c) {

    return UserDialog::address ? UserDialog::address : new UserDialog(h, c);
}

// constructor sets the default selections
//
UserDialog::UserDialog(void* h, iContext* c) : hinst(h), context(c) {

    address = this;

    // default settings
    strcpy(display,    RUN_IN_WINDOW_DESC, MAX_DESC);
    strcpy(resolution, L"", MAX_DESC);
    strcpy(controller, NO_CONTROLLER_DESC, MAX_DESC);
    strcpy(trigger,    L"", MAX_DESC);
    action   = 0;
    sound    = 0;
    flags    = 0;
    hwnd     = NULL;
}

// configureContext displays a DialogBox and gets the configuration
// information from the user
//
bool UserDialog::configureContext() {

    bool rc;

    rc = DialogBox((HINSTANCE)hinst, MAKEINTRESOURCE(IDD_DLG), NULL, 
     dlgProc) == TRUE;

    hwnd = NULL;

    return rc;
}

// populate the user dialog with the available configurations 
//
void UserDialog::populateUserDialog(void* hwnd) {

    // store window handle for error message box
    this->hwnd = hwnd;

    // populate the adapter combo box
    populateAdapterList(hwnd);
	// populate the action combo box
	populateActionList(hwnd);
    // populate the mappable key combo box
    populateMappableKeyList(hwnd);
	// populate the configurable sound file list
	populateSoundFileList(hwnd);
    // populate the controller combo box
    populateControllerList(hwnd);
}

// populateAdapterList populates the adapter combo box with a list of the
// installed display adapters
//
void UserDialog::populateAdapterList(void* hwndw) {

    int dev; // index of the current line item
    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the display adapter combo box
    SendDlgItemMessage(hwnd, IDC_DIS, CB_RESETCONTENT, 0, 0L);
    // retrieve the number of display adapters available
    int nd = context->get(GF_DS_ACNT);
    for (int id = 0; id < nd; id++) {
		// retrieve adapter id's description
        if (context->get(GF_ADPDESC, id)[0]) {
            // add the description and the index to adapter combo box
			dev = SendDlgItemMessage(hwnd, IDC_DIS, CB_ADDSTRING, 0,
             (LPARAM)context->get(GF_ADPDESC, id));
            SendDlgItemMessage(hwnd, IDC_DIS, CB_SETITEMDATA, dev, id);
        }
    }
    // append the option of running in a window
    dev = SendDlgItemMessage(hwnd, IDC_DIS, CB_ADDSTRING, 0,
     (LPARAM)RUN_IN_WINDOW_DESC);
    SendDlgItemMessage(hwnd, IDC_DIS, CB_SETITEMDATA, dev, 
     RUN_IN_WINDOW);

    // set the default display adapter to the previously selected adapter
	dev = SendDlgItemMessage(hwnd, IDC_DIS, CB_FINDSTRINGEXACT, -1,
        (LPARAM)display);
	if (dev == CB_ERR) dev = 0;
    SendDlgItemMessage(hwnd, IDC_DIS, CB_SETCURSEL, dev, 0L);
}

// populateActionList populates the action combo box with a list of
// the actions that can have their keys configured
//
void UserDialog::populateActionList(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the action combo box
	SendDlgItemMessage(hwnd, IDC_ACT, CB_RESETCONTENT, 0, 0L);
    // retrieve the number of predefined actions
    int noActions = context->get(GF_NO_ACTS);
    for (int a = 0; a < noActions; a++) {
        // add the description and key mapping to the action combo box
		int i = SendDlgItemMessage(hwnd, IDC_ACT, CB_ADDSTRING, 0, 
		 (LPARAM)context->get(GF_ACTDESC, a));
        SendDlgItemMessage(hwnd, IDC_ACT, CB_SETITEMDATA, i, 
		 (LPARAM)context->get((Action)a));
    }
    SendDlgItemMessage(hwnd, IDC_ACT, CB_SETCURSEL, action, 0L);
}

// populateMappableKeyList populates the mappable key combo box with a
// list of all of the configurable keys
//
void UserDialog::populateMappableKeyList(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the mappable key combo box
    SendDlgItemMessage(hwnd, IDC_KEY, CB_RESETCONTENT, 0, 0L);
    // retrieve the number of mappable keys
	int noKeys = context->get(GF_NO_KEYS);
    for (int i = 0; i < noKeys; i++)
        // add the key's description to the mappable key combo box
		SendDlgItemMessage(hwnd, IDC_KEY, CB_ADDSTRING, 0, 
		 (LPARAM)context->get(GF_KEYDESC, i));

    // retrieve the index for the selected action
    int f = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETCURSEL, 0, 0L);
	// find the current key for the selected action
	int d = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, f, 0L);
	if (d == CB_ERR) d = 0;
    // set the cursor to the current key for the selection action
    SendDlgItemMessage(hwnd, IDC_KEY, CB_SETCURSEL, d, 0L);

	// enable the mappable key combo box
	EnableWindow(GetDlgItem(hwnd, IDC_KEY), TRUE);
}

// populateSoundFileList populates the sound file combo box with a
// list of the currently stored sound files, populates the sound
// combo box with the configurable sound descriptions and ties each
// configurable sound to the previously selected sound file
//
void UserDialog::populateSoundFileList(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    WIN32_FIND_DATA ffd;
	wchar_t directory[MAX_DESC+1];
	HANDLE handle = INVALID_HANDLE_VALUE;
	unsigned length;
	bool keepsearching;

	// empty the available sound file combo box
	SendDlgItemMessage(hwnd, IDC_SFL, CB_RESETCONTENT, 0, 0L);
    // interrogate the audio directory for sound files
	length = wcslen(AUDIO_DIRECTORY);
	if (length > MAX_DESC - 3)
		error(L"UserDialog::10 Audio Directory name is too long");
	else {
		strcpy(directory, AUDIO_DIRECTORY, MAX_DESC);
		strcat(directory, L"\\*", MAX_DESC);
		handle = FindFirstFile(directory, &ffd);
		keepsearching = handle != INVALID_HANDLE_VALUE;
		bool foundAFile = false;
		while (keepsearching) {
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				if (wcslen(ffd.cFileName) < MAX_DESC) {
					// add the file name to the sound file combo box
					SendDlgItemMessage(hwnd, IDC_SFL, CB_ADDSTRING, 0, 
					 (LPARAM)ffd.cFileName);
					foundAFile = true;
				}
			}
			keepsearching = FindNextFile(handle, &ffd) != 0;
		}
		if (foundAFile) {
			SendDlgItemMessage(hwnd, IDC_SFL, CB_SETCURSEL, 0, 0L);
			// enable the sound file combo box
			EnableWindow(GetDlgItem(hwnd, IDC_SFL), TRUE);
		}
	}

    // empty the configurable sound combo box
	SendDlgItemMessage(hwnd, IDC_AUD, CB_RESETCONTENT, 0, 0L);

    // populate the configurable sound combo box with a list of 
	// configurable sounds
    //
	int noSounds = context->get(GF_NO_SNDS);
    for (int msnd = 0; msnd < noSounds; msnd++) {
        // add the sound description to the configurable sound combo box
		SendDlgItemMessage(hwnd, IDC_AUD, CB_ADDSTRING, 0, 
		 (LPARAM)context->get(GF_SNDDESC, msnd));
		// find the previously selected match in the sound file list and
		// store the sound file item index in the sound list data item
		// default to the top of the list, if match not found
		int i = 0;
		bool found = false;
		const wchar_t* storedFilename = context->get(GF_SNDFILE, msnd);
		int nf = SendDlgItemMessage(hwnd, IDC_SFL, CB_GETCOUNT, 0, 0L);
		wchar_t availableFile[MAX_DESC + 1] = L"";
		for (int file = 0; file < nf && !found; file++) {
			SendDlgItemMessage(hwnd, IDC_SFL, CB_GETLBTEXT, file, 
			 (LPARAM)availableFile);
			if (!wcscmp(availableFile, storedFilename)) {
				found = true;
				i = file;
			}
		}
		SendDlgItemMessage(hwnd, IDC_AUD, CB_SETITEMDATA, msnd, i);
    }

	// set the cursor to the previously selected sound file
    SendDlgItemMessage(hwnd, IDC_AUD, CB_SETCURSEL, sound, 0L);
    int file = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETITEMDATA, 
     sound, 0L);
    SendDlgItemMessage(hwnd, IDC_SFL, CB_SETCURSEL, file, 0L);
}

// populateControllerList populates the controller combo box with a list of
// the attached controllers
//
void UserDialog::populateControllerList(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the controller combo box
	SendDlgItemMessage(hwnd, IDC_CNT, CB_RESETCONTENT, 0, 0L);
    // start the list with a no controller option
    int ctr = SendDlgItemMessage(hwnd, IDC_CNT, CB_ADDSTRING, 0, 
     (LPARAM)NO_CONTROLLER_DESC);
	// the data item for the no controller line item is NO_CONTROLLER
    SendDlgItemMessage(hwnd, IDC_CNT, CB_SETITEMDATA, ctr, NO_CONTROLLER);
    // assume no controller selected - correct this assumption if necessary
    SendDlgItemMessage(hwnd, IDC_CNT, CB_SETCURSEL, ctr, 0L);
    // uncheck the 'reverse y axis' checkbox
    SendDlgItemMessage(hwnd, IDC_YRV, BM_SETCHECK, BST_UNCHECKED, 0L);
    // uncheck the 'use z axis' checkbox
    SendDlgItemMessage(hwnd, IDC_ZON, BM_SETCHECK, BST_UNCHECKED, 0L);
    // disable the trigger and the check boxes
    EnableWindow(GetDlgItem(hwnd, IDC_TRG), FALSE);
    EnableWindow(GetDlgItem(hwnd, IDC_YRV), FALSE);
    EnableWindow(GetDlgItem(hwnd, IDC_ZON), FALSE);

    // populate the controller combo box
	int nc = context->get(GF_CT_CCNT);
    bool atLeastOne = false;
	for (int ic = 0; ic < nc; ic++) {
        if (context->get(GF_CTRDESC, ic)[0]) {
            // store the controller description in the address part
		    ctr = SendDlgItemMessage(hwnd, IDC_CNT, CB_ADDSTRING, 0, 
		     (LPARAM)context->get(GF_CTRDESC, ic));
            // store the controller index in the data part
		    SendDlgItemMessage(hwnd, IDC_CNT, CB_SETITEMDATA, ctr, 
		     (LPARAM)ic);
            atLeastOne = true;
        }
	}
    // if at least one controller is listed, correct the default choices
    if (atLeastOne) {
		// set the visible line item to the previously selected one
		ctr = SendDlgItemMessage(hwnd, IDC_CNT, CB_FINDSTRINGEXACT, -1,
         (LPARAM)controller);
		if (ctr == CB_ERR) 
            ctr = 0;
		SendDlgItemMessage(hwnd, IDC_CNT, CB_SETCURSEL, ctr, 0);
        // retrieve the index of the controller for the default line item
        int ic = SendDlgItemMessage(hwnd, IDC_CNT, CB_GETITEMDATA, ctr, 0);
        // if choice is an attached controller
        if (ic != NO_CONTROLLER) {
            // populate the trigger selection list
            populateControllerObjectList(hwnd);
            // retrieve previous checkbox settings
            bool zAOn = !!(flags & 2);
            bool yRev = !!(flags & 4);
            // set the 'reverse y axis' checkbox to the previous setting
            SendDlgItemMessage(hwnd, IDC_YRV, BM_SETCHECK, yRev ? 
             BST_CHECKED : BST_UNCHECKED, 0L);
            // set the 'use z axis' checkbox to the previous setting
            SendDlgItemMessage(hwnd, IDC_ZON, BM_SETCHECK, zAOn ?
             BST_CHECKED : BST_UNCHECKED, 0L);
            // enable the trigger combo box and the check boxes
            EnableWindow(GetDlgItem(hwnd, IDC_YRV), TRUE);
            EnableWindow(GetDlgItem(hwnd, IDC_ZON), TRUE);
        }
    }
}

// populateAdapterModeList populates the resolution combo box with the modes
// available on the selected display adapter
//
bool UserDialog::populateAdapterModeList(void* hwndw) {

    bool rc = false;
    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the resolutions combo box
    SendDlgItemMessage(hwnd, IDC_RES, CB_RESETCONTENT, 0, 0L);
    // retrieve the index for the selected adapter
    int dev = SendDlgItemMessage(hwnd, IDC_DIS, CB_GETCURSEL, 0, 0L);
    if (dev == CB_ERR)
        error(L"UserDialog::30 No adapter selected");
    else {
        // retrieve the id of the selected adapter
        int id = SendDlgItemMessage(hwnd, IDC_DIS, CB_GETITEMDATA, dev, 0L);
        if (id == RUN_IN_WINDOW) {
			// if window mode, disable the resolutions combo box
            EnableWindow(GetDlgItem(hwnd, IDC_RES), FALSE);
            rc = true;
        }
        else {
            // fullscreen modes
            // populate the resolutions combo box with the available modes
            bool noModes = true; // assume no modes available
            // retrieve the number of supported pixel formats
            int np = context->get(GF_DS_PCNT);
            int nr = context->get(GF_DS_MCNT);
            for (int ip = 0; ip < np; ip++) { // for each pixel format...
                for (int ir = 0; ir < nr; ir++) { // for each mode...
                    int i = (id * nr + ir) * np + ip;
                    if (context->get(GF_MDEDESC, i)[0]) {
                        // mode is supported - add it to the combo box
                        int res = SendDlgItemMessage(hwnd, IDC_RES,
                         CB_ADDSTRING, 0, 
                         (LPARAM)context->get(GF_MDEDESC, i));
                        // store mode|format in the data part of line item
						unsigned fmMd = (ip << 16) | ir;
                        SendDlgItemMessage(hwnd, IDC_RES, CB_SETITEMDATA, 
						 res, fmMd);
						// at least one mode has been found
                        noModes = false;
                    }
                }
            }
            if (noModes)
                error(L"UserDialog::31 Selected display has no available modes");
			else {
                // set the cursor to the previously selected resolution
				int res = SendDlgItemMessage(hwnd, IDC_RES, CB_FINDSTRINGEXACT,  
				 -1, (LPARAM)resolution);
				if (res == CB_ERR) res = 0;
				SendDlgItemMessage(hwnd, IDC_RES, CB_SETCURSEL, res, 0L);

				// enable the resolutions combo box
				EnableWindow(GetDlgItem(hwnd, IDC_RES), TRUE);
				rc = true;
			}
        }
    }

    return rc;
}

// populateControllerObjectList populates the trigger combo box with a list
// of the buttons that are available on the selected controller
//
void UserDialog::populateControllerObjectList(void* hwndw) {

    int ctr; // line item of selected controller
    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the controller objects combo box
    SendDlgItemMessage(hwnd, IDC_TRG, CB_RESETCONTENT, 0, 0L);
    // assume no trigger selection possible - correct if necessary
    EnableWindow(GetDlgItem(hwnd, IDC_TRG), FALSE);
    // retrieve the line item of the selected controller
    ctr = SendDlgItemMessage(hwnd, IDC_CNT, CB_GETCURSEL, 0, 0L);
    if (ctr != CB_ERR) {
        // retrieve the index of the selected controller
        int ic = SendDlgItemMessage(hwnd, IDC_CNT, CB_GETITEMDATA, ctr, 0);
        if (ic != NO_CONTROLLER) {
		    // examine each button on the controller
		    bool found = false;
		    int nb = context->get(GF_CT_BCNT); 
            for (int b = 0; b < nb; b++) {
			    if (context->get(GF_CTBDESC, b)[0]) {
                    // add the button's name to the trigger combo list
                    int trg = SendDlgItemMessage(hwnd, IDC_TRG, CB_ADDSTRING, 
                     0, (LPARAM)context->get(GF_CTBDESC, b));
                    // save the button number in the data area of line item
                    SendDlgItemMessage(hwnd, IDC_TRG, CB_SETITEMDATA, trg, b);
				    // found at least one button
				    found = true;
                }
            }
            // if buttons exist, set the default button to previously selected one
		    if (found) {
                int trg = 0;
			    // determine the default trigger button 
			    if (!wcscmp(context->get(GF_CTRDESC, ic), controller)) {
				    // set the default to the previously selected trigger
				    trg = SendDlgItemMessage(hwnd, IDC_TRG, CB_FINDSTRINGEXACT, 
				     -1, (LPARAM)trigger);
				    if (trg == CB_ERR) trg = 0;
			    }
			    SendDlgItemMessage(hwnd, IDC_TRG, CB_SETCURSEL, trg, 0L);
                // enable the trigger selection and check boxes
                EnableWindow(GetDlgItem(hwnd, IDC_TRG), TRUE);
            }
            EnableWindow(GetDlgItem(hwnd, IDC_YRV), TRUE);
            EnableWindow(GetDlgItem(hwnd, IDC_ZON), TRUE);
        }
    }
}

// showActionMapping shows the mapping of the selected action
//
void UserDialog::showActionMapping(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // retrieve the index for the selected action
    int a = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETCURSEL, 0, 0L);
	if (a == CB_ERR)
        error(L"UserDialog::41 Action selection failed");
	else {
		// retrieve the index of the key for the selected action
		int k = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, a, 0L);
		// store the key's index in the data item of the selected action
		SendDlgItemMessage(hwnd, IDC_KEY, CB_SETCURSEL, k, 0L);
	}
}

// updateActionMapping updates the mapping of the selected key onto the
// selected configurable action
//
void UserDialog::updateActionMapping(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // retrieve the index for the selected action
    int a = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETCURSEL, 0, 0L);
	// retrieve the index for the selected key
	int k = SendDlgItemMessage(hwnd, IDC_KEY, CB_GETCURSEL, 0, 0L);

	if (a != CB_ERR && k != CB_ERR)
		// store selected key in the data item of the selected action
		SendDlgItemMessage(hwnd, IDC_ACT, CB_SETITEMDATA, a, k);
}

// showSoundMapping shows the mapping of the selected sound
//
void UserDialog::showSoundMapping(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // retrieve the index for the selected sound
    int s = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETCURSEL, 0, 0L);
	if (s == CB_ERR)
        error(L"UserDialog::42 Sound selection failed");
	else {
		// retrieve the index of the file linked to the selected sound
		int f = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETITEMDATA, s, 0L);
		// show the file currently linked to the selected sound
		SendDlgItemMessage(hwnd, IDC_SFL, CB_SETCURSEL, f, 0L);
	}
}

// updateSoundMapping updates the mapping of the selected sound file
// onto the selected configurable sound
//
void UserDialog::updateSoundMapping(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // retrieve the index for the selected sound
    int s = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETCURSEL, 0, 0L);
	// retrieve the index of the selected sound file
	int f = SendDlgItemMessage(hwnd, IDC_SFL, CB_GETCURSEL, 0, 0L);

	if (s != CB_ERR && f != CB_ERR)
		// store the index of the selected sound file in the data item 
		// of the selected sound
		SendDlgItemMessage(hwnd, IDC_AUD, CB_SETITEMDATA, s, f);
}

// saveUserChoices configures the context for the user's selections 
//
bool UserDialog::saveUserChoices(void* hwndw) {

    bool rcd = false, rcc = false;
    HWND hwnd = (HWND)hwndw; // handle to current window

    //----- adapter device properties -----------------------------------------

    // Retrieve the index for the selected display device
    int dev = SendDlgItemMessage(hwnd, IDC_DIS, CB_GETCURSEL, 0, 0L);
    if (dev == CB_ERR)
        error(L"UserDialog::50 No display adapter selected");
    else {
        int  res    = 0; // selected resolution line item
		int  ir     = 0; // selected resolution mode
		int  ip     = 0; // selected pixel format
        // retrieve adapter id for the selected display device line item
        int  id = SendDlgItemMessage(hwnd, IDC_DIS, CB_GETITEMDATA, dev, 0L);
		bool inWndw = id == RUN_IN_WINDOW;
        if (!inWndw) {
            // retrieve the index for the selected resolution
            res = SendDlgItemMessage(hwnd, IDC_RES, CB_GETCURSEL, 0, 0L);
            if (res == CB_ERR) {
                error(L"UserDialog::51 Resolution selection failed");
                // revert to run in a window
                inWndw = true;
                id     = 0;
            }
            else {
				// get the data item of the selected resolution line item
                unsigned fmMd = SendDlgItemMessage(hwnd, IDC_RES,
                 CB_GETITEMDATA, res, 0L);
                // extract the mode and format from the data item
				ir = fmMd & 0xFFFF;
				ip = (fmMd >> 16);
            }
        }
        else
            id = 0;
        // store the selected display, mode, and pixel format
        context->set(GF_DS_ADPT, id);
        context->set(GF_DS_MODE, ir);
        context->set(GF_DS_PIXL, ip);
        context->set(GF_DS_RINW, inWndw ? 1 : 0);
		// store display and resolution selection strings for future reference
		SendDlgItemMessage(hwnd, IDC_DIS, CB_GETLBTEXT, dev, (LPARAM)display);
		if (!inWndw)
            SendDlgItemMessage(hwnd, IDC_RES, CB_GETLBTEXT, res, (LPARAM)resolution);

		rcd = true;
    }

	//----- key mappings for coordinator actions ------------------------------------

	// define the key mappings for the configurable actions
	Key k;
	int noActions = context->get(GF_NO_ACTS); 
	for (int a = 0; a < noActions; a++) {
		// extract the key mapping from the data parameter of the line item
		k = (Key)SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, a, 0L);
		context->set((Action)a, k);
	}

	// store the index of the currently visible action for future initialization
    action = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETCURSEL, 0, 0L);

	//----- sound mappings -------------------------------------------------------

	// define the files associated with the configurable sounds
	int i, noSounds = context->get(GF_NO_SNDS); 
	wchar_t f[MAX_DESC + 1];
	for (int s = 0; s < noSounds; s++) {
		i = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETITEMDATA, s, 0L);
		// extract the filename from the string parameter of the line item
		SendDlgItemMessage(hwnd, IDC_SFL, CB_GETLBTEXT, i, (LPARAM)f);
		context->set(GF_SNDFILE, s, f);
	}

	// store the index of the currently visible sound for future initialization
    sound = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETCURSEL, 0, 0L);

    //----- controller properties ---------------------------------------------

    // retrieve the user's selections
    int ctr = SendDlgItemMessage(hwnd, IDC_CNT, CB_GETCURSEL, 0, 0L);
    if (ctr == CB_ERR)
        error(L"UserDialog::54 Controller selection failed");
    else {
		int  trg, b = 0;
		bool y, z, none;
        // retrieve the index of the selected controller
        int ic = SendDlgItemMessage(hwnd, IDC_CNT, CB_GETITEMDATA, ctr, 0L);
        none = ic == NO_CONTROLLER;
		if (!none) {
			// retrieve the selected trigger button
			trg = SendDlgItemMessage(hwnd, IDC_TRG, CB_GETCURSEL, 0, 0L);
			if (trg == CB_ERR) {
				error(L"UserDialog::55 Trigger selection failed");
                trg = 0;
                b = 0;
            }
			else
				b = SendDlgItemMessage(hwnd, IDC_TRG, CB_GETITEMDATA, trg, 0);
            // retrieve the selected y axis direction
            y = SendDlgItemMessage(hwnd, IDC_YRV, BM_GETCHECK, 0, 0)
             == BST_CHECKED;
            // retrieve the selected z axis usage
            z = SendDlgItemMessage(hwnd, IDC_ZON, BM_GETCHECK, 0, 0)
             == BST_CHECKED;
		}
        else {
            b  = trg = 0;
            y  = z   = false;
            ic = 0;
        }

		// (re-)define the controller aspect of the context
        flags = ((y ? 1 : 0) << 2) | ((z ? 1 : 0) << 1) | (none ? 1 : 0);
        context->set(GF_CT_INDX, ic);
        context->set(GF_CT_TGRB, b);
        context->set(GF_CT_FLGS, flags);
        // store descriptions for future initialization
		SendDlgItemMessage(hwnd, IDC_CNT, CB_GETLBTEXT, ctr, (LPARAM)controller);
		SendDlgItemMessage(hwnd, IDC_TRG, CB_GETLBTEXT, trg, (LPARAM)trigger);

		rcc = true;
    }

    return rcd && rcc;
}

// error displays a message box with message msg and write the message
// to the log file
//
void UserDialog::error(const wchar_t* msg) const {

    if (hwnd) 
        MessageBox((HWND)hwnd, msg, L"Dialog Error", MB_OK);
    logError(msg);
}

// dlgProc handles each message "msg" to the Dialog Box window "hwnd"
//
BOOL CALLBACK dlgProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

    BOOL         rc         = FALSE;
    static bool  firsttime  = true;
    iEngine*     engine     = EngineAddress();
    iUserDialog* userDialog = UserDialogAddress();

    // Process message msg
    switch (msg) {
      case WM_INITDIALOG:
        // make this dialog window a layered window 
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) 
         | WS_EX_LAYERED);
        // make this dialog window 5% transparent
        SetLayeredWindowAttributes(hwnd, 0, (255 * 95) / 100, LWA_ALPHA);
        // interrogate the hardware
        engine->getConfigurations(hwnd);
        // populate dialog
        userDialog->populateUserDialog(hwnd);
		rc = true;
        break;

      case WM_COMMAND:          // user accessed a dialog box control
        switch (LOWORD(wp)) {   // which control?
          case IDC_DIS:     // accessed the display combo box
            // only process this if it is the first time or the user
            // has changed their selection.  This block resets the static
            // variable "firsttime".  Exiting resets the variable to true
            if (firsttime && HIWORD(wp) == CBN_SETFOCUS ||
             HIWORD(wp) == CBN_SELCHANGE ) {
                firsttime = false;  // won't be 1st time again for this box!
                // populate resolutions combo box for the selected adapter
                if (rc = userDialog->populateAdapterModeList(hwnd))
                    // safe to activate GO button now
                    EnableWindow(GetDlgItem(hwnd, IDC_GO), TRUE);
            } else
                rc = TRUE;
            break;

          case IDC_ACT:  // user accessed the action combo box
            // show the current mappable key for the selected action
            userDialog->showActionMapping(hwnd);
            break;

          case IDC_KEY:  // user accessed the mappable key combo box
            if (HIWORD(wp) == CBN_SELCHANGE)
                // associate the selected action with the selected key
                userDialog->updateActionMapping(hwnd);
            break;

          case IDC_AUD:  // user accessed the audio combo box
            // show the current mappable key for the selected action
            userDialog->showSoundMapping(hwnd);
            break;

          case IDC_SFL:  // user accessed the mappable file combo box
            if (HIWORD(wp) == CBN_SELCHANGE)
                // associate the selected action with the selected key
                userDialog->updateSoundMapping(hwnd);
            break;

          case IDC_CNT:  // user accessed the controller combo box
            if (HIWORD(wp) == CBN_SELCHANGE)
                // populate the objects combo box for the selected controller
                userDialog->populateControllerObjectList(hwnd);
            break;

          case IDC_GO:  // user pressed the Go button
            // save the context information
            if (userDialog->saveUserChoices(hwnd)) {
                EndDialog(hwnd, TRUE);
                firsttime = true;
                rc = TRUE;
            }
            break;

          case IDCANCEL:  // user pressed Cancel button, or Esc, etc.
            EndDialog(hwnd, FALSE);
            firsttime = true;
            rc = TRUE;
            break;
        }
        break;
    }

    return rc;
}

