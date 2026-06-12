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

		SDL_SoundSystem(const SDL_SoundSystem&) = delete;
		SDL_SoundSystem(SDL_SoundSystem&&) noexcept = delete;
		SDL_SoundSystem& operator=(const SDL_SoundSystem&) = delete;
		SDL_SoundSystem& operator=(SDL_SoundSystem&&) noexcept = delete;

		virtual void Destroy() override;
		virtual void Play(const std::string& id, float volume) override;
		virtual bool IsPlaying(const std::string& id) const override;
		virtual void Mute() override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}