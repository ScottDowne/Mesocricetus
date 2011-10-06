#ifndef _I_SCENE_H_
#define _I_SCENE_H_

/* Model Interface - Model Branch
 *
 * iModel.h
 * fwk4gps version 2.0
 * gam666/dps901
 * Sept 2 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iModel -------------------------------------
//
// iModel is the Interface to the Model class
//
class iContext;
class iObject;
class iTexture;
class iLight;
class iCamera;
class iSound;
class iText;
class iPrimitiveSet;
struct Rect;
enum  Category;

class iModel {
  public:
	// initialization
    virtual bool add(const iObject* o)                              = 0;
    virtual bool add(const iTexture* t)                             = 0;
    virtual bool add(const iLight* l)                               = 0;
    virtual bool add(const iCamera* c)                              = 0;
    virtual bool add(const iSound* s)                               = 0;
    virtual bool add(const iPrimitiveSet* v)                        = 0;
    virtual bool add(const iText* t)                                = 0;
    virtual void reset(int now)                                     = 0;
	virtual void restore(int now)                                   = 0;
	// execution
    virtual const Rect& hudRect() const                             = 0;
    virtual bool hudIsOn() const                                    = 0;
	virtual void update(int now)                                    = 0;
    virtual void render(Category category)                          = 0;
    virtual int  frameRate() const                                  = 0;
    virtual const iCamera* currentCamera() const                    = 0;
	// termination
    virtual void suspend() const                                    = 0;
    virtual void release() const                                    = 0;
	virtual bool remove(const iObject* o)                           = 0;
	virtual bool remove(const iTexture* t)                          = 0;
	virtual bool remove(const iLight* l)                            = 0;
    virtual bool remove(const iCamera* c)                           = 0;
    virtual bool remove(const iSound* s)                            = 0;
    virtual bool remove(const iPrimitiveSet* v)                     = 0;
    virtual bool remove(const iText* t)                             = 0;
    virtual void Delete() const                                     = 0;
};

extern "C"
iModel* CreateModel(iContext* c);

#endif
