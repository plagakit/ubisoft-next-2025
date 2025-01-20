#include "course_template.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>

void CourseTemplate::Load(const char* path)
{
	std::ifstream file(path);
	ASSERT_ERROR(file, "Error loading course w/ path %s", path);

	std::string partName;
	std::vector<std::vector<Tile>> currentPart;
	bool isHole = false;
	bool isStart = false;

	while (!file.eof())
	{
		char line[PART_MAX_WIDTH];
		file.getline(line, PART_MAX_WIDTH);
		if (line[0] == '.')
			continue;

		// Part delimiter
		if (line[0] == '-')
		{
			m_parts.push_back({
				std::move(partName),
				std::move(currentPart),
				isHole,
				isStart
			});
			//m_parts.emplace_back(std::move(partName), std::move(currentPart), isHole);

			partName.clear();
			currentPart.clear();
			isHole = false;
			isStart = false;
			continue;
		}

		// Part name is always first line
		if (partName.empty())
		{
			partName = line;
			continue;
		}

		std::vector<Tile> row;
		for (char c : line)
		{
			Tile t = CharToTile(c);
			if (t.type == Tile::Type::HOLE)
				isHole = true;
			else if (t.type == Tile::Type::START)
				isStart = true;
			else if (t.type == Tile::Type::UNKNOWN)
				break;

			row.push_back(t);
		}
		currentPart.push_back(std::move(row));
	}

	//for (const auto& p : m_parts) 
	//{
	//	std::cout << "Part: " << p.m_name << std::endl;
	//	for (int i = 0; i < p.m_tilemap.size(); i++)
	//	{
	//		std::cout << "R" << i << "\t: ";
	//		for (Tile tile : p.m_tilemap[i])
	//			std::cout << TileToChar(tile) << ' ';
	//		std::cout << std::endl;
	//	}
	//}
}

void CourseTemplate::Unload()
{

}

const std::vector<CourseTemplate::Part>& CourseTemplate::GetParts() const
{
	return m_parts;
}

Tile CourseTemplate::CharToTile(char c)
{
	Tile tile;
	switch (c)
	{
	case 'S': tile.type = Tile::Type::START; break;
	case ' ': tile.type = Tile::Type::BLANK; break;
	case 'H': tile.type = Tile::Type::HOLE;	 break;
	case '*': tile.type = Tile::Type::WATER; break;
	case 'I': tile.type = Tile::Type::ENTER; break;
	case 'X': tile.type = Tile::Type::EXIT;	 break;
	case '!': tile.type = Tile::Type::POWERUP_SPAWN; break;
	default:
	{
		// Walls 0-F
		char hex = std::toupper(c);
		if (hex >= '0' && hex <= '9')
		{
			tile.type = Tile::Type::WALL;
			tile.wallIndex = static_cast<int>(hex - '0');
		}
		else if (hex >= 'A' && hex <= 'F')
		{
			tile.type = Tile::Type::WALL;
			tile.wallIndex = static_cast<int>(hex - 'A' + 10);
		}
		else
		{
			tile.type = Tile::Type::UNKNOWN;
		}
	}
	}

	return tile;
}

char CourseTemplate::TileToChar(Tile tile)
{
	// TODO: reimplement

	//switch (tile)
	//{
	//case Tile::START: return 'S';
	//case Tile::BLANK: return ' ';
	//case Tile::HOLE: return 'H';
	//case Tile::WATER: return '*';
	//case Tile::ENTER: return 'I';
	//case Tile::EXIT: return 'E';
	//case Tile::WALL: return '0';
	//default: return '?';
	//}

	return '?';
}

