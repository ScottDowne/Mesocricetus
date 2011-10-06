/* Context Implementation
 *
 * Context.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Context.h" // for the Context class definition

//------------------------------- Context -------------------------------------
//
// The Context Object holds the current context for the Framework 
//
// CreateContext creates the Context Object
//
iContext* CreateContext() {

	return new Context();
}

// constructor initializes the instance variables and sets the default values
//
Context::Context() {

    // integers
    for (int i = 0; i < MAX_INTEGERS; i++)
        integer[i] = 0;

    // pointers to the bool arrays
    for (int b = 0; b < MAX_BOOL_ARRAYS; b++)
        boolArray[b] = NULL;

    // pointers to int arrays
    for (int i = 0; i < MAX_INT_ARRAYS; i++)
        intArray[i] = NULL;

    // pointers to string arrays
    for (int i = 0; i < MAX_STR_ARRAYS; i++)
        strArray[i] = NULL;

    // allocate context memory for key states
    alloc(GF_KB_KEYS, MAX_KEYS);

    // allocate context memory for mouse button states
    alloc(GF_MS_BTNS, MAX_M_BUTTONS);

    // allocate context memory for controller button states
    alloc(GF_CT_BTNS, MAX_C_BUTTONS);
    set(GF_CT_BCNT, MAX_C_BUTTONS);

    // allocate context memory for adapter descriptions
    alloc(GF_ADPDESC, MAX_ADAPTERS, NULL);

    // allocate context memory for mode descriptions
    alloc(GF_MDEDESC, MAX_ADAPTERS * MAX_MODES * MAX_P_FORMATS, NULL);

    // allocate context memory for controller descriptions
    alloc(GF_CTRDESC, MAX_CONTROLLERS, NULL);

    // allocate context memory for controller object descriptions
    alloc(GF_CTBDESC, MAX_CONTROLLERS * MAX_C_BUTTONS, NULL);

    // allocate context memory for mappable keys
	const wchar_t* mbleKey[] = KEY_DESCRIPTIONS;
	int nMappableKeys = sizeof mbleKey / sizeof(wchar_t*);
    alloc(GF_KEYDESC, nMappableKeys, mbleKey);
    set(GF_NO_KEYS, nMappableKeys);
    // allocate context memory for configurable actions
	const wchar_t* actDesc[] = ACTION_DESCRIPTIONS;
	int nActions = sizeof actDesc / sizeof(wchar_t*);
    alloc(GF_ACTDESC, nActions, actDesc);
    set(GF_NO_ACTS, nActions);
	// allocate context memory for key mappings
	int modelKey[] = ACTION_MAPPINGS;
    alloc(GF_KEYCODES, nActions, modelKey);

    // allocate context memory for the description of each configurable
	// sound that appears in the user dialog
	//
	// allocate context memory for the file name associated with each
	// configurable sound that appears in the user dialog
	//
	const wchar_t* sndDesc[] = SOUND_DESCRIPTIONS;
	const wchar_t* defFile[] = SOUND_MAPPINGS;
	int nSounds = sizeof sndDesc / sizeof (wchar_t*);
    alloc(GF_SNDDESC, nSounds, sndDesc);
    alloc(GF_SNDFILE, nSounds, defFile);
    set(GF_NO_SNDS, nSounds);
}

// alloc allocates memory for an array of n bools and initializes each bool
//
void Context::alloc(BoolArray id, int n) {

    if (boolArray[id])
        delete [] boolArray[id];
    boolArray[id] = new bool[n];
    for (int i = 0; i < n; i++)
        boolArray[id][i] = false;
}

// alloc allocates memory for an array of n ints and initializes each int
//
void Context::alloc(IntArray id, int n, const int* initial) {

    if (intArray[id])
        delete [] intArray[id];
    intArray[id] = new int[n];
    // initialize
    if (initial)
        for (int i = 0; i < n; i++)
            intArray[id][i] = initial[i];
    else
        for (int i = 0; i < n; i++)
            intArray[id][i] = 0;
}

// alloc allocates memory for an array of n strings and initializes each string
//
void Context::alloc(StrArray id, int n, const wchar_t* initial[]) {

    if (strArray[id])
        delete [] strArray[id];
    strArray[id] = new wchar_t[n][MAX_DESC + 1];
    if (initial)
        for (int i = 0; i < n; i++)
            strcpy(strArray[id][i], initial[i], MAX_DESC);
    else
        for (int i = 0; i < n; i++)
            strArray[id][i][0] = L'\0';
}

// destructor deletes the context memory
//
Context::~Context() {

    for (int b = 0; b < MAX_BOOL_ARRAYS; b++)
        if (boolArray[b])
            delete [] boolArray[b];

    for (int i = 0; i < MAX_INT_ARRAYS; i++)
        if (intArray[i])
            delete [] intArray[i];

    for (int i = 0; i < MAX_STR_ARRAYS; i++)
        if (strArray[i])
            delete [] strArray[i];
}

