#include "ToggleSoundCommand.h"
#include <ServiceLocator.h>

void Game::ToggleSoundCommand::Execute(float)
{
	auto& SS = REC::ServiceLocator::GetSoundSystem();
	SS.Mute();
	
	if (!SS.IsPlaying("BackgroundMusic"))
		SS.Play("BackgroundMusic", 0.5f);
}