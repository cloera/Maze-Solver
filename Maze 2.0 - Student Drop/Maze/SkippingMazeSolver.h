#ifndef Skipping_Maze_Solver_H
#define Skipping_Maze_Solver_H

// Algorithm & Maze generation - refactored and modified from material on the internet
//    Influenced by many different solvers and maze categories
//    Purpose of the maze is to create a concurrent problem set for C++ concurrency class
//    Maze solving algorithm and generators were derived from University of Maryland - Java Development

#include <exception>
#include "MazeSolver.h"
#include "Position.h"
#include "Choice.h"

class SolutionFoundSkip : public std::exception
{
public:
		
	SolutionFoundSkip(Position _pos, Direction _from) 
	{
		this->pos = _pos;
		this->from = _from;
	}
		
	Position pos;
	Direction from;

};


class SkippingMazeSolver : public MazeSolver 
{
public:
	
	SkippingMazeSolver(Maze *maze) 
	: MazeSolver( maze )
	{
		assert( maze );
	}
	
	
	Choice firstChoice( Position pos ) //throw SolutionFound 
	{
		ListDirection Moves = pMaze->getMoves(pos);

		if(Moves.size() == 1)
		{
			Direction tmp = Moves.begin();
			return follow(pos, tmp);
		}
		else
		{
			Choice p(pos, Direction::Uninitialized, Moves);
			return p;		
		}
	}
	
	Choice follow( Position at, Direction dir )  //throws SolutionFoundSkip() 
	{
		ListDirection Choices;
		Direction go_to = dir;
		Direction came_from = reverseDir(dir);
		at = at.move(go_to);

		do 
		{
			if( at == (pMaze->getEnd()) )
			{
				throw SolutionFoundSkip( at, reverseDir(go_to) );
				
			}
			if( at == (pMaze->getStart()) )
			{
				throw SolutionFoundSkip( at, reverseDir(go_to) );
			}
			Choices = pMaze->getMoves(at);
			Choices.remove(came_from);
	
			if(Choices.size() == 1) 
			{
				go_to = Choices.begin();
				at = at.move(go_to);
				came_from = reverseDir(go_to);
			}

		} while( Choices.size() == 1 );
		
		Choice pRet( at, came_from, Choices ) ;

		return pRet;
	}


	void markPath( std::list<Direction> *path) 
	{
		try 
		{
			Choice pChoice = firstChoice( pMaze->getStart() );
				
			Position at = pChoice.at;
			std::list<Direction>::iterator iter = path->begin();//.iterator();
			while(iter != path->end()) 
			{
				pChoice = follow(at, *iter);
				iter++;
				at = pChoice.at;
			} 
		}
		catch(SolutionFoundSkip e) 
		{ 
			e;  // unused
		}
	}

	
	std::list<Direction> *pathToFullPath( std::list<Direction> *path ) 
	{
		std::list<Direction>::iterator pathIter = path->begin();
		std::list<Direction> *pFullPath = new std::list<Direction>();

		// Get full solution path.
		Position curr = pMaze->getStart();
		Direction go_to = Direction::Uninitialized;
		Direction came_from = Direction::Uninitialized;

		while( !(curr == pMaze->getEnd()) ) 
		{
			ListDirection pMoves = pMaze->getMoves(curr);

			if( Direction::Uninitialized != came_from)
			{
				pMoves.remove(came_from);
			}

			if(pMoves.size() == 1)
			{
				go_to = pMoves.front();
			}
			else if(pMoves.size() > 1)
			{
				go_to = *pathIter++;
			}
			else if(pMoves.size() == 0) 
			{
				printf("Error in solution--leads to deadend.");
				assert(false);
			}

			pFullPath->push_back(go_to);
			curr = curr.move(go_to);
			came_from = reverseDir(go_to);
		}
		
		return pFullPath;
	}

};


#endif