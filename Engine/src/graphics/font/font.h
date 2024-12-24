#pragma once

#include <string>

#include "math/vector/vector2.h"


struct Font
{
	enum Type
	{
		MONOSPACE_8x13,
		MONOSPACE_9x15,
		HELVETICA_10,
		HELVETICA_12,
		HELVETICA_18,
		TIMES_ROMAN_10,
		TIMES_ROMAN_24
	};

	Type type;

	void* GetGLUTFont() const;
	int GetFontLength(std::string text) const;
	int GetFontHeight() const;

};