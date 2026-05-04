#pragma once
#include <memory>

#include <State/IState.h>
#include <Components/Component.h>

namespace REC
{
	class StateComponent : public Component
	{
	public:
		explicit StateComponent(GameObject* owner, std::unique_ptr<IState>&& initialState);
		virtual ~StateComponent() = default;

		StateComponent(const StateComponent& other) = delete;
		StateComponent(StateComponent&& other) noexcept = delete;
		StateComponent& operator=(const StateComponent& other) = delete;
		StateComponent& operator=(StateComponent&& other) noexcept = delete;

		void Update(float deltaTime) override;
		void ChangeState(std::unique_ptr<IState>&& newState);

	private:
		std::unique_ptr<IState> m_pCurrentState;
	};
}