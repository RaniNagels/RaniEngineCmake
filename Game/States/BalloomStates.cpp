#include "BalloomStates.h"
#include <GameObject.h>
#include <Components/AnimatedSpriteComponent.h>
#include <Components/LivesComponent.h>
#include <Events/Event.h>
#include "../Commands/MoveCommand.h"
#include "../Ids.h"

Game::BalloomFloatingState::BalloomFloatingState(REC::GameObject* owner)
	: REC::GameObjectState(owner)
{
	m_pAnimatedSpriteComponent = GetGameObject()->GetComponent<REC::AnimatedSpriteComponent>();
	m_pLivesComponent = GetGameObject()->GetComponent<REC::LivesComponent>();

	if (m_pAnimatedSpriteComponent == nullptr || m_pLivesComponent == nullptr)
		throw std::runtime_error("BalloomFloatingState relies on AnimatedSpriteComponent && LivesComponent!");
}

Game::BalloomFloatingState::~BalloomFloatingState()
{
	UnsubscribeFromEvent({ REC::make_sdbm_hash("MoveEvent"), REC::EventIds::LostLive });
}

void Game::BalloomFloatingState::Enter()
{
	REC::AnimationDescriptor animation{};
	animation.animationDataFileKey = "characterData";
	animation.animationKey = "balloom_look_left";
	animation.startOnStartup = true;

	m_pAnimatedSpriteComponent->ChangeAnimation(animation);

	SubscribeToEvent({ REC::make_sdbm_hash("MoveEvent"), REC::EventIds::LostLive });
}

std::optional<std::unique_ptr<REC::IState>> Game::BalloomFloatingState::Update(float)
{
	if (!m_pLivesComponent->HasLivesLeft() || m_DeathNotified)
		return std::make_unique<BalloomDeadState>(GetGameObject());

	if (m_ChangedDirection)
	{
		std::string animationKey{ "balloom_look_" };
		if (m_XDirection > 0)
			animationKey += "right";
		else
			animationKey += "left";

		REC::AnimationDescriptor animation{};
		animation.animationDataFileKey = "characterData";
		animation.animationKey = animationKey;
		animation.startOnStartup = true;

		m_pAnimatedSpriteComponent->ChangeAnimation(animation);
	}

	return {};
}

void Game::BalloomFloatingState::Exit()
{}

void Game::BalloomFloatingState::Notify(REC::Event* event)
{
	if (event->IsEvent(REC::EventIds::LostLive))
	{
		REC::GameObjectEventArgs* args = dynamic_cast<REC::GameObjectEventArgs*>(event->GetArgs());
		if (args == nullptr) return;
		if (args->sender != GetGameObject()) return;
		m_DeathNotified = true;
	}
	else if (event->IsEvent(REC::make_sdbm_hash("MoveEvent")))
	{
		MoveEventArgs* args = dynamic_cast<MoveEventArgs*>(event->GetArgs());
		if (args == nullptr) return;
		if (args->actor != GetGameObject()) return;

		m_HasBeenNotified = true;
		if (m_XDirection == args->direction.x) return;

		m_ChangedDirection = true;
		m_XDirection = args->direction.x;
	}
}

Game::BalloomDeadState::BalloomDeadState(REC::GameObject* owner)
	: REC::GameObjectState(owner)
{
	m_pAnimatedSpriteComponent = GetGameObject()->GetComponent<REC::AnimatedSpriteComponent>();
	if (!m_pAnimatedSpriteComponent)
		throw std::runtime_error("BalloomDeadState relies on AnimatedSpriteComponent!");

	REC::GameObjectEventArgs args{};
	args.sender = GetGameObject();
	m_pVeryDeathEvent = std::make_unique<REC::Event>(Game::EventIds::VeryDeathEvent, args);
}

void Game::BalloomDeadState::Enter()
{
	REC::AnimationDescriptor animation{};
	animation.animationDataFileKey = "characterData";
	animation.animationKey = "balloom_death";
	animation.startOnStartup = true;

	m_pAnimatedSpriteComponent->ChangeAnimation(animation);
}

std::optional<std::unique_ptr<REC::IState>> Game::BalloomDeadState::Update(float)
{
	if (!m_pAnimatedSpriteComponent->IsAnimating())
	{
		m_pVeryDeathEvent->Broadcast();
	}
	return {};
}

void Game::BalloomDeadState::Exit()
{
}
