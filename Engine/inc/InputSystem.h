#pragma once

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
		bool m_ShouldQuit{ false };

		void DetectInput();
		void ProcessKeyboardButtons();
	};

}
