#pragma once
#include <vector>
#include <memory>
#include <concepts>

#include <Commands/ICommand.h>
#include <Input/InputAction.h>
#include <Events/Event.h>

namespace REC
{
	template <typename C>
	concept Command = std::derived_from<C, ICommand>;

	template <typename A>
	concept Action = std::derived_from<A, IInputAction>;

	template <typename E>
	concept EventCommand = std::derived_from<E, Event>;

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
		C* AddCommand(Args&&... args)
		{
			m_Commands.emplace_back(std::make_unique<C>(std::forward<Args>(args)...));
			return static_cast<C*>(m_Commands.back().get());
		}

		template <EventCommand E>
		E* AddEvent(EventId id)
		{
			m_EventCommands.emplace_back(std::make_unique<E>(id, EventArgs()));
			return static_cast<E*>(m_EventCommands.back().get());
		}

		template <Action A, typename... Args>
		void AddInputAction(Args&&... args)
		{
			m_Actions.emplace_back(std::make_unique<A>(std::forward<Args>(args)...));
		}

		template <Action A>
		void AddInputAction(std::unique_ptr<A> action)
		{
			m_Actions.emplace_back(std::move(action));
		}

		std::vector<IInputAction*> GetInputActions(InputActionType type) const;

		// will execute all commands and trigger all events
		void Execute(float deltaTime, float inputStrength = 1.f) const;
		void ToggleActive() { m_IsActive = !m_IsActive; }
		bool IsActive() const { return m_IsActive; }

	private:
		std::vector<std::unique_ptr<IInputAction>> m_Actions{};
		std::vector<std::unique_ptr<ICommand>> m_Commands{};
		std::vector<std::unique_ptr<Event>> m_EventCommands{};
		bool m_IsActive{ true };
	};
}