#ifndef STUDENTSOLVEBUILDER_H
#define STUDENTSOLVEBUILDER_H

#include <assert.h>
#include <list>

#include "MTMazeStudentSolver.h"
#include "DFSTop.h"
#include "DFSBottom.h"

class StudentSolveBuilder : MTMazeStudentSolver
{
public:

	StudentSolveBuilder(Maze *maze, DFSTop *top, DFSBottom *bottom)
		: MTMazeStudentSolver(maze), topDFS(top), botDFS(bottom)
	{
		assert(maze);
	}

	std::list<Direction> *Solve() override
	{
		std::list<Direction> *topPath = new std::list<Direction>();
		std::list<Direction> *bottomPath = new std::list<Direction>();

		std::thread *t1 = new std::thread(&DFSTop::partialSolve, std::ref(topDFS), std::ref(topPath));
		std::thread *t2 = new std::thread(&DFSBottom::partialSolve, std::ref(botDFS), std::ref(bottomPath));

		t1->join();
		t2->join();

		// Move contents in bottomPath to the end of topPath
		// Incremented begin() iterator to account for extra direction added to beginning of bottomPath (Not good)
		topPath->splice(topPath->end(), *bottomPath, ++bottomPath->begin());

		delete t2;
		delete t1;
		delete bottomPath;

		return topPath;
	}

	void DFS() override {}
	void TracePath(std::list<Direction> *pSolution) override {}

	bool isVisited(Position pos) override { return false; }
	void setVisited(Position pos) override {}

	DFSTop *topDFS;
	DFSBottom *botDFS;
};
#endif // !STUDENTSOLVEBUILDER_H
