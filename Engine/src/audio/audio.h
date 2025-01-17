#pragma once

#include "core/resource/resource.h"

class Audio : public Resource
{
public:
	Audio(const std::string& path, bool looping);
	~Audio() override;

	void Play();

	void SetLooping(bool looping);

private:
	bool m_looping;

	static bool m_simpleSoundInitialized;
};