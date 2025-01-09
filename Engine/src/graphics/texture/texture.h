#pragma once

#include "core/resource/resource.h"
#include <memory>

class CSimpleSprite;

/** 
* A wrapper over CSimpleSprite to make it a Resource. Does
* not include CSimpleSprite - if you want to use its methods,
* include it. (https://en.cppreference.com/w/cpp/language/pimpl)
*/
class Texture : public Resource
{
public:
	// Define constructor and destructor as default in texture.cpp to avoid 
	// unique_ptr looking for ~CSimpleSprite() here, where it is forward- 
	// declared and doesn't exist. It will instead look in texture.cpp where 
	// it is included.
	Texture();
	~Texture();

	void Load(const char* path) override;
	void Unload() override;

	CSimpleSprite& Get();
	const CSimpleSprite& Get() const;

private:
	std::unique_ptr<CSimpleSprite> m_sprite;
};