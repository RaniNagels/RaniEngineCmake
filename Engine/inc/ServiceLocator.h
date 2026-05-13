#pragma once
#include <memory>

#include <ISoundSystem.h>

namespace REC
{
	class ServiceLocator final
	{
	public:
		static ISoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem> pSoundSystem);
		
	private:
		static std::unique_ptr<ISoundSystem> m_pSoundSystem;
	};
}