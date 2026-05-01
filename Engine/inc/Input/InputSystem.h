#pragma once
#include <vector>
#include <memory>

#include <Input/InputBinding.h>

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
		bool ShouldQuit() const;

		InputBinding* CreateInputBinding();
		void RemoveInputBinding(InputBinding* inputBinding);
		void SetNumberOfActiveControllers(uint8_t num);

	private:
		class Impl;
		std::unique_ptr<Impl> m_Impl;
	};
}
