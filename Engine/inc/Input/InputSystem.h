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

		void ProcessInput(float deltaTime);
		bool ShouldQuit() const { return m_ShouldQuit; }

		InputBinding* CreateInputBinding();
		void RemoveInputBinding(InputBinding* inputBinding);
		void SetNumberOfActiveControllers(uint8_t num);

	private:
		// TODO: everything that is private should go into the Pimpl, splitting data like this is overcomplicating it
		void HandleInput(float deltaTime);
		void HandleKeyboard(float deltaTime, InputBinding* binding);
		void HandleControllerButtons(float deltaTime, InputBinding* binding, Controller* controller);
		void HandleControllerRanges(float deltaTime, InputBinding* binding, Controller* controller);

		class Impl;
		std::unique_ptr<Impl> m_Impl;

		std::vector<std::unique_ptr<InputBinding>> m_Bindings{};
		bool m_ShouldQuit{ false };
	};
}
