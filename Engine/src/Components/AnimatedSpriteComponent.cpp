#include <Components/AnimatedSpriteComponent.h>
#include "../ResourceManager.h"
#include "AnimationInfo.h"
#include "FrameInfo.h"
#include <Components/SpriteRenderComponent.h>
#include <GameObject.h>

REC::AnimatedSpriteComponent::AnimatedSpriteComponent(GameObject* owner, const AnimationDescriptor& descriptor)
	: Component(owner)
	, m_Descriptor{descriptor}
{
	m_AnimationInfo = ResourceManager::GetInstance().GetResource<AnimationInfo>( m_Descriptor.animationKey);
	
	if (!GetOwner()->HasComponent<SpriteRenderComponent>())
		throw std::runtime_error("animated sprite relies on spriteRenderComponent!");

	m_pSpriteRenderComponent = GetOwner()->GetComponent<SpriteRenderComponent>();

	for (auto frame : m_AnimationInfo->frameKeys)
		m_Frames.emplace_back(m_pSpriteRenderComponent->RequestFrameInfo(frame));

	if (m_Descriptor.startOnStartup)
		StartAnimation();

	m_FrameDuration = m_AnimationInfo->duration / m_Frames.size();
}

void REC::AnimatedSpriteComponent::Update(float deltaT)
{
	if (m_Animating)
	{
		m_Counter += deltaT;
		if (m_Counter > m_FrameDuration)
		{
			m_CurrentFrame++;
			if (m_CurrentFrame >= m_Frames.size())
			{
				m_CurrentFrame = 0;
				if (m_AnimationInfo->loop == false)
				{
					StopAnimation();
					return;
				}
			}
			m_pSpriteRenderComponent->SetFrame(m_Frames[m_CurrentFrame]);
			m_Counter -= m_FrameDuration;
		}
	}
}

void REC::AnimatedSpriteComponent::StartAnimation()
{
	m_Counter = 0;
	m_CurrentFrame = 0;
	m_pSpriteRenderComponent->SetFrame(m_Frames[m_CurrentFrame]);
	m_Animating = true;
}

void REC::AnimatedSpriteComponent::StopAnimation()
{
	m_Animating = false;
}

bool REC::AnimatedSpriteComponent::IsAnimating()
{
	return m_Animating;
}