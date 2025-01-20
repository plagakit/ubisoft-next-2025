#pragma once

#include <engine.h>
#include "game/course/tile.h"

/**
A resource for holding all of a course's tiles and
parts. The course manager uses this to procedurally
generate a full course during gameplay.
*/
class CourseTemplate : public Resource
{
public:
	// See course0.txt for a legend on tiles & characters
	static constexpr const char* TILE_CHARS = "0123456789SH*IE";
	static constexpr int PART_MAX_WIDTH = 128;
	static constexpr int PART_MAX_WALLS = 16;

	struct Part
	{
		std::string m_name;
		std::vector<std::vector<Tile>> m_tilemap;
		bool m_isHole;
		bool m_isStart;
	};

	void Load(const char* path) override;
	void Unload() override;

	const std::vector<Part>& GetParts() const;

private:
	std::vector<Part> m_parts;

	Tile CharToTile(char c);
	char TileToChar(Tile t);
};