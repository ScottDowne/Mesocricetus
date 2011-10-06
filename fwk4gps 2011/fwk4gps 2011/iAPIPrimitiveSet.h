#ifndef _I_API_PRIMITIVE_SET_H_
#define _I_API_PRIMITIVE_SET_H_

/* APIPrimitiveSet Interface - Direct API Branch
 *
 * iAPIPrimitiveSet.h
 * fwk4gps version 2.0
 * gam666/dps901
 * Sept 2 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPIPrimitiveSet ---------------------------
//
// iAPIPrimitiveSet is the Interface to the Primitive Set hierarchy
//
enum PrimitiveType;
enum VertexType;
class iVertexList;

class iAPIPrimitiveSet {
  public:
    virtual iAPIPrimitiveSet* clone() const                              = 0;
    virtual void draw()                                                  = 0;
    virtual void suspend()                                               = 0;
    virtual void release()                                               = 0;
	virtual void Delete() const                                          = 0;
};

extern "C"
iAPIPrimitiveSet* CreateAPIPrimitiveSet(PrimitiveType, int, VertexType, 
 iVertexList*);

#endif
