#pragma once

struct Obstacle
{
	enum Type
	{
		WALL,
		WATER
	};

	Type type;
};