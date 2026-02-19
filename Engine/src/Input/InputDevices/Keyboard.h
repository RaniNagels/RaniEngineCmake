#pragma once
#include "../../../inc/Input/Input.h"
#include <array>

namespace REC
{
	class Keyboard final
	{
	public:
		explicit Keyboard() = default;
		~Keyboard() = default;

		Keyboard(const Keyboard& other) = delete;
		Keyboard(Keyboard&& other) = delete;
		Keyboard& operator=(const Keyboard& other) = delete;
		Keyboard& operator=(Keyboard&& other) = delete;

		void ResetState();
		void UpdateState(uint8_t key, bool isDown);

		// Continously pressed
		bool IsPressed(Input::Keyboard::Button key) const;

		// Is it down when in the previous frame it was up?
		bool IsDownThisFrame(Input::Keyboard::Button key) const;

		// Is it up when in the previous frame it was down?
		bool IsUpThisFrame(Input::Keyboard::Button key) const;

	private:
		std::array<bool, size_t(256U)> m_Current{};
		std::array<bool, size_t(256U)> m_Previous{};
	};
}