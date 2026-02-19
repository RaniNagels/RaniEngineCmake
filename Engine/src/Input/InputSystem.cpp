#include <SDL3/SDL.h>
#include "../../inc/Input/InputSystem.h"

REC::InputSystem::InputSystem()
{

}

void REC::InputSystem::ProcessInput()
{
	DetectInput();
	ProcessKeyboardButtons();
}

void REC::InputSystem::DetectInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		switch (e.type)
		{
		case SDL_EVENT_QUIT:
			m_ShouldQuit = true;
			break;
		case SDL_EVENT_KEY_UP:
			break;
		case SDL_EVENT_KEY_DOWN:
			break;
		}
	}
}

void REC::InputSystem::DetectControllerInput(int)
{

}

void REC::InputSystem::ProcessKeyboardButtons()
{
}