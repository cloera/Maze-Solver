#ifndef MAZE_H
#define MAZE_H

#include <list>
#include <memory>
#include <atomic>

#include "Position.h"
#include "Direction.h"
#include "Choice.h"

#define DEBUG_PRINT 0

// Maze cells are (row,col) starting at (0,0)
// Linear storage of integers 0 to (width*height - 1)
// Upper Upper corner (0,0) or index 0
// Lower Left corner (width-1, height-1) or (width*height - 1)

class Maze
{
public:

	Maze();
	Maze( int _width, int _height);
	~Maze();

	void Load( const char * const fileName );
	void Solve();

	ListDirection getMoves(Position pos);
	bool Maze::canMove( Position pos, Direction dir );
	bool checkSolution( std::list<Direction> *soln );

	Position getStart();
	Position getEnd();

	void setEast(Position pos);
	void setSouth(Position pos);
	int getCell(Position pos);
	void setCell(Position pos, int val);

	// My added functions
	void atomic_setCell(Position pos, int val);
	int atomic_getCell(Position pos);

// data:
	std::atomic<int> *pMazeData;
	std::atomic<int> height;
	std::atomic<int> width;
	std::atomic<int> solvable;
};


#endif