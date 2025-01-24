#pragma once

#include "core/resource/resource.h"

/**
A resource that stores an Audio (on Windows it
stores the path to the sound and plays it using
SimpleSound w/o exposing <App> to any users.)
*/
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