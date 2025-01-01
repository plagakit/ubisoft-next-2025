#include "pch.h"
#include "font.h"

#include <App/app.h>
#include "core/debug/logger.h"


void* Font::GetGLUTFont() const
{
	switch (type)
	{
	case MONOSPACE_8x13:
		return GLUT_BITMAP_8_BY_13;
	case MONOSPACE_9x15:
		return GLUT_BITMAP_9_BY_15;
	case HELVETICA_10:
		return GLUT_BITMAP_HELVETICA_10;
	case HELVETICA_12:
		return GLUT_BITMAP_HELVETICA_12;
	case HELVETICA_18:
		return GLUT_BITMAP_HELVETICA_18;
	case TIMES_ROMAN_10:
		return GLUT_BITMAP_TIMES_ROMAN_10;
	case TIMES_ROMAN_24:
		return GLUT_BITMAP_TIMES_ROMAN_24;
	}
	return GLUT_BITMAP_8_BY_13;
}

int Font::GetFontLength(std::string text) const
{
	switch (type)
	{
	case MONOSPACE_8x13:
		return 8 * static_cast<int>(text.size());
	case MONOSPACE_9x15:
		return 9 * static_cast<int>(text.size());
	default:
		Logger::Warn("Font %d has a variable width, inaccurate length returned by GetFontLength");
		return 10 * static_cast<int>(text.size());
	}
}

int Font::GetFontHeight() const
{
	switch (type)
	{
	case MONOSPACE_8x13:
		return 14;
	case MONOSPACE_9x15:
		return 16;
	default:
		// TODO: all cases
		return 16;
	}
}