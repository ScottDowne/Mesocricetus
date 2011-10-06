#ifndef _I_API_GRAPHIC_H_
#define _I_API_GRAPHIC_H_

/* APIGraphic Interface - Direct API Branch
 *
 * iAPIGraphic.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPIGraphic ---------------------------
//
// iAPIGraphic is the Interface to the APIGraphic hierarchy
//
enum PrimitiveType;
enum VertexType;
class iVertexList;

class iAPIGraphic {
  public:
    virtual iAPIGraphic* clone() const                              = 0;
    virtual void draw()                                             = 0;
    virtual void suspend()                                          = 0;
    virtual void release()                                          = 0;
	virtual void Delete() const                                     = 0;
};

iAPIGraphic* CreateAPIPrimitiveSet(PrimitiveType, int, VertexType, 
 iVertexList*);

#endif
