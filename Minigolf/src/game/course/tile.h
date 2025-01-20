#pragma once

/**
The different tiles that can be part
of a course during gameplay.
*/
struct Tile
{
	enum class Type
	{
		BLANK,
		START, HOLE,
		ENTER, EXIT,
		WALL,
		WATER,
		POWERUP_SPAWN,
		UNKNOWN
	};

	Type type		= Type::UNKNOWN;
	int wallIndex	= -1;
};