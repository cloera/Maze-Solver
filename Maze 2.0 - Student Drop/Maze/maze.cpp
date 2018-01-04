#include <stdio.h>
#include <assert.h>

// Algorithm & Maze generation - refactored and modified from material on the internet
//    Influenced by many different solvers and maze categories
//    Purpose of the maze is to create a concurrent problem set for C++ concurrency class
//    Maze solving algorithm and generators were derived from University of Maryland - Java Development

#include <list>
#include <memory>

#include "Maze.h"
#include "File.h"
#include "Choice.h"

enum class InternalBit
{
	EAST_BIT = 1,
	SOUTH_BIT = 2,
};

struct FileHeader
{
	int width;
	int height;
	int solvable;
	// int data[];  data following
};

Maze::Maze()
{
	this->width = 0;
	this->height = 0;
	this->pMazeData = 0;
}

Maze::Maze( int _width, int _height)
{
	this->width = _width;
	this->height = _height;

	this->pMazeData = new std::atomic<int>[ this->width * this->height ];
	assert( pMazeData != 0 );
}

Maze::~Maze()
{
	delete this->pMazeData;
}

void Maze::Load( const char * const inFileName )
{
	FileHandle fh;
	FileError  ferror;

	//----------- Open to Read  --------------------------------
	ferror = File::open(fh, inFileName, FILE_READ );
	assert( ferror == FILE_SUCCESS );

	//----------- Seek ------------------------------------------

	ferror = File::seek( fh, FILE_SEEK_END, 0 );
	assert( ferror == FILE_SUCCESS );

	int fileSize;
	ferror = File::tell( fh, fileSize );
	assert( ferror == FILE_SUCCESS );

	unsigned char *pBuff = new unsigned char[fileSize];
	assert( pBuff != 0 );
	
	ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
	assert( ferror == FILE_SUCCESS );

	ferror = File::read( fh, pBuff, fileSize );
	assert( ferror == FILE_SUCCESS );

	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );

	// ----- Everything is in memory ------
	// --- Now decompress it 

	FileHeader *pHdr;

	pHdr = (FileHeader *) pBuff;

		// copy it to Maze data
		this->height = pHdr->height;
		this->width = pHdr->width;
		this->solvable = pHdr->solvable;

	pBuff += sizeof(FileHeader);

	int *pIntData = (int *)pBuff;

	// reserve the space
	this->pMazeData = new std::atomic<int>[ this->width * this->height ];
	assert(this->pMazeData != 0 );
	memset(this->pMazeData, 0x0, (this->width * this->height) * sizeof(int) );

	int i = 0;
	int j = 0;
	Position pos = Position(0,0);
	while(pos.row < height) 
	{
		pos = Position(0, pos.row);
		while(pos.col < width) 
		{
			int bits = *pIntData++;

			int origBits = bits;
			j++;

			for( int bit = 0; (bit < 16 && pos.col < width); bit++) 
			{
				if((bits & 1) == 1)
				{
					
					setEast(pos); 
				}

				if((bits & 2) == 2)
				{
					setSouth(pos);
				}

			//	printf("-->i:%d %j:%d  org:%x val:%d\n",i++, j, origBits, getCell(pos) );

				bits >>= 2;
				
				pos = pos.move(Direction::East);
			}
		}
		pos = pos.move(Direction::South);
	}
}
 

ListDirection Maze::getMoves(Position pos) 
{
	ListDirection result;
		
	if( canMove(pos,Direction::South) )
	{
		result.south = Direction::South;
	}
	if(canMove(pos,Direction::East))
	{
		result.east = Direction::East;
	}
	if(canMove(pos,Direction::West))
	{
		result.west = Direction::West;
	}
	if(canMove(pos,Direction::North))
	{
		result.north = Direction::North;
	}

	return result;
}
	
bool Maze::canMove(Position pos, Direction dir) 
{
	bool status = false;

	switch(dir) 
	{
	case Direction::North:
		if(pos.row == 0) 
		{
			status = false;
		}
		else 
		{
			status = ((getCell(pos.move(Direction::North)) & (int)InternalBit::SOUTH_BIT) == 0);
		}
		break;

	case Direction::South:
		status = ((getCell(pos) & (int)InternalBit::SOUTH_BIT) == 0);
		break;

	case Direction::East:
		status = ((getCell(pos) & (int)InternalBit::EAST_BIT) == 0);
		break;

	case Direction::West:
		if(pos.col == 0) 
		{
			status = false;
		}
		else 
		{
			status = ((getCell(pos.move(Direction::West)) & (int)InternalBit::EAST_BIT) == 0);
		}	
		break;

	default:
		assert( false );
		break;
	}

	return status;
		
}

Position Maze::getStart() 
{
	Position tmp = Position(this->width/2, 0);
	return tmp;
}


Position Maze::getEnd() 
{
	Position tmp = Position(this->width/2, this->height-1);
	return tmp;
}

void Maze::setEast(Position pos) 
{
	int newVal = getCell(pos) | (int)InternalBit::EAST_BIT;
	setCell(pos,newVal);
}	

void Maze::setSouth(Position pos) 
{
	int newVal = getCell(pos) | (int)InternalBit::SOUTH_BIT;
	setCell(pos,newVal);
}

int Maze::getCell(Position pos) 
{
	int val = pMazeData[pos.col + pos.row * this->width];
	return val;
}

int Maze::atomic_getCell(Position pos)
{
	return this->pMazeData[pos.col + pos.row * this->width].load();
}
	
void Maze::setCell(Position pos, int val) 
{
	this->pMazeData[pos.col + pos.row * this->width] = val;
}

void Maze::atomic_setCell(Position pos, int val)
{
	this->pMazeData[pos.col + pos.row * this->width] |= val;
}

bool Maze::checkSolution( std::list<Direction> *soln ) 
{
	assert( soln );
	//printf("\nCheckSolution(): begin ...\n");
		
	Position at = getStart();

	//printf("  ---> start (row,col)  %d %d \n",at.row, at.col);
	std::list<Direction>::iterator iter;

	for( iter = soln->begin(); iter != soln->end(); ++iter ) 
	{
		Direction dir = *iter;

		if( !canMove(at,dir) )
		{
			return false;
		}

		at = at.move(dir);
	
		//printf("             (row,col)  %d %d \n",at.row, at.col);
	}

	bool results = (at == getEnd());
		
	at = getEnd();

	//printf("  --->   end (row,col)  %d %d \n",at.row, at.col);
	if( soln->size() > 1 )
	{
		printf("    checkSolution(%d elements): passed\n",(int)soln->size());
	}
	else
	{
		printf("    checkSolution(%d elements): FAILED!!\n",(int)soln->size());
	}

	return results;
}
