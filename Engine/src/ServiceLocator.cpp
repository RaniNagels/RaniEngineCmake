#include <ServiceLocator.h>

#include "Sound/Null_SoundSystem.h"

#include <stdexcept>

std::unique_ptr<REC::ISoundSystem> REC::ServiceLocator::m_pSoundSystem = std::make_unique<REC::Null_SoundSystem>();

REC::ISoundSystem& REC::ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSystem;
}

void REC::ServiceLocator::RegisterSoundSystem(std::unique_ptr<ISoundSystem> pSoundSystem)
{
	m_pSoundSystem = std::move(pSoundSystem);
}