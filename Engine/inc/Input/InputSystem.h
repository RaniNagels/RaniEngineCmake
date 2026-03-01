#pragma once
#include <vector>
#include <memory>

// TODO: pimple away
#include "../../src/Input/ControllerState.h" // including a private file!!
#include "../../src/Input/InputDevices/Keyboard.h"
#include "../../src/Input/InputDevices/Controller.h"

namespace REC
{
	class InputSystem final
	{
	public:
		explicit InputSystem();
		~InputSystem() = default;

		InputSystem(const InputSystem& other) = delete;
		InputSystem(InputSystem&& other) = delete;
		InputSystem& operator=(const InputSystem& other) = delete;
		InputSystem& operator=(InputSystem&& other) = delete;

		void ProcessInput();
		bool ShouldQuit() const { return m_ShouldQuit; }

	private:
		//class Impl;
		//std::unique_ptr<Impl> impl;

		std::vector<std::unique_ptr<Controller>> m_Controllers{};
		std::unique_ptr<Keyboard> m_Keyboard;
		bool m_ShouldQuit{ false };

		void ResetStates();
		void DetectInput();
		bool DetectControllerInput(size_t index, ControllerState& out);
		void ProcessKeyboardButtons();
		void ProcessRanges();
		void ProcessControllerButtons();

		static void ApplyThumbDeadZone(int16_t& x, int16_t& y, int deadzone)
		{
			float fx = static_cast<float>(x);
			float fy = static_cast<float>(y);

			float magnitude = float(sqrt(fx * fx + fy * fy));

			if (magnitude < deadzone)
			{
				x = 0;
				y = 0;
				return;
			}
		}
	};
}
