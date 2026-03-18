#include "ChangeHealthCommand.h"
#include <Components/HealthComponent.h>

Game::ChangeHealthCommand::ChangeHealthCommand(REC::GameObject* actor, float changeDelta)
	: GameObjectInputCommand(actor)
	, m_HealthDelta{changeDelta}
	, m_pHealthComponent{actor->GetComponent<REC::HealthComponent>()}
{
}

void Game::ChangeHealthCommand::Execute(float)
{
	m_pHealthComponent->ChangeHealth(-m_HealthDelta);
}
