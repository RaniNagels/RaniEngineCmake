#include "MoveCommand.h"

#include <Components/TransformComponent.h>

#include "../Components/GridComponent.h"
#include "../Ids.h"

Game::MoveCommand::MoveCommand(REC::GameObject* actor, glm::vec2 direction, float speed, GridComponent* playGroundGrid)
	: GameObjectInputCommand(actor)
	, m_Direction{direction}
	, m_Speed{speed}
	, m_pPlayGroundGrid{ playGroundGrid }
	, m_pMoveEvent{ std::make_unique<REC::Event>(Game::EventIds::MoveEvent, MoveEventArgs()) }
{
    m_Direction.x = std::clamp(m_Direction.x, -1.f, 1.f);
    m_Direction.y = std::clamp(m_Direction.y, -1.f, 1.f);

    auto* args = dynamic_cast<MoveEventArgs*>(m_pMoveEvent->GetArgs());
    if (args)
    {
        args->direction = m_Direction;
		args->speed = m_Speed;
		args->actor = GetGameObject();
    }
}

void Game::MoveCommand::Execute(float deltaTime)
{
    glm::vec3 movement{ m_Direction, 0.f };
    movement *= m_Speed * deltaTime * GetStrength();

    // TODO
	//GetGameObject()->Move(movement.x, movement.y);

    glm::vec3 currentPosition = GetGameObject()->GetTransform()->GetWorldPosition();
    auto currentCell = m_pPlayGroundGrid->GetCell(currentPosition);

    glm::vec3 newPos = currentPosition + movement;
    auto newCell = m_pPlayGroundGrid->GetCell(newPos);

    if (currentCell != newCell)
    {
        if (newCell.IsValid() && !newCell.isWall)
			GetGameObject()->Move(movement.x, movement.y);
    }
    else
		GetGameObject()->Move(movement.x, movement.y);

    m_pMoveEvent->Broadcast();
}
