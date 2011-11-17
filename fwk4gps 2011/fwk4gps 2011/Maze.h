#ifndef _MAZE_H_
#define _MAZE_H_

#include "Configuration.h"
class Maze {

    int mazeArray[MAZE_ARRAY_COL][MAZE_ARRAY_ROW];
	void generateMazeWallH(int x1, int y1, int x2, int y2);
	void generateMazeWallV(int x1, int y1, int x2, int y2);
	int condition;
	bool collision;
  public:
    Maze();
	void renderMaze();
	void generateMaze();
	bool checkCollision(int x, int y);
	int checkCondition();
	void toggleCollision();
	void toggleCollision(bool state);
};
#endif
