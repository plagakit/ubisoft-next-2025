#include "pch.h"
#include "texture.h"

#include "core/debug/assert.h"
#include <App/SimpleSprite.h>
#include <filesystem>

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
