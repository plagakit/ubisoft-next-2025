#pragma once

#include "core/resource/resource.h"
#include <memory>

class CSimpleSprite;

/** 
* A wrapper over CSimpleSprite to make it a Resource. Makes
* so that anything that wants to use a texture doesn't necessarily
* have to include everything in App.
* 
* Resources:
* https://en.cppreference.com/w/cpp/language/pimpl
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

	float GetWidth() const;
	float GetHeight() const;

	// TODO: find a better solution for this
	void SetSheetDimensions(int rows, int cols);
	void SetFrame(unsigned int f);

private:
	std::unique_ptr<CSimpleSprite> m_sprite;
};