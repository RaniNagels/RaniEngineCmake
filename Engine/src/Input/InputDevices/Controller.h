#pragma once
#include "../../../inc/Input/Input.h"
#include "../ControllerState.h"

namespace REC
{
	class Controller final
	{
	public:
		explicit Controller(uint8_t index);
		~Controller() = default;

		Controller(const Controller& other) = delete;
		Controller(Controller&& other) = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) = delete;

		void ResetState();
		void UpdateState(const ControllerState& state);

		bool IsPressed(Input::Controller::Button button) const;
		bool IsDownThisFrame(Input::Controller::Button button) const;
		bool IsUpThisFrame(Input::Controller::Button button) const;
		bool IsRangeActive(Input::Controller::Range range) const;

		uint8_t GetID() const { return m_ID; }

	private:
		uint8_t m_ID;

		ControllerState m_PreviousState;
		ControllerState m_CurrentState;

		// unlike keyboard this is a bitmask not an array
		uint16_t m_ButtonsPressedThisFrame;
		uint16_t m_ButtonsReleasedThisFrame;
	};
}