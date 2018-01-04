#include "Direction.h"

Direction reverseDir( Direction dir )
{
	Direction Val;

	switch( dir )
		{
		case Direction::East:
			Val = Direction::West;
			break;
		case Direction::West:
			Val = Direction::East;
			break;
		case Direction::South:
			Val = Direction::North;
			break;
		case Direction::North:
			Val = Direction::South;
			break;
		default:
			assert(false);
			break;
		}

	return Val;
}


char *DirectionHelper::getString( Direction dir )
{
	char *s = 0;
	switch( dir )
	{
	case Direction::East:
		s = "East";
		break;
	case Direction::West:
		s = "West";
		break;
	case Direction::South:
		s = "South";
		break;
	case Direction::North:
		s = "North";
		break;
	case Direction::Uninitialized:
		s = "Uninitialized";
		break;
	default:
		assert(false);
		
		break;
	}
	return s;
}
