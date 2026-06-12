#include "ToggleSoundCommand.h"
#include <ServiceLocator.h>

void Game::ToggleSoundCommand::Execute(float)
{
	auto& SS = REC::ServiceLocator::GetSoundSystem();
	SS.Mute();
}