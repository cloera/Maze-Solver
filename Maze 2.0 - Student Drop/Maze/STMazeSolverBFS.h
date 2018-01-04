#ifndef ST_Maze_Solver_BFS_H
#define ST_Maze_Solver_BFS_H

// Algorithm & Maze generation - refactored and modified from material on the internet
//    Influenced by many different solvers and maze categories
//    Purpose of the maze is to create a concurrent problem set for C++ concurrency class
//    Maze solving algorithm and generators were derived from University of Maryland - Java Development

#include <assert.h>
#include <list>
#include "Direction.h"
#include "Choice.h"

// Breadth First solver
class SolutionNodeBFS 
{
public:
	SolutionNodeBFS *pParent;
	Choice pChoice;
		
	SolutionNodeBFS()
	{
		this->pParent = 0;
	}

	SolutionNodeBFS( SolutionNodeBFS * const parent, Choice choice) 
	{
		this->pParent = parent;
		this->pChoice = choice;
	}

	~SolutionNodeBFS()
	{
	
	}
};


class STMazeSolverBFS: public SkippingMazeSolver 
{
public:	
		
	STMazeSolverBFS(Maze *maze) 
	: SkippingMazeSolver( maze )
	{
		exploring = Direction::Uninitialized;
		assert( maze );
	}

	// Expands the searching node pNode
	// Returns a list of new decision nodes 
	//    Nodes that have either "deadend" or have more than one option {choice}
	void expand( std::list<SolutionNodeBFS> *pResult, SolutionNodeBFS * const pNode ) 
	{
		// Nothing dynamic 
		ListDirection ds = pNode->pChoice.pChoices;

		for( Direction dir = ds.begin(); dir != Direction::Uninitialized; dir = ds.getNext(dir) )
		{ 
			exploring = dir;		
			SolutionNodeBFS pTmp(  pNode, follow( pNode->pChoice.at, exploring ) );
			pResult->push_back( pTmp );
		}
	}
	
	// Breadth first algorithm
	// Creates a list of "open" choices - the "frontier"
	//    Each open choice is the result of a follow
	// Each of those are explored deeper into the new list "new frontier"
	//    Becomes the active frontier
	// Rinse and repeat until you hit the final location

	std::list<Direction> *Solve() 
	{	
		SolutionNodeBFS *pCurr = 0;
		std::list<SolutionNodeBFS> *frontier = new std::list<SolutionNodeBFS>();

		try 
		{
			// Start from the 1st location
			SolutionNodeBFS pTmp(0, firstChoice(pMaze->getStart()) );
			frontier->push_front( pTmp );
			
			// walk the frontier list until finished or unitl solution was found
			while(frontier->size()) 
			{
				std::list<SolutionNodeBFS> *new_frontier = new std::list<SolutionNodeBFS>();
			
					// iterate through the list
					for ( std::list<SolutionNodeBFS>::iterator itFrontier = frontier->begin(); 
						  itFrontier != frontier->end(); 
						  ++itFrontier)
					{

						// Node is not a deadend, do a deeper search
						if( !itFrontier->pChoice.isDeadend() ) 
						{
							SolutionNodeBFS *pNode = new SolutionNodeBFS();
							*pNode = *itFrontier;

							pCurr = pNode;

							// add the expanded list to the new_frontier list
							std::list<SolutionNodeBFS> *pExpandedList = new std::list<SolutionNodeBFS>();

								expand(pExpandedList, pCurr);
								new_frontier->splice( new_frontier->end(), *pExpandedList );

							delete pExpandedList;

						}
						else
						{
							// Do not add - deadend
							// not contributing to the solution
						}
					}
			
				// Empty and remove allocations
				frontier->clear();
				delete frontier;

				frontier = new_frontier;

			}
			// No solution found.
			return 0;
		}


		catch (SolutionFoundSkip e) 
		{
			std::list<Direction> *soln = new std::list<Direction>();

			if(pCurr == 0) 
			{
				// this only happens if there was a direct path from the start to the end
				assert(0);
				soln->push_front( pMaze->getMoves( pMaze->getStart()).front() );	
				return pathToFullPath( soln );
			}
			else 
			{
				// First save the direction we were going in when the exit was discovered.
				soln->push_front(exploring);

				
				while(pCurr != 0) 
				{

					try 
					{
						Choice walkBack = follow( pCurr->pChoice.at, pCurr->pChoice.from);
						soln->push_front( walkBack.from );

						pCurr = pCurr->pParent;
						
					}
					catch(SolutionFoundSkip e2) 
					{
						// If there is a choice point at the maze entrance, then record
						// which direction we should choose. 
						if( pMaze->getMoves(pMaze->getStart()).size() > 1)
						{
							soln->push_front(e2.from);
						}
						 
						return pathToFullPath( soln );
					}
				}

				markPath( soln );

				return pathToFullPath( soln );
			}
		}
	}


	private:
		Direction exploring;
};

#endif


