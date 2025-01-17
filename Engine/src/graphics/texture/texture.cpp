#include "pch.h"
#include "texture.h"

#include "core/debug/assert.h"
#include <App/SimpleSprite.h>
#include <filesystem>


Texture::Texture(const std::string& path) : 
	Resource(path)
{
	ASSERT_WARN(std::filesystem::exists(path), "Could not find file @ %s!", path);
	m_sprite = std::make_unique<CSimpleSprite>(path);
}

// unique ptr destroys sprite when removed from map
Texture::~Texture() = default;

CSimpleSprite& Texture::Get()
{
	return *m_sprite.get();
}

const CSimpleSprite& Texture::Get() const
{
	return *m_sprite.get();
}
