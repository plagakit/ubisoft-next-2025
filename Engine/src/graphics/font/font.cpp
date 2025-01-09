#include "pch.h"
#include "font.h"

#include <App/app.h>
#include "core/debug/logger.h"


void Font::Load(const char* path)
{
	m_type = PathToType(path);

	// TODO: all font height cases
	if		(m_type == MONOSPACE_8x13)	m_charHeight = 14;
	else if (m_type == MONOSPACE_9x15)	m_charHeight = 16;
	else								m_charHeight = 16;
}

void Font::Unload()
{
}

Font::Type Font::GetType() const
{
	return m_type;
}

void* Font::GetGLUTFont() const
{
	switch (m_type)
	{
	case MONOSPACE_8x13:	return GLUT_BITMAP_8_BY_13;
	case MONOSPACE_9x15:	return GLUT_BITMAP_9_BY_15;
	case HELVETICA_10:		return GLUT_BITMAP_HELVETICA_10;
	case HELVETICA_12:		return GLUT_BITMAP_HELVETICA_12;
	case HELVETICA_18:		return GLUT_BITMAP_HELVETICA_18;
	case TIMES_ROMAN_10:	return GLUT_BITMAP_TIMES_ROMAN_10;
	case TIMES_ROMAN_24:	return GLUT_BITMAP_TIMES_ROMAN_24;
	}
	return GLUT_BITMAP_8_BY_13;
}

int Font::GetFontLength(std::string text) const
{
	switch (m_type)
	{
	case MONOSPACE_8x13:
		return 8 * static_cast<int>(text.size());
	case MONOSPACE_9x15:
		return 9 * static_cast<int>(text.size());
	default:
		Logger::Warn("Font type #%d has a variable width, inaccurate length returned by GetFontLength", m_type);
		return 10 * static_cast<int>(text.size());
	}
}

int Font::GetFontHeight() const
{
	return m_charHeight;
}

Font::Type Font::PathToType(const char* path)
{
	std::string str{ path };
	if		(str == "MONOSPACE_8x13")	return MONOSPACE_8x13;
	else if (str == "MONOSPACE_9x15")	return MONOSPACE_9x15;
	else if (str == "HELVETICA_10")		return HELVETICA_10;
	else if (str == "HELVETICA_12")		return HELVETICA_12;
	else if (str == "HELVETICA_18")		return HELVETICA_18;
	else if (str == "TIMES_ROMAN_10")	return TIMES_ROMAN_10;
	else if (str == "TIMES_ROMAN_24")	return TIMES_ROMAN_24;
	Logger::Warn("'%s' is not a valid font name!", path);
	return MONOSPACE_8x13;
}
