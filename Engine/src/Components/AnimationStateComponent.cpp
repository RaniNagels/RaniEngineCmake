#include <Components/AnimationStateComponent.h>

REC::AnimationStateComponent::AnimationStateComponent(GameObject* owner, std::unique_ptr<REC::IState>&& initialState)
	: Component(owner)
	, m_pCurrentState(std::move(initialState))
{
	m_pCurrentState->Enter();
}

void REC::AnimationStateComponent::Update(float deltaTime)
{
	if (auto result = m_pCurrentState->Update(deltaTime))
	{
		if (result.has_value())
			ChangeState(std::move(result.value()));
	}
}

void REC::AnimationStateComponent::ChangeState(std::unique_ptr<IState>&& newState)
{
	m_pCurrentState->Exit();
	m_pCurrentState = std::move(newState);
	m_pCurrentState->Enter();
}
