#include "pch.h"
#include "texture.h"

#include "core/debug/assert.h"
#include <filesystem>
#ifdef PLATFORM_WINDOWS
#include <App/SimpleSprite.h>
#endif

Texture::Texture() = default;
Texture::~Texture() = default;

void Texture::Load(const char* path)
{
	ASSERT_WARN(std::filesystem::exists(path), "Could not find file @ %s!", path);
	m_sprite = std::make_unique<CSimpleSprite>(path);
}

void Texture::Unload()
{
	// unique ptr destroys sprite when removed from map
}

CSimpleSprite& Texture::Get()
{
	return *m_sprite.get();
}

const CSimpleSprite& Texture::Get() const
{
	return *m_sprite.get();
}

float Texture::GetWidth() const
{
	return m_sprite->GetWidth();
}

float Texture::GetHeight() const
{
	return m_sprite->GetHeight();
}

void Texture::SetSheetDimensions(int rows, int cols)
{
	m_sprite = std::make_unique<CSimpleSprite>(m_path.c_str(), cols, rows);
}

void Texture::SetFrame(unsigned int f)
{
	m_sprite->SetFrame(f);
}
