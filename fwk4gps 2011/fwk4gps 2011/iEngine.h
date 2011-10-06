#ifndef _I_ENGINE_H_
#define _I_ENGINE_H_

/* Engine Interface
 *
 * iEngine.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iEngine ------------------------------------
//
// iEngine is the interface to the Engine class
//
class iContext;

class iEngine {
  public:
	// configuration
    virtual void getConfigurations(void*) = 0;
	virtual void reconfigure()            = 0;
    // execution
	virtual int  run()                    = 0;
	virtual void resize() const           = 0;
    virtual bool isActive() const         = 0;
	// suspend execution
    virtual void suspend()                = 0;
    virtual void restore()                = 0;
	// termination
    virtual void Delete() const           = 0;
};

extern "C"
iEngine* CreateEngine(void*, int, iContext*);

extern "C"
iEngine* EngineAddress();

#endif