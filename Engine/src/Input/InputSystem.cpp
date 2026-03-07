#include "../../inc/Input/InputSystem.h"
#include <SDL3/SDL.h>

#include "InputDevices/Keyboard.h"
#include "InputDevices/Controller.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <Xinput.h>
#endif

#include <cmath>

class REC::InputSystem::Impl
{
public:
	Impl()
		: m_Keyboard{ std::make_unique<Keyboard>() }
		, m_MAX_AMOUNT_OF_CONTROLLERS{4}
	{
		for (uint8_t i{}; i < m_MAX_AMOUNT_OF_CONTROLLERS; ++i)
		{
			m_Controllers.emplace_back(std::make_unique<Controller>(i));
		}
	}

	bool ProcessInput()
	{
		ResetStates();
		bool shouldQuit = DetectInput();
		return shouldQuit;
	}

	Keyboard* GetKeyboard() const { return m_Keyboard.get(); }
	std::vector<Controller*> GetControllers()
	{
		std::vector<Controller*> controllers;
		controllers.reserve(m_Controllers.size());
		for (const auto& c : m_Controllers)
			controllers.emplace_back(c.get());
		return controllers;
	}

private:
	void ResetStates()
	{
		m_Keyboard->ResetState();
		for (const auto& c : m_Controllers)
			c->ResetState();
	}

	bool DetectInput()
	{
		bool shouldQuit{ false };

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_EVENT_QUIT:
				shouldQuit = true;
				break;
			case SDL_EVENT_KEY_UP:
				m_Keyboard->UpdateState(static_cast<uint8_t>(e.key.scancode), false);
				break;
			case SDL_EVENT_KEY_DOWN:
				m_Keyboard->UpdateState(static_cast<uint8_t>(e.key.scancode), true);
				break;
			}

		}
#if defined(_WIN32)
		// Controller
		for (size_t i{}; i < m_Controllers.size(); ++i)
		{
			XINPUT_STATE state{};
			if (!DetectControllerInput(i, state))
				continue;

			m_Controllers[i]->UpdateState(&state);
		}
#endif

		return shouldQuit;
	}

#if defined(_WIN32)
	bool DetectControllerInput(size_t index, XINPUT_STATE& out)
	{
		XINPUT_STATE state{};
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(DWORD(index), &state) != ERROR_SUCCESS)
			return false;

		CopyMemory(&out, &state, sizeof(XINPUT_STATE));

		ApplyThumbDeadZone(out.Gamepad.sThumbLX, out.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		ApplyThumbDeadZone(out.Gamepad.sThumbRX, out.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

		if (out.Gamepad.bLeftTrigger  < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) out.Gamepad.bLeftTrigger = 0;
		if (out.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) out.Gamepad.bRightTrigger = 0;

		return true;
	}
#endif

	static void ApplyThumbDeadZone(int16_t & x, int16_t & y, int deadzone)
	{
		float fx = static_cast<float>(x);
		float fy = static_cast<float>(y);
	
		float magnitude = float(std::sqrt(fx * fx + fy * fy));
	
		if (magnitude < deadzone)
		{
			x = 0;
			y = 0;
			return;
		}
	}

	std::unique_ptr<Keyboard> m_Keyboard;
	std::vector<std::unique_ptr<Controller>> m_Controllers;

	const uint8_t m_MAX_AMOUNT_OF_CONTROLLERS;
};

REC::InputSystem::InputSystem() : m_Impl{std::make_unique<Impl>()} { }

REC::InputSystem::~InputSystem() = default;

void REC::InputSystem::ProcessInput()
{
	m_ShouldQuit = m_Impl->ProcessInput();
	HandleInput();
}

REC::InputBinding* REC::InputSystem::CreateInputBinding()
{
	return m_Bindings.emplace_back(std::make_unique<InputBinding>()).get();
}

void REC::InputSystem::RemoveInputBinding(InputBinding* inputBinding)
{
	std::erase_if(m_Bindings, [inputBinding](const std::unique_ptr<InputBinding>& binding)
		{
			return binding.get() == inputBinding;
		});
}

void REC::InputSystem::HandleInput()
{
	std::vector<IInputAction*> actions;
	for (auto& binding : m_Bindings)
	{
		// --- keyboard ----------------------------------------------------------------------------------
		actions = binding->GetInputActions(InputActionType::KeyboardButton);
		for (auto* action : actions)
		{
			auto* keyboardAction = static_cast<KeyboardButtonAction*>(action);
			switch (keyboardAction->state)
			{
			case ButtonState::Pressed:
				if (m_Impl->GetKeyboard()->IsPressed(keyboardAction->button) ||
					m_Impl->GetKeyboard()->IsDownThisFrame(keyboardAction->button)) // ensure the first time it is pressed is also accounted for
					binding->Execute();
				break;
			case ButtonState::Down:
				if (m_Impl->GetKeyboard()->IsDownThisFrame(keyboardAction->button))
					binding->Execute();
				break;
			case ButtonState::Up:
				if (m_Impl->GetKeyboard()->IsUpThisFrame(keyboardAction->button))
					binding->Execute();
				break;
			}
		}

		// --- Controllers -------------------------------------------------------------------------------
		for (auto* controller : m_Impl->GetControllers())
		{
			// --- Controller - Buttons ------------------------------------------------------------------
			actions = binding->GetInputActions(InputActionType::ControllerButton);
			for (auto* action : actions)
			{
				auto* controllerButtonAction = static_cast<ControllerButtonAction*>(action);
				switch (controllerButtonAction->state)
				{
				case ButtonState::Pressed:
					if (controller->IsPressed(controllerButtonAction->button) || \
						controller->IsDownThisFrame(controllerButtonAction->button)) // ensure the first time it is pressed is also accounted for
						binding->Execute(controller->GetID());
					break;
				case ButtonState::Down:
					if (controller->IsDownThisFrame(controllerButtonAction->button))
						binding->Execute(controller->GetID());
					break;
				case ButtonState::Up:
					if (controller->IsUpThisFrame(controllerButtonAction->button))
						binding->Execute(controller->GetID());
					break;
				}
			}

			// --- Controller - Ranges -------------------------------------------------------------------
			// TODO: currently only check if range is active, does not pass the value (yet)
			actions = binding->GetInputActions(InputActionType::ControllerRange);
			for (auto* action : actions)
			{
				auto* controllerRangeAction = static_cast<ControllerRangeAction*>(action);
				if (controller->IsRangeActive(controllerRangeAction->range))
				{
					binding->Execute(controller->GetID(), controller->GetRange(controllerRangeAction->range));
				}
			}
		}
	}
}
