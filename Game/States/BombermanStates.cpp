#include "BombermanStates.h"

#include <GameObject.h>
#include <Components/AnimatedSpriteComponent.h>
#include <Components/TransformComponent.h>
#include <Components/LivesComponent.h>

#include "../Commands/MoveCommand.h"
#include <ServiceLocator.h>

// ----- DEAD -------------------------------------------------------------------------
Game::BombermanDeadState::BombermanDeadState(REC::GameObject* owner)
	: REC::GameObjectState(owner)
{
	m_pAnimatedSpriteComponent = GetGameObject()->GetComponent<REC::AnimatedSpriteComponent>();
	if (m_pAnimatedSpriteComponent == nullptr)
		throw std::runtime_error("BombermanDeadState relies on AnimatedSpriteComponent!");
}

void Game::BombermanDeadState::Enter()
{
	REC::AnimationDescriptor animation{};
	animation.animationDataFileKey = "characterData";
	animation.animationKey = "bomberman_death";
	animation.startOnStartup = true;

	m_pAnimatedSpriteComponent->ChangeAnimation(animation);
}

std::optional<std::unique_ptr<REC::IState>> Game::BombermanDeadState::Update(float)
{
	return {};
}

// ----- IDLE -------------------------------------------------------------------------
Game::BombermanIdleState::BombermanIdleState(REC::GameObject* owner)
	: REC::GameObjectState(owner)
{
	m_pAnimatedSpriteComponent = GetGameObject()->GetComponent<REC::AnimatedSpriteComponent>();
	m_pLivesComponent = GetGameObject()->GetComponent<REC::LivesComponent>();

	if (m_pAnimatedSpriteComponent == nullptr || m_pLivesComponent == nullptr)
		throw std::runtime_error("BombermanLeftState relies on AnimatedSpriteComponent && LivesComponent!");
}

void Game::BombermanIdleState::Notify(REC::Event* event)
{
	MoveEventArgs* args = dynamic_cast<MoveEventArgs*>(event->GetArgs());
	if (args == nullptr) return;
	if (args->actor != GetGameObject()) return;
	m_HasBeenNotified = true;
	m_Direction = args->direction;
}

void Game::BombermanIdleState::Enter()
{
	m_pAnimatedSpriteComponent->StopAnimation();
	m_pAnimatedSpriteComponent->ResetAnimation();
	SubscribeToEvent({ REC::make_sdbm_hash("MoveEvent") });
}

std::optional<std::unique_ptr<REC::IState>> Game::BombermanIdleState::Update(float)
{
	if (!m_pLivesComponent->HasLivesLeft())
		return std::make_unique<BombermanDeadState>(GetGameObject());

	if (m_HasBeenNotified)
		return std::make_unique<BombermanWalkingState>(GetGameObject(), m_Direction);

	return {};
}

void Game::BombermanIdleState::Exit()
{
	m_pAnimatedSpriteComponent->StartAnimation();
	UnsubscribeFromEvent({ REC::make_sdbm_hash("MoveEvent") });
}

// ----- WALKING -------------------------------------------------------------------------
Game::BombermanWalkingState::BombermanWalkingState(REC::GameObject* owner, glm::vec2 dir)
	: REC::GameObjectState(owner)
	, m_Direction(dir)
{
	m_AnimationKey = GetAnimationKey(m_Direction);

	m_pAnimatedSpriteComponent = GetGameObject()->GetComponent<REC::AnimatedSpriteComponent>();
	m_pLivesComponent = GetGameObject()->GetComponent<REC::LivesComponent>();

	if (m_pAnimatedSpriteComponent == nullptr || m_pLivesComponent == nullptr)
		throw std::runtime_error("BombermanLeftState relies on AnimatedSpriteComponent && LivesComponent!");
}

void Game::BombermanWalkingState::Notify(REC::Event* event)
{
	MoveEventArgs* args = dynamic_cast<MoveEventArgs*>(event->GetArgs());
	if (args == nullptr) return;
	if (args->actor != GetGameObject()) return;

	m_HasBeenNotified = true;
	if (m_Direction == args->direction) return; 

	m_ChangedDirection = true;
	m_Direction = args->direction;
}

void Game::BombermanWalkingState::Enter()
{
	ChangeAnimation(m_AnimationKey);
	SubscribeToEvent({ REC::make_sdbm_hash("MoveEvent") });
}

std::optional<std::unique_ptr<REC::IState>> Game::BombermanWalkingState::Update(float )
{
	if (!m_pLivesComponent->HasLivesLeft())
		return std::make_unique<BombermanDeadState>(GetGameObject());

	if (!m_HasBeenNotified)
		return std::make_unique<BombermanIdleState>(GetGameObject());

	if (m_ChangedDirection)
	{
		m_ChangedDirection = false;
		ChangeAnimation(GetAnimationKey(m_Direction));
	}

	auto& SS = REC::ServiceLocator::GetSoundSystem();

	if (m_Direction.x != 0 && !SS.IsPlaying("stepHorizontalSound"))
		SS.Play("stepHorizontalSound", 0.5f);
	else if (m_Direction.y != 0 && !SS.IsPlaying("stepVerticalSound"))
		SS.Play("stepVerticalSound", 0.5f);

	m_HasBeenNotified = false;
	return {};
}

void Game::BombermanWalkingState::Exit()
{
	UnsubscribeFromEvent({ REC::make_sdbm_hash("MoveEvent") });
}

std::string Game::BombermanWalkingState::GetAnimationKey(glm::vec2 dir) const
{
	constexpr float epsilon = 0.001f;

	if (dir.x > epsilon)
		return "bomberman_walk_right";
	else if (dir.x < -epsilon)
		return "bomberman_walk_left";
	else if (dir.y > epsilon)
		return "bomberman_walk_front";
	else if (dir.y < -epsilon)
		return "bomberman_walk_back";
	else 
		throw std::runtime_error("Direction cannot be (0,0) for BombermanWalkingState!");
}

void Game::BombermanWalkingState::ChangeAnimation(const std::string& key)
{
	m_AnimationKey = key;
	REC::AnimationDescriptor animation{};
	animation.animationDataFileKey = "characterData";
	animation.animationKey = key;
	animation.startOnStartup = true;

	m_pAnimatedSpriteComponent->ChangeAnimation(animation);
}
