#ifndef MAZEMARKERS_H
#define MAZEMARKERS_H

enum class MazeMarkers
{
	// From InternalBit in maze.cpp
	EAST_WALL_BIT = 0x1,
	SOUTH_WALL_BIT = 0x2,
	// From direction markers
	FromNorth = 0x4,
	FromWest = 0x8,
	FromEast = 0x10,
	FromSouth = 0x20,
	// Visited markers
	VisitedTop = 0x40,
	VisitedBottom = 0x80
};

#endif // !MAZEMARKERS_H

