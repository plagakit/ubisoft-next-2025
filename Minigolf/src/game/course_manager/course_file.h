#pragma once

#include <engine.h>

class Course : public Resource
{
public:
	enum Tile
	{
		START, WALL, BLANK, HOLE, WATER, ENTER, EXIT, END
	};

	void Load(const char* path) override;
	void Unload() override;

private:
	Tile CharToTile(char c);
};