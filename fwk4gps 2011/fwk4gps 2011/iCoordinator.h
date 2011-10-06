#ifndef _I_COORDINATOR_H_
#define _I_COORDINATOR_H_

/* Coordinator Interface - Model Branch
 *
 * iCoordinator.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iCoordinator -------------------------------------
//
// iCoordinator is the Interface to the Coordinator class
//
class iContext;
class iObject;
class iTexture;
class iLight;
class iCamera;
class iSound;
class iText;
class iGraphic;
enum  Category;

class iCoordinator {
  public:
	// initialization
    virtual bool  add(const iObject* o)                              = 0;
    virtual bool  add(const iTexture* t)                             = 0;
    virtual bool  add(const iLight* l)                               = 0;
    virtual bool  add(const iCamera* c)                              = 0;
    virtual bool  add(const iSound* s)                               = 0;
    virtual bool  add(const iGraphic* v)                             = 0;
    virtual bool  add(const iText* t)                                = 0;
    virtual void  reset(int now)                                     = 0;
	virtual void  restore(int now)                                   = 0;
	// execution
    virtual void  hudPos(float&, float&) const                       = 0;
    virtual bool  hudIsOn() const                                    = 0;
	virtual void  update(int now)                                    = 0;
    virtual void  render(Category category)                          = 0;
    virtual int   frameRate() const                                  = 0;
    virtual const iCamera* currentCamera() const                     = 0;
	// termination
    virtual void  suspend() const                                    = 0;
    virtual void  release() const                                    = 0;
	virtual bool  remove(const iObject* o)                           = 0;
	virtual bool  remove(const iTexture* t)                          = 0;
	virtual bool  remove(const iLight* l)                            = 0;
    virtual bool  remove(const iCamera* c)                           = 0;
    virtual bool  remove(const iSound* s)                            = 0;
    virtual bool  remove(const iGraphic* v)                          = 0;
    virtual bool  remove(const iText* t)                             = 0;
    virtual void  Delete() const                                     = 0;
};

iCoordinator* CreateCoordinator(iContext* c);

#endif
