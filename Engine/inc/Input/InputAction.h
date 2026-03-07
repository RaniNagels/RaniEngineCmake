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

	// TODO: remove or start using PlayerIndex
	// Currently -> ControllerComponent.id, command.execute gets passed an ID of triggered controller, execute checks id with gameObject.controllerComponent.id
	struct IInputAction
	{
		virtual ~IInputAction() = default;
		virtual InputActionType GetType() const = 0;
	};

	struct ControllerButtonAction final : public IInputAction
	{
		ControllerButtonAction(Input::Controller::Button btn, ButtonState st = ButtonState::Pressed, uint8_t plIdx = -1)
			: button{ btn }, state{ st }, playerIndex{ plIdx }
		{
		}

		virtual InputActionType GetType() const override { return InputActionType::ControllerButton; }

		Input::Controller::Button button;
		ButtonState state;
		uint8_t playerIndex; // -1 is all players
	};

	struct KeyboardButtonAction final : public IInputAction
	{
		KeyboardButtonAction(Input::Keyboard::Button btn, ButtonState st = ButtonState::Pressed, uint8_t plIdx = -1)
			: button{ btn }, state{ st }, playerIndex{ plIdx }
		{
		}

		virtual InputActionType GetType() const override { return InputActionType::KeyboardButton; }

		Input::Keyboard::Button button;
		ButtonState state;
		uint8_t playerIndex; // -1 is all players
	};

	struct ControllerRangeAction final : public IInputAction
	{
		ControllerRangeAction(Input::Controller::Range rg, uint8_t plIdx = -1)
			: range{ rg }, playerIndex{ plIdx }
		{
		}

		virtual InputActionType GetType() const override { return InputActionType::ControllerRange; }

		Input::Controller::Range range;
		uint8_t playerIndex; // -1 is all players
	};
}