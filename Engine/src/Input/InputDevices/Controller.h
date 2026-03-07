#pragma once
#include <cstdint>
#include <Input/Input.h>
#include <memory>

namespace REC
{
	// TODO: create a static that keeps track of all the Id's that are still available and automatically asings an Id at construction
	class Controller final
	{
	public:
		explicit Controller(uint8_t index);
		~Controller();

		Controller(const Controller& other) = delete;
		Controller(Controller&& other) = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) = delete;

		void ResetState();
		void UpdateState(void* state);

		bool IsPressed(Input::Controller::Button button) const;
		bool IsDownThisFrame(Input::Controller::Button button) const;
		bool IsUpThisFrame(Input::Controller::Button button) const;
		bool IsRangeActive(Input::Controller::Range range) const;
		float GetRange(Input::Controller::Range range) const; // between -1 and 1

		uint8_t GetID() const { return m_ID; }

	private:
		class Impl;
		std::unique_ptr<Impl> m_Impl;

		uint8_t m_ID;

		// unlike keyboard this is a bitmask not an array
		uint16_t m_ButtonsPressedThisFrame;
		uint16_t m_ButtonsReleasedThisFrame;
	};
}