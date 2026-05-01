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
	Impl(uint8_t index)
		: m_PreviousState{}
		, m_CurrentState{}
		, m_ID{ index }
	{
	}

	void ResetState()
	{
		m_PreviousState = m_CurrentState;
		m_CurrentState = XINPUT_STATE();
	}

	uint8_t GetID() const { return m_ID; }

	void SetCurrentState(const XINPUT_STATE* state) { m_CurrentState = *state; }

	uint16_t GetButtonChanges() const { return m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons; }
	uint16_t GetCurrentButtons() const { return m_CurrentState.Gamepad.wButtons; }

	XINPUT_GAMEPAD GetCurrentGampad() { return m_CurrentState.Gamepad; }

	bool IsDownThisFrame(Input::Controller::Button button) const
	{
		return (GetButtonChanges() & button) && (GetCurrentButtons() & button);
	}

	bool IsUpThisFrame(Input::Controller::Button button) const
	{
		return (GetButtonChanges() & button) && !(GetCurrentButtons() & button);
	}

private:
	XINPUT_STATE m_PreviousState;
	XINPUT_STATE m_CurrentState;

	uint8_t m_ID;
};
#else
// TODO: SDL implementation
class REC::Controller::Impl
{
public:
	Impl(uint8_t index) : m_ID(index) {}
	uint8_t GetID() const { return m_ID; }
private:
	uint8_t m_ID;
};
#endif

REC::Controller::Controller(uint8_t index)
	: m_Impl{std::make_unique<Impl>(index)}
{
}

REC::Controller::~Controller() = default;

void REC::Controller::ResetState()
{
#if defined(_WIN32)
	m_Impl->ResetState();
#endif
}

void REC::Controller::UpdateState([[maybe_unused]] void* state)
{
#if defined(_WIN32)
	m_Impl->SetCurrentState(static_cast<XINPUT_STATE*>(state));
#endif
}

bool REC::Controller::IsPressed([[maybe_unused]] Input::Controller::Button button) const
{
#if defined(_WIN32)
	return  m_Impl->GetCurrentButtons() & button;
#else
	return false;
#endif
}

bool REC::Controller::IsDownThisFrame([[maybe_unused]] Input::Controller::Button button) const
{
#if defined(_WIN32)
	return m_Impl->IsDownThisFrame(button);
#else
	return false;
#endif
}

bool REC::Controller::IsUpThisFrame([[maybe_unused]] Input::Controller::Button button) const
{
#if defined(_WIN32)
	return m_Impl->IsUpThisFrame(button);
#else
return false;
#endif
}

bool REC::Controller::IsRangeActive([[maybe_unused]] Input::Controller::Range range) const
{
#if defined(_WIN32)
	auto gamepad = m_Impl->GetCurrentGampad();

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
#endif

	return false;
}

// will return a value between -1 and 1
float REC::Controller::GetRange([[maybe_unused]] Input::Controller::Range range) const
{
#if defined(_WIN32)
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
#else
	return 1.f;
#endif
}

uint8_t REC::Controller::GetID() const
{
	return m_Impl->GetID();
}
