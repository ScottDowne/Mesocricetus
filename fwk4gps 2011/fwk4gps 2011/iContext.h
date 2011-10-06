#ifndef _I_CONTEXT_H_
#define _I_CONTEXT_H_

/* Context Interface
 *
 * iContext.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iContext -----------------------------------
//
// iContext is the Interface to the Context Object
//
enum Integer;
enum Float;
enum VectorId;
enum BoolArray;
enum IntArray;
enum StrArray;
enum Action;
enum MouseButton;
enum CntlrButton;
struct Vector;

class iContext {
  public:
    // settings
    virtual void   set(BoolArray, int, bool)                          = 0;
    virtual void   set(Action, int)                                   = 0;
    virtual void   set(VectorId, Vector)                              = 0;
    virtual void   set(Integer, int)                                  = 0;
    virtual void   set(Float, float)                                  = 0;
    virtual void   set(IntArray, int, int)                            = 0;
    virtual void   set(StrArray, int, const wchar_t*)                 = 0;
    // queries
    virtual int    get(Integer) const                                 = 0;
    virtual float  get(Float) const                                   = 0;
    virtual int    get(IntArray, int) const                           = 0;
    virtual const  wchar_t* get(StrArray, int) const                  = 0;
    virtual Vector get(VectorId) const                                = 0;
	virtual int    get(Action) const                                  = 0;
    virtual bool   pressed(Action) const                              = 0;
    virtual bool   pressed(MouseButton) const                         = 0;
    virtual bool   pressed(CntlrButton) const                         = 0;
    virtual bool   pressed() const                                    = 0;
    // termination function
	virtual void   Delete() const                                     = 0;
};

extern "C"
iContext* CreateContext();

#endif
