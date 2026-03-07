#pragma once
#include <ICommand.h>
#include <Input/InputAction.h>
#include <vector>
#include <memory>
#include <concepts>

namespace REC
{
	template <typename C>
	concept Command = std::derived_from<C, ICommand>;

	template <typename A>
	concept Action = std::derived_from<A, IInputAction>;

	class InputBinding final
	{
	public:
		explicit InputBinding() = default;
		~InputBinding() = default;

		InputBinding(const InputBinding& other) = delete;
		InputBinding(InputBinding&& other) = delete;
		InputBinding& operator=(const InputBinding& other) = delete;
		InputBinding& operator=(InputBinding&& other) = delete;

		template <Command C, typename... Args>
		void AddCommand(Args&&... args)
		{
			m_Commands.emplace_back(std::make_unique<C>(std::forward<Args>(args)...));
		}

		template <Action A, typename... Args>
		void AddInputAction(Args&&... args)
		{
			m_Actions.emplace_back(std::make_unique<A>(std::forward<Args>(args)...));
		}

		std::vector<IInputAction*> GetInputActions(InputActionType type) const;

		// will execute all commands
		void Execute(uint8_t controllerIndex = -1, float inputStrength = 1.f) const;

	private:
		std::vector<std::unique_ptr<IInputAction>> m_Actions{};
		std::vector<std::unique_ptr<ICommand>> m_Commands{};
	};
}