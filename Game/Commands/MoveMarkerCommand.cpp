#include "MoveMarkerCommand.h"
#include <GameObject.h>
#include "../Components/MarkerComponent.h"

Game::MoveMarkerCommand::MoveMarkerCommand(REC::GameObject* actor, bool moveUp)
	: GameObjectInputCommand(actor)
	, m_MoveUp(moveUp)
{
	m_pMarkerComponent = GetGameObject()->GetComponent<MarkerComponent>();
	if (!m_pMarkerComponent)
		throw std::logic_error("MoveMarkerCommand requires a MarkerComponent on the GameObject!");
}

void Game::MoveMarkerCommand::Execute(float)
{
	m_pMarkerComponent->MoveToNextPosition(m_MoveUp);
}
