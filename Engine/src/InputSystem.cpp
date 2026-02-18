#include <SDL3/SDL.h>
#include "../inc/InputSystem.h"

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
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			m_ShouldQuit = true;
		}
		if (e.type == SDL_EVENT_KEY_DOWN) {

		}
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {

		}
		// etc...
	}
}


void REC::InputSystem::ProcessKeyboardButtons()
{
}