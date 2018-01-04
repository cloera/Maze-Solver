#ifndef POSITION_MAZE_H
#define POSITION_MAZE_H

#include <assert.h>
#include "Direction.h"

class Position 
{
public:
	
	Position(int col, int row) 
	{
		this->col = col;
		this->row = row;
	}

	Position()
	{
		this->col = 0;
		this->row = 0;
	}

	bool operator ==( const Position &p ) const 
	{
		return ( (this->col == p.col) && (this->row == p.row));
	}

	Position move(Direction dir) 
	{
		Position tmp;

		switch(dir) 
		{
		case Direction::North:
			tmp = Position(col,row-1);
			break;

		case Direction::South:			
			tmp = Position(col,row+1);
			break;

		case Direction::East:
			tmp = Position(col+1,row);
			break;

		case Direction::West:
			tmp = Position(col-1,row);
			break;

		default:
			assert(false);
			break;
		}

		return tmp;
	}
		
	public: 
		// data
		int col;
		int row;
};




#endif