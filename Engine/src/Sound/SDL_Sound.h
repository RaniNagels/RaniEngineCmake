#pragma once
#include "../Resources/ResourceTypes/ISound.h"
#include <memory>

struct MIX_Mixer;
namespace REC
{
	class SDL_Sound final : public ISound
	{
	public:
		SDL_Sound(const std::string& filePath);
		virtual ~SDL_Sound();
		virtual void Destroy() override;
		void Load(MIX_Mixer* mixer);
		virtual void Play(float volume = 1.f, int loops = 0) const override;
		virtual bool IsLoaded() const override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}