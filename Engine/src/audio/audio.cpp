#include "pch.h"
#include "audio.h"

#include "core/debug/logger.h"
#include "core/app_settings.h"

#ifdef PLATFORM_WINDOWS
#include <App/app.h>
#endif

bool Audio::m_simpleSoundInitialized = false;

Audio::Audio() :
	m_looping(false)
{}

void Audio::Load(const char* path)
{
	//if (!m_simpleSoundInitialized)
	//{
	//	CSimpleSound::GetInstance().Initialize();
	//	m_simpleSoundInitialized = true;
	//}
}

void Audio::Unload()
{
#ifdef PLATFORM_WINDOWS
	//CSimpleSound::GetInstance().StopSound(m_path.c_str());
	App::StopSound(m_path.c_str());
#endif
}

void Audio::Play()
{
#ifdef PLATFORM_WINDOWS
	//SoundFlags flags = m_looping ? SoundFlags::Looping : SoundFlags::None;
	//bool result = CSimpleSound::GetInstance().StartSound(m_path.c_str(), flags);
	App::PlaySoundW(m_path.c_str(), m_looping);
#endif
}

void Audio::Stop()
{
#ifdef PLATFORM_WINDOWS
	//SoundFlags flags = m_looping ? SoundFlags::Looping : SoundFlags::None;
	//bool result = CSimpleSound::GetInstance().StartSound(m_path.c_str(), flags);
	App::StopSound(m_path.c_str());
#endif
}

void Audio::SetLooping(bool looping)
{
	m_looping = looping;
}
