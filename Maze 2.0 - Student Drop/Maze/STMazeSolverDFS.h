#ifndef ST_Maze_Solver_DFS_H
#define ST_Maze_Solver_DFS_H

// Algorithm & Maze generation - refactored and modified from material on the internet
//    Influenced by many different solvers and maze categories
//    Purpose of the maze is to create a concurrent problem set for C++ concurrency class
//    Maze solving algorithm and generators were derived from University of Maryland - Java Development

#include <assert.h>
#include <list>

#include "Choice.h"
#include "SkippingMazeSolver.h"

class STMazeSolverDFS : public SkippingMazeSolver 
{
public: 
	STMazeSolverDFS(Maze *maze) 
	: SkippingMazeSolver( maze )
	{
		assert(pMaze);
	}

	std::list<Direction> *Solve() override
	{
		std::list<Choice> *pChoiceStack = new std::list<Choice>();
		Choice ch;
		int count = 0;
		try 
		{
			pChoiceStack->push_front( firstChoice( pMaze->getStart() ) ) ;

			while(!(pChoiceStack->size() == 0) )
			{
				count++;

				ch = pChoiceStack->front();
				if(ch.isDeadend()) 
				{
					// backtrack.
					pChoiceStack->pop_front();

					if(!(pChoiceStack->size() == 0) )
					{
						pChoiceStack->front().pChoices.pop_front();
					}

					continue;
				}
				
				pChoiceStack->push_front( follow( ch.at, ch.pChoices.front() ) );
				
			}
			// No solution found.
			return 0;
		}
		catch (SolutionFoundSkip e) 
		{
			e; // unused

			std::list<Choice>::iterator iter = pChoiceStack->begin();
			std::list<Direction> *pSolutionPath  = new std::list<Direction>();

			while(iter != pChoiceStack->end()) 
			{
				pSolutionPath->push_front(iter->pChoices.front());
				iter++;
			}

			return pathToFullPath( pSolutionPath );
		}
	}
};


#endif