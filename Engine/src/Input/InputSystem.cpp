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
#include <EngineContext.h>

class REC::InputSystem::Impl
{
public:
	Impl()
		: m_Keyboard{ std::make_unique<Keyboard>() }
		, m_MAX_AMOUNT_OF_CONTROLLERS{4}
	{
		for (uint8_t i{}; i < m_MAX_AMOUNT_OF_CONTROLLERS; ++i)
			m_Controllers.emplace_back(std::make_unique<Controller>(i));
		m_AmountOfActiveControllers = m_MAX_AMOUNT_OF_CONTROLLERS;
	}

	void ProcessInput(float deltaTime)
	{
		ResetStates();
		m_ShouldQuit = DetectInput();
		HandleInput(deltaTime);
	}
	bool ShouldQuit() const { return m_ShouldQuit; }

	InputBinding* CreateInputBinding()
	{
		return m_Bindings.emplace_back(std::make_unique<InputBinding>()).get();
	}
	void RemoveInputBinding(InputBinding* inputBinding)
	{
		std::erase_if(m_Bindings, [inputBinding](const std::unique_ptr<InputBinding>& binding)
			{
				return binding.get() == inputBinding;
			});
	}
	void SetActiveControllers(uint8_t amountOfActiveControllers)
	{
		if (amountOfActiveControllers <= m_MAX_AMOUNT_OF_CONTROLLERS)
			m_AmountOfActiveControllers = amountOfActiveControllers;
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

	void HandleInput(float deltaTime)
	{
		for (auto& binding : m_Bindings)
		{
			HandleKeyboard(deltaTime, binding.get());

			// --- Controllers -------------------------------------------------------------------------------
			for (uint8_t controllerID{}; controllerID < m_AmountOfActiveControllers; ++controllerID)
			{
				HandleControllerButtons(deltaTime, binding.get(), m_Controllers[controllerID].get());
				HandleControllerRanges(deltaTime, binding.get(), m_Controllers[controllerID].get());
			}
		}
	}
	void HandleKeyboard(float deltaTime, InputBinding* binding)
	{
		auto actions = binding->GetInputActions(InputActionType::KeyboardButton);
		for (auto* action : actions)
		{
			auto* keyboardAction = static_cast<KeyboardButtonAction*>(action);
			switch (keyboardAction->state)
			{
			case ButtonState::Pressed:
				if (m_Keyboard.get()->IsPressed(keyboardAction->button) ||
					m_Keyboard.get()->IsDownThisFrame(keyboardAction->button)) // ensure the first time it is pressed is also accounted for
					binding->Execute(deltaTime);
				break;
			case ButtonState::Down:
				if (m_Keyboard.get()->IsDownThisFrame(keyboardAction->button))
					binding->Execute(deltaTime);
				break;
			case ButtonState::Up:
				if (m_Keyboard.get()->IsUpThisFrame(keyboardAction->button))
					binding->Execute(deltaTime);
				break;
			}
		}
	}
	void HandleControllerButtons(float deltaTime, InputBinding* binding, Controller* controller)
	{
		auto actions = binding->GetInputActions(InputActionType::ControllerButton);
		for (auto* action : actions)
		{
			auto* controllerButtonAction = static_cast<ControllerButtonAction*>(action);
			if (controllerButtonAction->playerIndex == controller->GetID())
			{
				switch (controllerButtonAction->state)
				{
				case ButtonState::Pressed:
					if (controller->IsPressed(controllerButtonAction->button) || \
						controller->IsDownThisFrame(controllerButtonAction->button)) // ensure the first time it is pressed is also accounted for
						binding->Execute(deltaTime);
					break;
				case ButtonState::Down:
					if (controller->IsDownThisFrame(controllerButtonAction->button))
						binding->Execute(deltaTime);
					break;
				case ButtonState::Up:
					if (controller->IsUpThisFrame(controllerButtonAction->button))
						binding->Execute(deltaTime);
					break;
				}
			}
		}
	}
	void HandleControllerRanges(float deltaTime, InputBinding* binding, Controller* controller)
	{
		auto actions = binding->GetInputActions(InputActionType::ControllerRange);
		for (auto* action : actions)
		{
			auto* controllerRangeAction = static_cast<ControllerRangeAction*>(action);
			if (controllerRangeAction->playerIndex == controller->GetID())
			{
				if (controller->IsRangeActive(controllerRangeAction->range))
				{
					binding->Execute(deltaTime, controller->GetRange(controllerRangeAction->range));
				}
			}
		}
	}

	std::unique_ptr<Keyboard> m_Keyboard;
	std::vector<std::unique_ptr<Controller>> m_Controllers;
	uint8_t m_AmountOfActiveControllers{};

	const uint8_t m_MAX_AMOUNT_OF_CONTROLLERS;

	std::vector<std::unique_ptr<InputBinding>> m_Bindings{};
	bool m_ShouldQuit{ false };
};

REC::InputSystem::InputSystem() : m_Impl{std::make_unique<Impl>()} { }

REC::InputSystem::~InputSystem() = default;

void REC::InputSystem::ProcessInput(float deltaTime)
{
	m_Impl->ProcessInput(deltaTime);
}

bool REC::InputSystem::ShouldQuit() const
{
	return m_Impl->ShouldQuit();
}

REC::InputBinding* REC::InputSystem::CreateInputBinding()
{
	return m_Impl->CreateInputBinding();
}

void REC::InputSystem::RemoveInputBinding(InputBinding* inputBinding)
{
	m_Impl->RemoveInputBinding(inputBinding);
}

void REC::InputSystem::SetNumberOfActiveControllers(uint8_t num)
{
	m_Impl->SetActiveControllers(num);
}
