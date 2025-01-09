#pragma once

#include "core/resource/resource.h"

class Font : public Resource
{
public:
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

	void Load(const char* path) override;
	void Unload() override;

	Type GetType() const;
	void* GetGLUTFont() const;
	int GetFontLength(std::string text) const;
	int GetFontHeight() const;

private:
	Type m_type;
	int m_charHeight;

	Type PathToType(const char* path);

};