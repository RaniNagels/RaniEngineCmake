#include "SoftBlockComponent.h"
#include "../Ids.h"
#include <GameObject.h>
#include "GridComponent.h"
#include <Scene.h>
#include <Events/Event.h>
#include "../RenderLayers.h"
#include <Util.h>
#include <Components/SpriteRenderComponent.h>
#include "DoorCollisionComponent.h"
#include "PickUpComponent.h"

Game::SoftBlockComponent::SoftBlockComponent(REC::GameObject* owner,REC::Scene* scene, uint8_t row, uint8_t col)
	: Component(owner)
	, m_pScene{ scene }
	, m_row{ row }
	, m_col{ col }
{
	SubscribeToEvent({ EventIds::DestroySoftBlockEvent });
}

Game::SoftBlockComponent::~SoftBlockComponent()
{
	UnsubscribeFromEvent({ EventIds::DestroySoftBlockEvent });
}

void Game::SoftBlockComponent::Notify(REC::Event* event)
{
	if (event->IsEvent(EventIds::DestroySoftBlockEvent))
	{
		auto* eventArgs = dynamic_cast<GridEventArgs*>(event->GetArgs());
		if (eventArgs != nullptr)
		{
			auto* gridComp = eventArgs->grid->GetComponent<GridComponent>();
			if (gridComp != nullptr)
			{
				if (eventArgs->row == m_row && eventArgs->col == m_col)
				{
					auto cell = gridComp->GetCell(eventArgs->row, eventArgs->col);
					if (cell.isDestructableWall)
					{
						if (cell.hasPowerUp)
							CreatePowerUp(gridComp, eventArgs->row, eventArgs->col);
						else if (cell.hasExit)
							CreateDoor(gridComp, eventArgs->row, eventArgs->col);
					}

					gridComp->ModifyCell(eventArgs->row, eventArgs->col, false); // remove destructable wall
					GetOwner()->Destroy();
				}
			}
		}
	}
}

void Game::SoftBlockComponent::CreateDoor(GridComponent* grid, uint8_t row, uint8_t col)
{
	auto position = grid->GetAbsoluteCellPosition(row, col);

	REC::GameObjectDescriptor doorBlockDesc{};
	doorBlockDesc.id = Game::ObjectIds::Door;
	doorBlockDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Placables);
	doorBlockDesc.startPosX = position.x;
	doorBlockDesc.startPosY = position.y;

	REC::SpriteDescriptor doorSpriteDesc{};
	doorSpriteDesc.drawHeight = uint16_t(grid->GetCellSize().y);
	doorSpriteDesc.drawWidth = uint16_t(grid->GetCellSize().x);
	doorSpriteDesc.textureKey = "generalSprites";
	doorSpriteDesc.frameDataFileKey = "characterData";
	doorSpriteDesc.frameKey = "door";

	auto* door = m_pScene->CreateGameObject(doorBlockDesc);
	door->AddComponent<REC::SpriteRenderComponent>(doorSpriteDesc);

	REC::CollisionDescriptor doorCollisionDesc{};
	doorCollisionDesc.collisionType = REC::CollisionType::Static;
	doorCollisionDesc.bounds.emplace_back(REC::CollisionBound{ REC::Rect{ 0.f, 0.f, float(grid->GetCellSize().x), float(grid->GetCellSize().y) }, false });
	door->AddCollisionComponent<DoorCollisionComponent>(doorCollisionDesc);
}

void Game::SoftBlockComponent::CreatePowerUp(GridComponent* grid, uint8_t row, uint8_t col)
{
	auto position = grid->GetAbsoluteCellPosition(row, col);
	// get cell
	auto& cell = grid->GetCell(row, col);
	std::string frameKey{};
	switch (cell.powerUpType)
	{
	case 0:
		frameKey = "ExtraBomb";
		break;
	case 1:
		frameKey = "Detonator";
		break;
	case 2:
		frameKey = "Flames";
		break;
	}

	REC::GameObjectDescriptor powerUpDesc{};
	powerUpDesc.id = Game::ObjectIds::PickUp;
	powerUpDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Placables);
	powerUpDesc.startPosX = position.x;
	powerUpDesc.startPosY = position.y;
	
	REC::SpriteDescriptor powerUpSpriteDesc{};
	powerUpSpriteDesc.drawHeight = uint16_t(grid->GetCellSize().y);
	powerUpSpriteDesc.drawWidth = uint16_t(grid->GetCellSize().x);
	powerUpSpriteDesc.textureKey = "generalSprites";
	powerUpSpriteDesc.frameDataFileKey = "characterData";
	powerUpSpriteDesc.frameKey = frameKey;

	auto* powerUp = m_pScene->CreateGameObject(powerUpDesc);
	powerUp->AddComponent<REC::SpriteRenderComponent>(powerUpSpriteDesc);
	powerUp->AddComponent<PickUpComponent>(cell.powerUpType);
	
	REC::CollisionDescriptor powerUpCollisionDesc{};
	powerUpCollisionDesc.collisionType = REC::CollisionType::Static;
	powerUpCollisionDesc.bounds.emplace_back(REC::CollisionBound{ REC::Rect{ 0.f, 0.f, float(grid->GetCellSize().x), float(grid->GetCellSize().y) }, false });
	powerUp->AddCollisionComponent<REC::CollisionComponent>(powerUpCollisionDesc);
}
