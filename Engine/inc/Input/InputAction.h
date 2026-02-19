#pragma once
#include "Input.h"
#include "ButtonState.h"

namespace REC
{
	// this is used to quickly identify the action in the binding
	enum class InputActionType
	{
		ControllerRange,
		ControllerButton,
		KeyboardButton
	};

	struct IInputAction
	{
		virtual ~IInputAction() = default;
		virtual InputActionType GetType() = 0;
	};

	struct ControllerButtonAction final : IInputAction
	{
		ControllerButtonAction(Input::Controller::Button btn, ButtonState st = ButtonState::Pressed, uint8_t plIdx = -1)
			: button{ btn }, state{ st }, playerIndex{ plIdx }
		{
		}

		InputActionType GetType() const override { return InputActionType::ControllerButton; }

		Input::Controller::Button button;
		ButtonState state;
		uint8_t playerIndex; // -1 is all players
	};

	struct KeyboardButtonAction final : IInputAction
	{
		KeyboardButtonAction(Input::Keyboard::Button btn, ButtonState st = ButtonState::Pressed, uint8_t plIdx = -1)
			: button{ btn }, state{ st }, playerIndex{ plIdx }
		{
		}

		InputActionType GetType() const override { return InputActionType::KeyboardButton; }

		Input::Keyboard::Button button;
		ButtonState state;
		uint8_t playerIndex; // -1 is all players
	};

	struct ControllerRangeAction final : IInputAction
	{
		ControllerRangeAction(Input::Controller::Range rg, uint8_t plIdx = -1)
			: range{ rg }, playerIndex{ plIdx }
		{
		}

		InputActionType GetType() const override { return InputActionType::ControllerRange; }

		Input::Controller::Range range;
		uint8_t playerIndex; // -1 is all players
	};
}