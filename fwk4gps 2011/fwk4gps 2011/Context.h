#ifndef _CONTEXT_H_
#define _CONTEXT_H_

/* Context Definition
 *
 * Context.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iContext.h"         // for the Context Interface
#include "iUtilities.h"       // for strcpy()
#include "Configuration.h"    // for MAX_DESC
#include "MathDeclarations.h" // for Vector

//-------------------------------- Context ------------------------------------
//
// Context holds the current context for the Framework 
//
class Context : public iContext {

    int     integer[MAX_INTEGERS];      // integer data
    float   real[MAX_FLOATS];           // float data
    Vector  vector[MAX_VECTORS];        // vector data 
    bool*   boolArray[MAX_BOOL_ARRAYS]; // pointers to bool arrays
    int*    intArray [MAX_INT_ARRAYS];  // pointers to integer arrays
    wchar_t (*strArray[MAX_STR_ARRAYS])[MAX_DESC + 1]; // to StrArray arrays

    Context();
    Context(const Context& src);            // prevents copying
    Context& operator=(const Context& src); // prevents assignments
    virtual ~Context();
    // memory allocations
    void   alloc(BoolArray id, int n);
    void   alloc(IntArray id, int n, const int*);
    void   alloc(StrArray id, int n, const wchar_t*[]);

  public:
	friend iContext* CreateContext();
	// settings
    void   set(Integer d, int i)                    { integer[d] = i; }
    void   set(Float f, float x)                    { real[f] = x; }
    void   set(BoolArray b, int k, bool s)          { boolArray[b][k] = s; }
    void   set(IntArray d, int i, int x)            { intArray[d][i] = x; }
    void   set(VectorId d, Vector v)                { vector[d] = v; }
    void   set(StrArray i, int m, const wchar_t* s) { strcpy(strArray[i][m], s, MAX_DESC); }
    void   set(Action a, int k)                     { intArray[GF_KEYCODES][a] = k; }
    // queries
    int    get(Integer d) const                     { return integer[d]; }
    float  get(Float f) const                       { return real[f]; }
    int    get(IntArray d, int i) const             { return intArray[d][i]; }
    const  wchar_t* get(StrArray s, int i) const    { return strArray[s][i]; }
    Vector get(VectorId d) const                    { return vector[d]; }
	int    get(Action a) const                      { return intArray[GF_KEYCODES][a]; }
    bool   pressed(Action a) const                  { return boolArray[GF_KB_KEYS][intArray[GF_KEYCODES][a]]; }
    bool   pressed(MouseButton b) const             { return boolArray[GF_MS_BTNS][b]; }
    bool   pressed(CntlrButton c) const             { return boolArray[GF_CT_BTNS][c]; }
    bool   pressed(XboxCntlrButton d) const         { return boolArray[GF_XB_BTNS][d]; }
    bool   pressed() const                          { return boolArray[GF_CT_BTNS][integer[GF_CT_TGRB]]; }
    // termination function
	void   Delete() const { delete this; }
};

#endif
