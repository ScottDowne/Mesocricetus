
#include <cstdlib>
#include <ctime>
#include "Configuration.h"
#include "iObject.h"         // for the Object Interface
#include "iGraphic.h"        // for the Graphic Interface
#include "iTexture.h"        // for the Texture Interface
#include "Maze.h"

Maze::Maze() {
  
	// set initial boundries for the maze
	for (int y = 0; y < MAZE_ARRAY_ROW; y++) {
		for (int x = 0; x < MAZE_ARRAY_COL; x++) {
		  if (y == 0 || y == MAZE_ARRAY_ROW-1 || x == 0 || x == MAZE_ARRAY_COL-1) {
			  mazeArray[y][x] = 0;
		  } else {
		      mazeArray[y][x] = 1;
		  } 
		}
	}

	// set starting and end points
	mazeArray[0][1] = 1;
	mazeArray[MAZE_ARRAY_ROW-1][MAZE_ARRAY_COL-2] = 1;
	condition = -1;
	collision = true;
	
	// randomly generate a maze
	srand((unsigned)time(0));
	generateMazeWallH(1, 1, MAZE_ARRAY_ROW-1, MAZE_ARRAY_COL-1 );

	// render the maze
	renderMaze();
}

void Maze::generateMazeWallH(int x1, int y1, int x2, int y2) {
	
  int wallLocation = 0;
  do {
  
    wallLocation = (rand() % (y2-y1)) + y1;
  } while (wallLocation % 2 != 0);

  int holeLocation = 0;
  do {
  
    holeLocation = (rand() % (x2-x1)) + x1;
  } while (holeLocation % 2 ==0);

  for (int x = x1; x < x2; x++) {
  
    if (x != holeLocation) mazeArray[wallLocation][x] = 0;
  }
  
  if (x2 - x1 >= 3) {

    if (wallLocation - y1 > 1) generateMazeWallV(x1, y1, x2, wallLocation);
    if (y2 - wallLocation > 1) generateMazeWallV(x1, wallLocation+1, x2, y2);
  }
}

void Maze::generateMazeWallV(int x1, int y1, int x2, int y2) {

  int wallLocation = 0;
  do {
  
    wallLocation = (rand() % (x2-x1)) + x1;
  } while (wallLocation % 2 != 0);

  int holeLocation = 0;
  do {
  
    holeLocation = (rand() % (y2-y1)) + y1;
  } while (holeLocation % 2 == 0);

  for (int y = y1; y < y2; y++) {
  
    if (y != holeLocation) mazeArray[y][wallLocation] = 0;
  }

  if (y2 - y1 >= 3) {

    if (wallLocation - x1 > 1) generateMazeWallH(x1, y1, wallLocation, y2);
    if (x2 - wallLocation > 1) generateMazeWallH(wallLocation+1, y1, x2, y2);
  }
}

void Maze::renderMaze() {
	iTexture* wall = CreateTexture(L"wall.PNG");
    Colour white(1, 1, 1);
    Colour yellow(0.7f, 0.7f, 0.0f);
    Colour grey(0.7f, 0.7f, 0.7f);
    Colour wallColour(0.8f, 0.8f, 0.6f);
    Colour black(0, 0, 0);

    Reflectivity wallReflectivity = Reflectivity(wallColour);
	// render maze
    int left, right, back, front;

    for (int y = 0; y < MAZE_ARRAY_COL; y++) {

       for (int x = 0; x < MAZE_ARRAY_ROW; x++) {
      
          if ( mazeArray[y][x] == 0) {
         
             left = right = back = front = 0;

             if (x == 0 || mazeArray[y][x-1] > 0) {
            
                left = 1;
             }

             if (x == MAZE_ARRAY_COL-1 || mazeArray[y][x+1] > 0) {
            
                right = 1;
             }

             if (y == 0 || mazeArray[y-1][x] > 0) {
            
                back = 1;
             }

             if (y == MAZE_ARRAY_ROW-1 || mazeArray[y+1][x] > 0) {
            
                front = 1;
             }
      
             iObject * obj = CreateObject(CreateFaces(-SCALE / 2.0f, -SCALE / 2.0f, -SCALE / 2.0f, 
                              SCALE / 2.0f, SCALE / 2.0f, SCALE / 2.0f, front, right, back, left, 0, 1, 2), &wallReflectivity);
             obj->attach(wall);
             obj->setRadius(sqrtf(SCALE*SCALE/2.0f));
             obj->translate(x * SCALE + SCALE / 2.0f, SCALE / 2.0f, (MAZE_ARRAY_COL - y) * SCALE + SCALE / 2.0f);
          }
          else {

            iObject * obj =  CreateObject(CreateFaces(-SCALE / 2.0f, -SCALE / 2.0f, -SCALE / 2.0f, 
                              SCALE / 2.0f, SCALE / 2.0f, SCALE / 2.0f, 0, 0, 0, 0, 1, 0, 2), &wallReflectivity);//->attach(wall);
            obj->setRadius(sqrtf(SCALE*SCALE/2.0f));
            obj->translate(x * SCALE + SCALE / 2.0f, SCALE / 2.0f, (MAZE_ARRAY_COL - y) * SCALE + SCALE / 2.0f);
          }
       }
    }
}

bool Maze::checkCollision(int x, int y) {

	bool rc = collision;
	// flip it
	y = MAZE_ARRAY_COL - y;

	if ( x >= 0 && x < MAZE_ARRAY_ROW &&  y >= 0 && y < MAZE_ARRAY_COL && mazeArray[y][x] >= 1) {
	  rc = false;
	}
	
	// set player condition to start, victory, or neither
	// -1 start, 1 victory, 0 neither
	  if (y == MAZE_ARRAY_ROW-1 && x == MAZE_ARRAY_COL-2) {
	
	    condition = -1;
	  } else if (y == 0 && x == 1) {
	
	      condition = 1;
	  } else {
	
		  condition = 0;
	  }

	return rc;
}

int Maze::checkCondition() {

  return condition;
}

void Maze::toggleCollision() {

	collision = !collision;
}

void Maze::toggleCollision(bool state) {

	collision = state;
}