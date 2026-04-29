#pragma once
#include <memory>
#include <ISoundSystem.h>

namespace REC
{
	class ServiceLocator final
	{
	public:
		static ISoundSystem& GetSoundSystem() 
		{ 
			if (!m_pSoundSystem)
				throw std::runtime_error("Sound system not registered in service locator.");
			return *m_pSoundSystem; 
		}
		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem> pSoundSystem) { m_pSoundSystem = std::move(pSoundSystem); }
	private:
		inline static std::unique_ptr<ISoundSystem> m_pSoundSystem = nullptr;
	};
}