#pragma once

#include "core/resource/resource.h"

class Audio : public Resource
{
public:
	Audio();

	void Load(const char* path) override;
	void Unload() override;

	void Play();
	void Stop();

	void SetLooping(bool looping);

private:
	bool m_looping;

	static bool m_simpleSoundInitialized;
};