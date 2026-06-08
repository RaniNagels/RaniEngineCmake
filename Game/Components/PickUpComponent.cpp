#include "PickUpComponent.h"


Game::PickUpComponent::PickUpComponent(REC::GameObject* owner, Game::PickUpComponent::PickUpType type)
	: Component(owner)
	, m_Type{ type }
{}

Game::PickUpComponent::~PickUpComponent()
{}

void Game::PickUpComponent::Update(float )
{}
