#include "EnemyMovementComponent.h"
#include <GameObject.h>
#include <Scene.h>
#include "GridComponent.h"
#include <Events/Event.h>
#include <Components/TransformComponent.h>

Game::EnemyMovementComponent::EnemyMovementComponent(REC::GameObject* owner, GridComponent* grid, REC::Scene* scene, float speed)
	: REC::Component(owner)
	, m_Speed(speed)
	, m_pGrid(grid)
	, m_pScene(scene)
	, m_TargetPosition(owner->GetTransform()->GetWorldPosition().x, owner->GetTransform()->GetWorldPosition().y) 
{
	SubscribeToEvent({ REC::EventIds::DeathEvent });
	FindNewPosition();
}

Game::EnemyMovementComponent::~EnemyMovementComponent()
{
	UnsubscribeFromEvent({ REC::EventIds::DeathEvent });
}

void Game::EnemyMovementComponent::Update(float deltaTime)
{
	if (glm::distance(GetOwner()->GetTransform()->GetWorldPosition(), glm::vec3{ m_TargetPosition.x, m_TargetPosition.y, 0.f }) < 1.f)
	{
		FindNewPosition();
	}
	else
	{
		auto direction = glm::normalize(glm::vec3{ m_TargetPosition, 0.f } - GetOwner()->GetTransform()->GetWorldPosition());
		GetOwner()->GetTransform()->SetLocalPosition(GetOwner()->GetTransform()->GetWorldPosition() + direction * m_Speed * deltaTime);
	}
}

void Game::EnemyMovementComponent::Notify(REC::Event * event)
{
	if (event->IsEvent(REC::EventIds::DeathEvent))
	{
		auto args = static_cast<REC::GameObjectEventArgs*>(event->GetArgs());
		if (args->sender == GetOwner())
		{
			GetOwner()->Destroy();
		}
	}
}

void Game::EnemyMovementComponent::FindNewPosition()
{
	auto myPosition = GetOwner()->GetTransform()->GetWorldPosition();
	glm::vec2 myPosition2D{ myPosition.x, myPosition.y };
	auto& currentCell = m_pGrid->GetCell(myPosition2D);

	// find neighboring cells that are not walls
	if (currentCell.row != uint8_t(-1) && currentCell.col != uint8_t(-1))
	{
		std::vector<GridComponent::Cell> neighbors{};
		for (int rowStep = -1; rowStep <= 1; ++rowStep)
		{
			for (int colStep = -1; colStep <= 1; ++colStep)
			{
				if (abs(rowStep) + abs(colStep) == 1) // only orthogonal neighbors
				{
					uint8_t neighborRow = currentCell.row + uint8_t(rowStep);
					uint8_t neighborCol = currentCell.col + uint8_t(colStep);
					if (m_pGrid->IsRowColValid(neighborRow, neighborCol))
					{
						auto& neighborCell = m_pGrid->GetCell(neighborRow, neighborCol);
						if (!neighborCell.isWall && !neighborCell.isDestructableWall)
						{
							neighbors.push_back(neighborCell);
						}
					}
				}
			}
		}
		if (!neighbors.empty())
		{
			int randomIndex = rand() % neighbors.size();
			m_TargetPosition = m_pGrid->GetAbsoluteCellPosition(neighbors[randomIndex]);
			m_TargetPosition += m_pGrid->GetCellSize() / 2.f; // move to center of cell
		}
	}
}
