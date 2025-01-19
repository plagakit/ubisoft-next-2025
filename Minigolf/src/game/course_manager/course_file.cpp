#include "course_file.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>

char tileToChar(Course::Tile tile) {
	switch (tile) {
	case Course::Tile::START: return 'S';
	case Course::Tile::BLANK: return ' ';
	case Course::Tile::HOLE: return 'H';
	case Course::Tile::WATER: return '*';
	case Course::Tile::ENTER: return 'I';
	case Course::Tile::EXIT: return 'E';
	case Course::Tile::WALL: return '0'; // Walls are represented by '0' in the original example
	default: return '?'; // Unknown tile
	}
}


void Course::Load(const char* path)
{
	std::ifstream file(path);
	ASSERT_ERROR(file, "Error loading course w/ path %s", path);

	std::string partName;
	std::unordered_map<std::string, std::vector<std::vector<Tile>>> courseParts;
	std::vector<std::vector<Tile>> currentPart;

	// See course0.txt for a legend on tiles & characters
	static const char* TILE_CHARS = "0123456789SH*IE";
	constexpr int MAX_WIDTH = 128;

	while (!file.eof()) 
	{
		char line[MAX_WIDTH];
		file.getline(line, MAX_WIDTH);
		if (line[0] == '.') 
			continue;

		// Part delimiter
		if (line[0] == '-')
		{
			courseParts[partName] = currentPart;
			partName.clear();
			currentPart.clear();
			continue;
		}

		// Parse part name (first line of each part)
		if (partName.empty()) 
		{
			partName = line;
			continue;
		}

		// Process part content
		std::vector<Tile> row;
		for (char c : line)
		{
			Tile t = CharToTile(c);
			if (t == Tile::END)
				break;

			row.push_back(t);
		}
		currentPart.push_back(row);
	}

	// Display parsed course parts
	for (const auto& [name, part] : courseParts) 
	{
		std::cout << "Part: " << name << std::endl;
		for (int i = 0; i < part.size(); i++)
		{
			std::cout << "R" << i << "\t: ";
			for (Tile tile : part[i])
				std::cout << tileToChar(tile) << ' ';
			std::cout << std::endl;
		}
	}
}

void Course::Unload()
{

}

Course::Tile Course::CharToTile(char c)
{
	switch (c) 
	{
	case 'S': return START;
	case ' ': return BLANK;
	case 'H': return HOLE;
	case '*': return WATER;
	case 'I': return ENTER;
	case 'E': return EXIT;
	default:
	{
		if (c >= '0' && c <= '9')
			return WALL;
		return END;
	}
	}
}

