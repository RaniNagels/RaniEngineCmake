#pragma once
#include <string>

namespace REC
{
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void Destroy() = 0;
		virtual void Play(const std::string& id, float volume) = 0;
	};
}