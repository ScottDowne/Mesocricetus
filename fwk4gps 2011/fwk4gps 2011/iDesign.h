#ifndef _I_DESIGN_H_
#define _I_DESIGN_H_

/* Design Interface - Model Branch
 *
 * iDesign.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iDesign ------------------------------------
//
// iDesign is the Interface to the Design class
//
class iContext;

class iDesign {
  public:
	// initialization
    virtual void configure()                            = 0;
    virtual void interrogate(void*)                     = 0;
    virtual bool setup(void*)                           = 0;
	virtual void initialize(int now)                    = 0;
	virtual void reset(int now)                         = 0;
    virtual void restore(int now)                       = 0;
	// execution
    virtual void update(int now)                        = 0;
    virtual void resize()                               = 0;
    virtual void render()                               = 0;
	// termination
    virtual void suspend()                              = 0;
    virtual void release()                              = 0;
    virtual void Delete() const                         = 0;
};

iDesign* CreateDesign(iContext* c);

#endif
