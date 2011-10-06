#ifndef _USER_DIALOG_H_
#define _USER_DIALOG_H_

/* UserDialog Definition - Window API Branch
 *
 * UserDialog.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iUserDialog.h"   // for the UserDialog Interface
#include "Configuration.h" // for MAX_DESC

//-------------------------------- UserDialog ---------------------------------
//
// The UserDialog class collects the configuration information from the user
//
class UserDialog : public iUserDialog {

    static iUserDialog* address; // points to this object

    void*      hinst;            // points to the application Instance
    iContext*  context;          // points to the Context object
    void*      hwnd;             // points to the Dialog Window

    // most recent configuration
    wchar_t display   [MAX_DESC + 1]; 
    wchar_t resolution[MAX_DESC + 1];
    wchar_t controller[MAX_DESC + 1];
    wchar_t trigger   [MAX_DESC + 1];
    int     action;
    int     sound;
    int     flags;

    UserDialog(void*, iContext* c);
	UserDialog(const UserDialog&);            // prevent copies
	UserDialog& operator=(const UserDialog&); // prevent assignments

  public:
    friend iUserDialog* CreateUserDialog(void*, iContext*);
    static iUserDialog* Address() { return address; }
    bool   configureContext();
	// initialization
    void   populateUserDialog(void*);
    void   populateAdapterList(void*);
    void   populateActionList(void*);
	void   populateSoundFileList(void*);
    void   populateControllerList(void*);
	void   populateMappableKeyList(void*);
	// execution
    bool   populateAdapterModeList(void*);
    void   populateControllerObjectList(void*);
	void   showActionMapping(void*);
	void   updateActionMapping(void*);
	void   showSoundMapping(void*);
	void   updateSoundMapping(void*);
    bool   saveUserChoices(void*);
    void   error(const wchar_t*) const;
	// termination function
	void   Delete() const { delete this; }
};

iUserDialog* UserDialogAddress() { return UserDialog::Address(); }

#endif