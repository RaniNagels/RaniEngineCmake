#include "SDL_SoundSystem.h"

#include <queue>
#include <SDL3_mixer/SDL_mixer.h>
#include <thread>
#include <mutex>

#include "Resources/ResourceTypes/ISound.h"
#include "Resources/ResourceManager.h"
#include "SDL_Sound.h"

// TODO: don't use threading in emscripten
class REC::SDL_SoundSystem::Impl final
{
public:
	Impl()
	{
		if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
			throw std::runtime_error(std::string("Failed to initialize SDL audio subsystem: ") + SDL_GetError());
		
		if (!MIX_Init())
			throw std::runtime_error(std::string("Failed to initialize SDL mixer: ") + SDL_GetError());

		m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		if (!m_pMixer)
			SDL_Log("Couldn't create mixer on default device: %s", SDL_GetError());

#ifndef __EMSCRIPTEN__
		m_SoundThread = std::jthread(&Impl::ProcessQueue, this);
#endif 

	}
	~Impl()
	{
		
	}

	void Destroy()
	{
		{
#ifndef __EMSCRIPTEN__
			std::lock_guard lock(m_QueueMutex);
#endif
			m_Running = false;
		}

#ifndef __EMSCRIPTEN__
		m_Condition.notify_one();
		m_SoundThread.join();
#endif


		if (m_pMixer)
		{
			MIX_DestroyMixer(m_pMixer);
			m_pMixer = nullptr;
		}
		MIX_Quit();
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
	}

	Impl(const Impl&) = delete;
	Impl& operator=(const Impl&) = delete;
	Impl(Impl&&) = delete;
	Impl& operator=(Impl&&) = delete;

	void Play(const std::string& id, float volume)
	{
		ResourceManager& rm = ResourceManager::GetInstance();
		ISound* sound = rm.GetResource<ISound>(id);
		if (!sound->IsLoaded()) // TODO: add an option at resource Creation to load immediatly
		{
			SDL_Sound* sdlSound = dynamic_cast<SDL_Sound*>(sound);
			if (sdlSound)
				sdlSound->Load(m_pMixer);
			else
				throw std::runtime_error("Failed to load sound: " + id);
		}
		// check if the sound is already playing
		

		{
			std::lock_guard lock(m_QueueMutex);
			m_SoundQueue.push({sound, volume});
		}
		m_Condition.notify_one();
	}

	bool IsPlaying(const std::string& id) const
	{
		ResourceManager& rm = ResourceManager::GetInstance();
		ISound* sound = rm.GetResource<ISound>(id);
		if (!sound)
			throw std::runtime_error("Sound not found: " + id);
		return sound->IsPlaying();
	}

private:
	void ProcessQueue()
	{
		while (m_Running)
		{
			SoundEvent event;
			{
				std::unique_lock lock(m_QueueMutex);
				m_Condition.wait(lock, [this] { return !m_SoundQueue.empty() || !m_Running; });

				if (!m_Running && m_SoundQueue.empty())
					return;

				if (m_SoundQueue.empty())
					continue;

				event = m_SoundQueue.front();
				m_SoundQueue.pop();
			}
			
			event.sound->Play(event.volume);
		}
	}

	struct SoundEvent
	{
		ISound* sound;
		float volume;
	};
	std::queue<SoundEvent> m_SoundQueue{};
	MIX_Mixer* m_pMixer;

	// thread
	std::jthread m_SoundThread;
	std::mutex m_QueueMutex;
	std::condition_variable m_Condition;
	bool m_Running{ true };
};

REC::SDL_SoundSystem::SDL_SoundSystem()
	: m_pImpl{ std::make_unique<Impl>() }
{
}

REC::SDL_SoundSystem::~SDL_SoundSystem() = default;

void REC::SDL_SoundSystem::Destroy()
{
	m_pImpl->Destroy();
}

void REC::SDL_SoundSystem::Play(const std::string& id, float volume)
{
	m_pImpl->Play(id, volume);
}

bool REC::SDL_SoundSystem::IsPlaying(const std::string& id) const
{
	return m_pImpl->IsPlaying(id);
}
