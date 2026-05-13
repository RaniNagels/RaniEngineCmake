#pragma once
#include <string>

namespace REC
{
	class ISound
	{
	public:
		virtual ~ISound() = default; // VIRTUAL!!!
		virtual void Destroy() = 0; 
		virtual void Play(float volume = 1.f, int loops = 0) const = 0;
		virtual bool IsLoaded() const = 0;
		virtual bool IsPlaying() const = 0;
	};
}