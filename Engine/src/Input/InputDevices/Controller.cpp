#include "Controller.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <Xinput.h>
#endif

#include <stdexcept>

#if defined(_WIN32)
class REC::Controller::Impl
{
public:
	Impl()
		: m_PreviousState{}
		, m_CurrentState{}
	{
	}

	void ResetState()
	{
		m_PreviousState = m_CurrentState;
		m_CurrentState = XINPUT_STATE();
	}

	void SetCurrentState(const XINPUT_STATE* state) { m_CurrentState = *state; }

	uint16_t GetButtonChanges() const { return m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons; }
	uint16_t GetCurrentButtons() const { return m_CurrentState.Gamepad.wButtons; }

	XINPUT_GAMEPAD GetCurrentGampad() { return m_CurrentState.Gamepad; }

private:
	XINPUT_STATE m_PreviousState;
	XINPUT_STATE m_CurrentState;
};
#else
class REC::Controller::Impl
{
public:
	Impl() {}
};
#endif

REC::Controller::Controller(uint8_t index)
	: m_Impl{std::make_unique<Impl>()}
	, m_ID{ index }
	, m_ButtonsPressedThisFrame{ 0 }
	, m_ButtonsReleasedThisFrame{ 0 }
{
}

REC::Controller::~Controller() = default;

void REC::Controller::ResetState()
{
	m_Impl->ResetState();
}

void REC::Controller::UpdateState(void* state)
{
#if defined(_WIN32)
	m_Impl->SetCurrentState(static_cast<XINPUT_STATE*>(state));
#endif

	int buttonChanges = m_Impl->GetButtonChanges();
	m_ButtonsPressedThisFrame = buttonChanges & m_Impl->GetCurrentButtons();
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_Impl->GetCurrentButtons());
}

bool REC::Controller::IsPressed(Input::Controller::Button button) const
{
	return  m_Impl->GetCurrentButtons() & button;
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
	auto gamepad = m_Impl->GetCurrentGampad();

	// TODO: return values of active range instead
	switch (range)
	{
	case Input::Controller::Range::Gamepad_LeftStick_X:
		if (gamepad.sThumbLX != 0)
			return true;
		break;
	case Input::Controller::Range::Gamepad_LeftStick_Y:
		if (gamepad.sThumbLY != 0)
			return true;
		break;
	case Input::Controller::Range::Gamepad_RightStick_X:
		if (gamepad.sThumbRX != 0)
			return true;
		break;
	case Input::Controller::Range::Gamepad_RightStick_Y:
		if (gamepad.sThumbRY != 0)
			return true;
		break;
	case Input::Controller::Range::Gamepad_LeftTrigger:
		if (gamepad.bLeftTrigger != 0)
			return true;
		break;
	case Input::Controller::Range::Gamepad_RightTrigger:
		if (gamepad.bRightTrigger != 0)
			return true;
		break;
	default:
		return false;
	}

	return false;
}

// will return a value between -1 and 1
float REC::Controller::GetRange(Input::Controller::Range range) const
{
	auto gamepad = m_Impl->GetCurrentGampad();

	constexpr float STICK_MAX{ MAXSHORT };
	constexpr float TRIGGER_MAX{ MAXBYTE };

	switch (range)
	{
	case Input::Controller::Range::Gamepad_LeftStick_X:
		return gamepad.sThumbLX / STICK_MAX;

	case Input::Controller::Range::Gamepad_LeftStick_Y:
		return gamepad.sThumbLY / STICK_MAX;

	case Input::Controller::Range::Gamepad_RightStick_X:
		return gamepad.sThumbRX / STICK_MAX;

	case Input::Controller::Range::Gamepad_RightStick_Y:
		return gamepad.sThumbRY / STICK_MAX;

	case Input::Controller::Range::Gamepad_LeftTrigger:
		return gamepad.bLeftTrigger / TRIGGER_MAX;

	case Input::Controller::Range::Gamepad_RightTrigger:
		return gamepad.bRightTrigger / TRIGGER_MAX;

	default:
		throw std::runtime_error("unknown range!");
	}
}
