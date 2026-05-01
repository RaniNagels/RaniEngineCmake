#pragma once
#include <memory>

#include <ISoundSystem.h>

namespace REC
{
	// TODO: create resource ID's instead of using strings
	class SDL_SoundSystem final : public ISoundSystem
	{
	public:
		SDL_SoundSystem();
		~SDL_SoundSystem();
		virtual void Destroy() override;
		virtual void Play(const std::string& id, float volume) override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}