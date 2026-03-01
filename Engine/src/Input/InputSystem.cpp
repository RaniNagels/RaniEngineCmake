#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>

#include "../../inc/Input/InputSystem.h"
#include <SDL3/SDL.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <Xinput.h>
#endif

REC::InputSystem::InputSystem()
	: m_Keyboard{std::make_unique<Keyboard>()}
{
	m_Controllers.emplace_back(std::make_unique<Controller>(static_cast<uint8_t>(0)));
	m_Controllers.emplace_back(std::make_unique<Controller>(static_cast<uint8_t>(1)));
}

void REC::InputSystem::ProcessInput()
{
	ResetStates();
	DetectInput();
	ProcessKeyboardButtons();
}

void REC::InputSystem::ResetStates()
{
	m_Keyboard->ResetState();
	for (const auto& c : m_Controllers)
		c->ResetState();
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
			m_Keyboard->UpdateState(static_cast<uint8_t>(e.key.scancode), false);
			break;
		case SDL_EVENT_KEY_DOWN:
			m_Keyboard->UpdateState(static_cast<uint8_t>(e.key.scancode), true);
			break;
		}

		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	// Controller
	for (size_t i{}; i < m_Controllers.size(); ++i)
	{
		ControllerState state{};
		if (!DetectControllerInput(i, state))
			continue;

		m_Controllers[i]->UpdateState(state);
	}
}

bool REC::InputSystem::DetectControllerInput([[maybe_unused]] size_t index, [[maybe_unused]] ControllerState& out)
{
#if defined(_WIN32)
	XINPUT_STATE state{};
	if (XInputGetState(DWORD(index), &state) != ERROR_SUCCESS)
		return false;

	out.packetNumber = state.dwPacketNumber;
	out.buttons = state.Gamepad.wButtons;

	out.leftTrigger = state.Gamepad.bLeftTrigger;
	out.rightTrigger = state.Gamepad.bRightTrigger;

	out.thumbLX = state.Gamepad.sThumbLX;
	out.thumbLY = state.Gamepad.sThumbLY;
	out.thumbRX = state.Gamepad.sThumbRX;
	out.thumbRY = state.Gamepad.sThumbRY;

	ApplyThumbDeadZone(out.thumbLX, out.thumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	ApplyThumbDeadZone(out.thumbRX, out.thumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

	if (out.leftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) out.leftTrigger = 0;
	if (out.rightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) out.rightTrigger = 0;

	return true;
#else
	return false;
#endif
}

void REC::InputSystem::ProcessKeyboardButtons()
{
}

void REC::InputSystem::ProcessRanges()
{
	
}

void REC::InputSystem::ProcessControllerButtons()
{
	
}