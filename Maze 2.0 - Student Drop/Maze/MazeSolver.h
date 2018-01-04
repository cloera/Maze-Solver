#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <list>
#include "Maze.h"

class MazeSolver abstract
{

public:

	// Constructor, set the maze
	MazeSolver( Maze *maze ) 
	{
		pMaze = maze;
		assert( this->pMaze != 0);
	}	

	// Must overload
	virtual std::list<Direction> *Solve() abstract;
	     
protected:
	// data
	Maze *pMaze;

};


#endif