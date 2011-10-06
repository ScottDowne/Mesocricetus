#ifndef _I_DISPLAY_H_
#define _I_DISPLAY_H_

/* Display Interface - Direct API Branch
 *
 * iDisplay.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iDisplay -----------------------------------
//
// iDisplay is the Interface to the Display object
//
class iContext;
struct Matrix;
struct Reflectivity;
enum RenderState;

class iDisplay {
  public:
	// configuration 
	virtual bool  interrogate(void*)                                   = 0;
    virtual void  configure()                                          = 0;
	// initialization
    virtual bool  setup(void*)                                         = 0;
    virtual bool  restore()                                            = 0;
	// execution
    virtual void  beginDrawFrame()                                     = 0;
    virtual void  beginDrawObject(const Matrix& w)                     = 0;
    virtual void  set(const Reflectivity& reflectivity)                = 0;
    virtual void  set(RenderState, bool)                               = 0;
    virtual void  beginDraw(unsigned flags)                            = 0;
    virtual void  endDraw()                                            = 0;
    virtual void  endDrawFrame()                                       = 0;
    virtual void  resize()                                             = 0;
	// termination
    virtual void  suspend()                                            = 0;
    virtual void  release()                                            = 0;
	virtual void  Delete()                                             = 0;
};

iDisplay* CreateDisplay(iContext* context);

#endif