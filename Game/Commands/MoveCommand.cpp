#include "MoveCommand.h"
#include <Components/TransformComponent.h>
#include "../Components/GridComponent.h"

Game::MoveCommand::MoveCommand(REC::GameObject* actor, glm::vec2 direction, float speed, GridComponent* playGroundGrid)
	: GameObjectInputCommand(actor)
	, m_Direction{direction}
	, m_Speed{speed}
	, m_pPlayGroundGrid{ playGroundGrid }
{
    m_Direction.x = std::clamp(m_Direction.x, -1.f, 1.f);
    m_Direction.y = std::clamp(m_Direction.y, -1.f, 1.f);
}

void Game::MoveCommand::Execute(float deltaTime)
{
    glm::vec3 movement{ m_Direction, 0.f };
    movement *= m_Speed * deltaTime * GetStrength();

    glm::vec3 currentPosition = GetGameObject()->GetTransform()->GetWorldPosition();
    auto currentCell = m_pPlayGroundGrid->GetCell(currentPosition);

    glm::vec3 newPos = currentPosition + movement;
    auto newCell = m_pPlayGroundGrid->GetCell(newPos);

    if (currentCell != newCell)
    {
        if (newCell.IsValid() && !newCell.isWall)
            GetGameObject()->GetTransform()->AddToLocalPosition(movement);
    }
    else
    {
        GetGameObject()->GetTransform()->AddToLocalPosition(movement);
    }

}
