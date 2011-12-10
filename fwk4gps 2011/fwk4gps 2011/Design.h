#ifndef _DESIGN_H_
#define _DESIGN_H_

/* Design Definition - Model Branch
 *
 * Design.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iDesign.h" // for the Design Interface
#include <list>
//-------------------------------- Design -------------------------------------
//
// The Design class defines the structure of the game design
//
class iCoordinator;
class iDisplay;
class iSoundCard;
class iObject;
class iLight;
class iSound;
class iText;

class Design : public iDesign {

	iContext*     context;     // points to the Context object
    iCoordinator* coordinator; // points to the Coordinator object
    iDisplay*     display;     // points to the Display object
    iSoundCard*   soundCard;   // points to the SoundCard object

	iLight*   pointLight;      // points to the point light

    //std::list<iObject*> * mazeList;   //points to the maze
	
    iCamera*  camera;          // points to the camera
	
    iText*  timerText;
   iSound* midi;
   iSound* jump;

    int lastUpdate;      // time that the coordinator was last updated
	int startTime;
	int timeLeft;
	wchar_t timerString[MAX_DESC + 1];
	bool gameOver;

	Maze* maze;

    Design(iContext* c);
    Design(const Design& s);            // prevents copying
    Design& operator=(const Design& s); // prevents assignment
    virtual ~Design();

  public:
	friend iDesign* CreateDesign(iContext* c);
	// initialization
    void   interrogate(void*);
    bool   setup(void*);
    void   initialize(int now);
	void   reset(int now);
    void   configure();
	// execution function
    void   update(int now);
    void   resize();
    void   render();
    // suspension
    void   suspend();
    void   restore(int now);
	// termination
    void   release();
	void   Delete() const { delete this; }
};

#endif
