#pragma once
#include <vector>
#include <memory>

#include "InputBinding.h"

namespace REC
{
	class Controller;

	class InputSystem final
	{
	public:
		explicit InputSystem();
		~InputSystem();

		InputSystem(const InputSystem& other) = delete;
		InputSystem(InputSystem&& other) = delete;
		InputSystem& operator=(const InputSystem& other) = delete;
		InputSystem& operator=(InputSystem&& other) = delete;

		void ProcessInput();
		bool ShouldQuit() const { return m_ShouldQuit; }

		InputBinding* CreateInputBinding();
		void RemoveInputBinding(InputBinding* inputBinding);
		void SetNumberOfActiveControllers(uint8_t num);

	private:
		void HandleInput();
		void HandleKeyboard(InputBinding* binding);
		void HandleControllerButtons(InputBinding* binding, Controller* controller);
		void HandleControllerRanges(InputBinding* binding, Controller* controller);

		class Impl;
		std::unique_ptr<Impl> m_Impl;

		std::vector<std::unique_ptr<InputBinding>> m_Bindings{};
		bool m_ShouldQuit{ false };
	};
}
