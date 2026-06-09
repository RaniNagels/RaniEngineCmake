#pragma once
#include <memory>

#include <State/IState.h>
#include <Components/Component.h>

namespace REC
{
	class AnimationStateComponent : public Component
	{
	public:
		explicit AnimationStateComponent(GameObject* owner, std::unique_ptr<IState>&& initialState);
		virtual ~AnimationStateComponent();

		AnimationStateComponent(const AnimationStateComponent& other) = delete;
		AnimationStateComponent(AnimationStateComponent&& other) noexcept = delete;
		AnimationStateComponent& operator=(const AnimationStateComponent& other) = delete;
		AnimationStateComponent& operator=(AnimationStateComponent&& other) noexcept = delete;

		void Update(float deltaTime) override;
		void ChangeState(std::unique_ptr<IState>&& newState);

	private:
		std::unique_ptr<IState> m_pCurrentState;
	};
}