#ifndef MT_Maze_Student_Solver_H
#define MT_Maze_Student_Solver_H

#include <assert.h>
#include <list>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Maze.h"
#include "MazeSolver.h"
#include "Direction.h"
#include "MazeMarkers.h"

struct MyChoice
{
	MyChoice()
	{
	}

	MyChoice(Position at, Position from, ListDirection _pChoices)
	{
		this->at = at;
		this->myDirections = _pChoices;
		this->from = from;
	}

	~MyChoice()
	{
	}

	bool isDeadend()
	{
		return (myDirections.size() == 0);
	}

	// data:
	Position at;
	Position from;
	ListDirection myDirections;
};

// Feel free to change your class any way you see fit
// It needs to inherit at some point from the MazeSolver
class MTMazeStudentSolver : public MazeSolver 
{
public: 

	MTMazeStudentSolver(Maze *maze) 
	: MazeSolver( maze ), isDFSTopDone(false), isDFSBotDone(false), startBottomTrace(Position()), startTopTrace(Position())
	{
		assert(pMaze);
	}

	virtual void DFS() abstract;
	virtual void TracePath(std::list<Direction> *pSolution) abstract;

	virtual bool isVisited(Position pos) abstract;
	virtual void setVisited(Position pos) abstract;



	void setFromDirection(Position pos, Direction dir)
	{
		// dir is direction moving towards
		// atomic_setCell() with MazeMarker of previous direction
		switch (dir)
		{
		case Direction::South:
			this->pMaze->atomic_setCell(pos, (int)MazeMarkers::FromNorth);
			break;
		case Direction::North:
			this->pMaze->atomic_setCell(pos, (int)MazeMarkers::FromSouth);
			break;
		case Direction::East:
			this->pMaze->atomic_setCell(pos, (int)MazeMarkers::FromWest);
			break;
		case Direction::West:
			this->pMaze->atomic_setCell(pos, (int)MazeMarkers::FromEast);
			break;
		}
	}

	Position movePosition(Position pos, Direction dir)
	{
		switch (dir)
		{
		case Direction::North:
			pos.row -= 1;
			break;

		case Direction::South:
			pos.row += 1;
			break;

		case Direction::East:
			pos.col += 1;
			break;

		case Direction::West:
			pos.col -= 1;
			break;

		default:
			assert(false);
			break;
		}

		return pos;
	}

	// Data
	Position startBottomTrace;
	Position startTopTrace;
	std::atomic<bool> isDFSTopDone;
	std::atomic<bool> isDFSBotDone;

};


#endif