#include "SDL_Sound.h"

#include <SDL3_mixer/SDL_mixer.h>
#include <stdexcept>
#include <algorithm>

class REC::SDL_Sound::Impl final
{
public:
	Impl(const std::string& filepath) : m_FilePath(filepath)
	{
		
	}
	~Impl() = default;

	void Destroy()
	{
		if (m_pAudio)
		{
			MIX_DestroyTrack(m_pTrack);
			m_pTrack = nullptr;

			MIX_DestroyAudio(m_pAudio);
			m_pAudio = nullptr;
		}
	}

	void Load(MIX_Mixer* mixer)
	{
		if (!m_pTrack)
			m_pTrack = MIX_CreateTrack(mixer);
		m_pAudio = MIX_LoadAudio(mixer, m_FilePath.c_str(), true);
		if (!m_pAudio)
			throw std::runtime_error(std::string("Failed to load audio file: ") + SDL_GetError());
	}

	// TODO: revise
	//        volume, loops
	void Play(float volume, int loops) const
	{
		if (!m_pTrack || !m_pAudio)
			throw std::runtime_error("Audio not loaded or mixer not set.");

		MIX_SetTrackAudio(m_pTrack, m_pAudio);
		MIX_SetTrackLoops(m_pTrack, loops);
		MIX_SetTrackGain(m_pTrack, volume);
		if (!MIX_PlayTrack(m_pTrack, 0))
			throw std::runtime_error(std::string("Failed to play audio: ") + SDL_GetError());
	}

	bool IsLoaded() const
	{
		return m_pTrack != nullptr;
	}

private:
	const std::string m_FilePath;
	MIX_Audio* m_pAudio{ nullptr };
	MIX_Track* m_pTrack{ nullptr };
};

REC::SDL_Sound::SDL_Sound(const std::string& filePath)
	: m_pImpl(std::make_unique<Impl>(filePath))
{
}

REC::SDL_Sound::~SDL_Sound()
{
	m_pImpl->Destroy();
}

void REC::SDL_Sound::Destroy()
{
	m_pImpl->Destroy();
}

void REC::SDL_Sound::Load(MIX_Mixer* mixer)
{
	m_pImpl->Load(mixer);
}

void REC::SDL_Sound::Play(float volume, int loops) const
{
	m_pImpl->Play(volume, loops);
}

bool REC::SDL_Sound::IsLoaded() const
{
	return m_pImpl->IsLoaded();
}
