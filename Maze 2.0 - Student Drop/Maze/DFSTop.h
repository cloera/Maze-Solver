#ifndef DFSTOP_H
#define DFSTOP_H

#include <assert.h>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "MTMazeStudentSolver.h"

class DFSTop : public MTMazeStudentSolver
{
public:
	DFSTop(Maze *maze)
		: MTMazeStudentSolver(maze)
	{
		assert(this->pMaze);
	}

	/* Not used but still need to implement it */
	std::list<Direction> *Solve() override
	{
		std::list<Direction> *pSolve = new std::list<Direction>();
		partialSolve(pSolve);
		return pSolve;
	}

	void partialSolve(std::list<Direction> *pSolve)
	{
		std::thread *topDFS = new std::thread(&DFSTop::DFS, this);
		std::thread *trace = new std::thread(&DFSTop::TracePath, this, std::ref(pSolve));

		topDFS->join();
		trace->join();

		delete topDFS;
		delete trace;
	}

	void DFS() override
	{
		std::list<MyChoice> stack;

		// Push start position to queue
		stack.push_front(MyChoice(this->pMaze->getStart(),
									this->pMaze->getStart(),
									this->pMaze->getMoves(this->pMaze->getStart())));

		// Set start position as visited
		this->setVisited(this->pMaze->getStart());

		MyChoice currentChoice;
		Direction nextDir;

		// loop until all Positions in maze have been traversed or end has been reached
		while (!stack.empty())
		{
			// Get current position in maze
			currentChoice = stack.front();

			// Break if at end or collided with other thread
			if (this->isDFSTopDone.load())
			{
				break;
			}

			// If list has directions then visit them
			if (currentChoice.myDirections.size() > 0)
			{
				// Pop next direction from list
				nextDir = stack.front().myDirections.pop_front();
				// push new choice to stack and Scout Path of choices 
				stack.push_front(ScoutPath(currentChoice, nextDir));

				continue;
			}

			stack.pop_front();

		} // End While loop
	} // END DFS()

	MyChoice ScoutPath(MyChoice currentChoice, Direction goToDir)
	{
		// Store Position where I came from
		currentChoice.from = currentChoice.at;
		// Store direction where I came from
		Direction came_from = reverseDir(goToDir);
		// Store next position
		currentChoice.at = movePosition(currentChoice.at, goToDir);

		// Keep on scouting maze until you hit a deadend or come to a junction
		do
		{
			// Break if at end or collided with other thread
			if ((this->pMaze->atomic_getCell(currentChoice.at) & (int)MazeMarkers::VisitedBottom) > 0 || 
				currentChoice.at == this->pMaze->getEnd())
			{
				//printf("Top at: %d, %d\n", currentChoice.at.row, currentChoice.at.col);
				//printf("Top from: %d, %d\n", currentChoice.from.row, currentChoice.from.col);
				this->startTopTrace = currentChoice.from;
				this->isDFSTopDone.store(true);
				break;
			}

			// Set MazeMarker of where I came from
			this->setFromDirection(currentChoice.at, goToDir);
			// Set as visited
			this->setVisited(currentChoice.at);

			// Get list of new directions
			currentChoice.myDirections = this->pMaze->getMoves(currentChoice.at);
			// Remove previous position from direction list
			currentChoice.myDirections.remove(came_from);

			if (currentChoice.myDirections.size() == 1)
			{
				// Set direction to go to
				goToDir = currentChoice.myDirections.front();
				// Store Position where I came from
				currentChoice.from = currentChoice.at;
				// Store direction where I came from
				came_from = reverseDir(goToDir);
				// Store next position
				currentChoice.at = movePosition(currentChoice.at, goToDir);
			}

		} while (currentChoice.myDirections.size() == 1);

		return currentChoice;
	}

	void TracePath(std::list<Direction> *pSolution) override
	{
		while (!(this->isDFSTopDone.load()))
		{
			//Sleep(1);
		}
		//printf("TracePath Top: Unlocked\n");

		// Get position to start tracing path
		Position currentPos = this->startTopTrace;
		//printf("Top StartTrace: row: %d, col: %d\n", currentPos.row, currentPos.col);

		// Traverse backwards from end of maze to start of maze and save path
		while (!(currentPos == this->pMaze->getStart()))
		{
			// Came from North direction to get to current
			if (this->pMaze->atomic_getCell(currentPos) & (int)MazeMarkers::FromNorth)
			{
				// Push South to direction list
				pSolution->push_front(Direction::South);
				// We moved South from previous position to get to current position so we came from the North
				currentPos = movePosition(currentPos, Direction::North);
				//printf("TracePath: Moving NORTH from SOUTH\n");
			}
			else if (this->pMaze->atomic_getCell(currentPos) & (int)MazeMarkers::FromWest)
			{
				// Push East to direction list
				pSolution->push_front(Direction::East);
				// We moved East from previous position to get to current position so we came from the West
				currentPos = movePosition(currentPos, Direction::West);
				//printf("TracePath: Moving WEST from EAST\n");
			}
			else if (this->pMaze->atomic_getCell(currentPos) & (int)MazeMarkers::FromEast)
			{
				// Push West to direction list
				pSolution->push_front(Direction::West);
				// We moved West from previous position to get to current position so we came from the East
				currentPos = movePosition(currentPos, Direction::East);
				//printf("TracePath: Moving EAST from WEST\n");
			}
			else if (this->pMaze->atomic_getCell(currentPos) & (int)MazeMarkers::FromSouth)
			{
				// Push North to direction list
				pSolution->push_front(Direction::North);
				// We moved North from previous position to get to current position so we came from the South
				currentPos = movePosition(currentPos, Direction::South);
				//printf("TracePath: Moving SOUTH from NORTH\n");
			}
		} // End While loop
	} // END TracePath()

	bool isVisited(Position pos) override
	{
		// Check if this position is visited
		return (this->pMaze->atomic_getCell(pos) & (int)MazeMarkers::VisitedTop) > 0;
	}

	void setVisited(Position pos) override
	{
		this->pMaze->atomic_setCell(pos, (int)MazeMarkers::VisitedTop);
	}

};

#endif // !DFSTOP_H

