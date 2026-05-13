#include "BombComponent.h"
#include <Components/AnimatedSpriteComponent.h>
#include <GameObject.h>
#include <stdexcept>
#include <Components/SpriteRenderComponent.h>
#include <ServiceLocator.h>

Game::BombComponent::BombComponent(REC::GameObject* owner, const BombDescriptor& descriptor)
	: Component(owner)
	, m_Descriptor(descriptor)
{
	if (GetOwner()->HasComponent<REC::AnimatedSpriteComponent>())
		m_pAnimatedSpriteComponent = GetOwner()->GetComponent<REC::AnimatedSpriteComponent>();
	else
		throw std::runtime_error("BombComponent relies on AnimatedSpriteComponent!");

	if (GetOwner()->HasComponent<REC::SpriteRenderComponent>())
		m_pSpriteRenderComponent = GetOwner()->GetComponent<REC::SpriteRenderComponent>();
	else
		throw std::runtime_error("BombComponent relies on SpriteRenderComponent!");
}

void Game::BombComponent::Update(float deltaT)
{
	if (!m_Exploded)
	{
		m_Timer += deltaT;
		if (m_Timer >= m_LifeTime)
		{
			Detonate();
		}
	}
	else
	{
		if (m_pAnimatedSpriteComponent->IsAnimating() == false)
		{
			GetOwner()->Destroy();
		}
	}
}

void Game::BombComponent::Detonate()
{
	REC::AnimationDescriptor animation{};
	animation.animationDataFileKey = "characterData";
	animation.animationKey = "explosion";

	auto& bounds = GetOwner()->GetCollisionComponent()->GetModifiableBounds();
	bounds.push_back(REC::CollisionBound{ REC::Rect{ -125.f, -25.f, 250.f, 50.f }, true });
	bounds.push_back(REC::CollisionBound{ REC::Rect{ -25.f, -125.f, 50.f, 250.f }, true });

	m_pSpriteRenderComponent->ChangeHeight(250); // TODO: no magic numbers!
	m_pAnimatedSpriteComponent->ChangeAnimation(animation);
	m_pAnimatedSpriteComponent->StartAnimation();
	m_Exploded = true;

	REC::ServiceLocator::GetSoundSystem().Play("explosionSound", 1.f);
}

void Game::BombComponent::Reset()
{
	m_Timer = 0.f;
	m_Exploded = false;
}

