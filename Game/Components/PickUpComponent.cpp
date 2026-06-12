#include "PickUpComponent.h"


Game::PickUpComponent::PickUpComponent(REC::GameObject* owner, uint8_t type)
	: Component(owner)
{
	switch (type)
	{
	case 0:
		m_Type = PickUpType::ExtraBomb;
		break;
	case 1:
		m_Type = PickUpType::Detonator;
		break;
	case 2:
		m_Type = PickUpType::Flames;
		break;
	}
}

Game::PickUpComponent::~PickUpComponent()
{}

void Game::PickUpComponent::Update(float )
{}
