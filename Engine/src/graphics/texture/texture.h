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
	Texture(const std::string& path);
	~Texture() override;

	CSimpleSprite& Get();
	const CSimpleSprite& Get() const;

private:
	std::unique_ptr<CSimpleSprite> m_sprite;
};