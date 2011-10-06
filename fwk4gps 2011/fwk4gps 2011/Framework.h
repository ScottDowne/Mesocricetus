#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

/* Framework Definition
 *
 * Framework.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- Framework ----------------------------------
//
// The Framework supervises the application 
//
class iContext;
class iEngine;

class Framework {

    iContext* context; // points to the Context object
    iEngine*  engine;  // points to the Engine object

	Framework(const Framework& m);            // prevents copying
	Framework& operator=(const Framework& m); // prevents assignment

  public:
    Framework(void*, int);
	int     run();
    virtual ~Framework();
};

#endif