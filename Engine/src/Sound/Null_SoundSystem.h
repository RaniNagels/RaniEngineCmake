#pragma once
#include <ISoundSystem.h>

namespace REC
{
	class Null_SoundSystem final : public ISoundSystem
	{
	public:
		Null_SoundSystem() = default;
		virtual ~Null_SoundSystem() = default;

		virtual void Destroy() override {}
		virtual void Play(const std::string&, float) override {}
		virtual bool IsPlaying(const std::string&) const override { return false; }
		virtual void Mute() override {};
	};
}