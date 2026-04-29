#pragma once
#include <ISoundSystem.h>

namespace REC
{
	// not used, SDL_SoundSystem is the default sound system, given upon initialization of the engine
	class Null_SoundSystem final : public ISoundSystem
	{
	public:
		Null_SoundSystem() = default;
		virtual ~Null_SoundSystem() = default;
		virtual void Play(SoundID, float) override {}
	};
}