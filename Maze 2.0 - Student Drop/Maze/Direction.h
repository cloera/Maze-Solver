#ifndef DIRECTION_MAZE_H
#define DIRECTION_MAZE_H

#include <assert.h>


enum class Direction
{
	North, 
	East, 
	South, 
	West,
	Uninitialized
};

Direction reverseDir( Direction dir );



class DirectionHelper
{
public:
	static char *getString( Direction dir );

};


#endif