#include "Controller.h"

REC::Controller::Controller(uint8_t index)
	: m_ID{ index }
	, m_ButtonsPressedThisFrame{ 0 }
	, m_ButtonsReleasedThisFrame{ 0 }
	, m_CurrentState{}
	, m_PreviousState{}
{
}

void REC::Controller::ResetState()
{
	m_PreviousState = m_CurrentState;
	m_CurrentState = ControllerState();
}

void REC::Controller::UpdateState(const ControllerState& state)
{
	m_CurrentState = state;

	auto buttonChanges = m_CurrentState.buttons ^ m_PreviousState.buttons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.buttons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.buttons);
}

bool REC::Controller::IsPressed(Input::Controller::Button button) const
{
	return m_CurrentState.buttons & button;
}

bool REC::Controller::IsDownThisFrame(Input::Controller::Button button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool REC::Controller::IsUpThisFrame(Input::Controller::Button button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool REC::Controller::IsRangeActive(Input::Controller::Range range) const
{
	// TODO: return values of active range instead
	switch (range)
	{
	case Input::Controller::Range::Gamepad_LeftStick:
		if (m_CurrentState.thumbLX != 0 || m_CurrentState.thumbLY != 0)
			return true;
		break;
	case Input::Controller::Range::Gamepad_RightStick:
		if (m_CurrentState.thumbRX != 0 || m_CurrentState.thumbRY != 0)
			return true;
		break;
	case Input::Controller::Range::Gamepad_LeftTrigger:
		if (m_CurrentState.leftTrigger != 0)
			return true;
		break;
	case Input::Controller::Range::Gamepad_RightTrigger:
		if (m_CurrentState.rightTrigger != 0)
			return true;
		break;
	default:
		return false;
	}

	return false;
}
