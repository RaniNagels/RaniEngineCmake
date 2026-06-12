#include "BombComponent.h"
#include <Components/AnimatedSpriteComponent.h>
#include <GameObject.h>
#include <stdexcept>
#include <Components/SpriteRenderComponent.h>
#include <ServiceLocator.h>
#include "GridComponent.h"
#include <Components/TransformComponent.h>
#include "../RenderLayers.h"
#include "DebugBoundsRenderComponent.h"
#include "../Ids.h"

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

	REC::GameObjectEventArgs args{};
	args.sender = GetOwner();
	m_pDetonateEvent = std::make_unique<REC::Event>(EventIds::BombDetonationEvent, args);

	Game::GridEventArgs gridEventArgs{};
	gridEventArgs.grid = descriptor.grid->GetOwner();
	m_DestructionEvents.reserve(4);

	for (uint8_t i{}; i < 4; ++i)
	{
		gridEventArgs.row = uint8_t(-1);
		gridEventArgs.col = uint8_t(-1);
		m_DestructionEvents.push_back(std::make_unique<REC::Event>(EventIds::DestroySoftBlockEvent, gridEventArgs));
	}

	SubscribeToEvent({ EventIds::BombDetonationEvent });
}

Game::BombComponent::~BombComponent()
{
	UnsubscribeFromEvent({ EventIds::BombDetonationEvent });
}

void Game::BombComponent::Update(float deltaT)
{
	if (!m_Exploded)
	{
		m_Timer += deltaT;
		if (m_Timer >= m_LifeTime)
		{
			m_pDetonateEvent->Broadcast(); // do not change the object vector during update!!
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
	glm::uvec4 explosionRange{GetExplosionRange()}; // right (x), down (y), left (-x), up (-y)
	auto size = m_Descriptor.grid->GetCellSize();

	auto scene = m_Descriptor.scene;
	for (uint8_t i = 1; i <= explosionRange.x; ++i)
	{
		CreateExplosionInCell(scene, GetOwner(), { size.x * i, 0 }, i == explosionRange.x);
	}
	for (uint8_t i = 1; i <= explosionRange.y; ++i)
	{
		CreateExplosionInCell(scene, GetOwner(), { 0, size.y * i }, i == explosionRange.y);
	}
	for (uint8_t i = 1; i <= explosionRange.z; ++i)
	{
		CreateExplosionInCell(scene, GetOwner(), { -size.x * i, 0 }, i == explosionRange.z);
	}
	for (uint8_t i = 1; i <= explosionRange.w; ++i)
	{
		CreateExplosionInCell(scene, GetOwner(), { 0, -size.y * i }, i == explosionRange.w);
	}

	REC::AnimationDescriptor animation{};
	animation.animationDataFileKey = "characterData";
	animation.animationKey = "explosion_center";

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

void Game::BombComponent::Notify(REC::Event* event)
{
	if (event->IsEvent(EventIds::BombDetonationEvent))
	{
		Detonate();
	}
}

glm::uvec4 Game::BombComponent::GetExplosionRange() const
{
	//auto& bombcell = m_Descriptor.grid->GetCell(GetOwner()->GetTransform()->GetWorldPosition());
	//
	//glm::uvec4 explosionRange{}; // right (x), down (y), left (-x), up (-y)
	//for (uint8_t range{ 1 }; range <= m_Descriptor.explosionRange; ++range)
	//{
	//	// check right
	//	uint8_t cellCol = bombcell.col + range;
	//	uint8_t cellRow = bombcell.row;
	//
	//	if (m_Descriptor.grid->IsRowColValid(cellRow, cellCol))
	//	{
	//		if (m_Descriptor.grid->GetCell(cellRow, cellCol).isDestructableWall
	//			&& uint8_t(explosionRange.x) == range - uint8_t(1))
	//		{
	//			auto* args = m_DestructionEvents[0]->GetArgs();
	//			auto* gridEventArgs = dynamic_cast<GridEventArgs*>(args);
	//			gridEventArgs->row = cellRow;
	//			gridEventArgs->col = cellCol;
	//			m_DestructionEvents[0]->Broadcast();
	//
	//			explosionRange.x = range;
	//		}
	//		if (!m_Descriptor.grid->GetCell(cellRow, cellCol).isWall
	//			&& uint8_t(explosionRange.x) == range - uint8_t(1))
	//		{
	//			explosionRange.x = range;
	//		}
	//	}
	//
	//	// check down
	//	cellCol = bombcell.col;
	//	cellRow = bombcell.row + range;
	//
	//	if (m_Descriptor.grid->IsRowColValid(cellRow, cellCol))
	//	{
	//		if (m_Descriptor.grid->GetCell(cellRow, cellCol).isDestructableWall
	//			&& uint8_t(explosionRange.y) == range - uint8_t(1))
	//		{
	//			auto* args = m_DestructionEvents[1]->GetArgs();
	//			auto* gridEventArgs = dynamic_cast<Game::GridEventArgs*>(args);
	//			gridEventArgs->row = cellRow;
	//			gridEventArgs->col = cellCol;
	//			m_DestructionEvents[1]->Broadcast();
	//
	//			explosionRange.y = range;
	//		}
	//		if (!m_Descriptor.grid->GetCell(cellRow, cellCol).isWall
	//			&& uint8_t(explosionRange.y) == range - uint8_t(1))
	//		{
	//			explosionRange.y = range;
	//		}
	//	}
	//
	//	// check left
	//	cellCol = bombcell.col - range;
	//	cellRow = bombcell.row;
	//
	//	if (m_Descriptor.grid->IsRowColValid(cellRow, cellCol))
	//	{
	//		if (m_Descriptor.grid->GetCell(cellRow, cellCol).isDestructableWall
	//			&& uint8_t(explosionRange.z) == range - uint8_t(1))
	//		{
	//			auto* args = m_DestructionEvents[2]->GetArgs();
	//			auto* gridEventArgs = dynamic_cast<Game::GridEventArgs*>(args);
	//			gridEventArgs->row = cellRow;
	//			gridEventArgs->col = cellCol;
	//			m_DestructionEvents[2]->Broadcast();
	//
	//			explosionRange.z = range;
	//		}
	//		if (!m_Descriptor.grid->GetCell(cellRow, cellCol).isWall
	//			&& uint8_t(explosionRange.z) == range - uint8_t(1))
	//		{
	//			explosionRange.z = range;
	//		}
	//	}
	//
	//	// check up
	//	cellCol = bombcell.col;
	//	cellRow = bombcell.row - range;
	//
	//	if (m_Descriptor.grid->IsRowColValid(cellRow, cellCol))
	//	{
	//		if (m_Descriptor.grid->GetCell(cellRow, cellCol).isDestructableWall
	//			&& uint8_t(explosionRange.w) == range - uint8_t(1))
	//		{
	//			auto* args = m_DestructionEvents[3]->GetArgs();
	//			auto* gridEventArgs = dynamic_cast<Game::GridEventArgs*>(args);
	//			gridEventArgs->row = cellRow;
	//			gridEventArgs->col = cellCol;
	//			m_DestructionEvents[3]->Broadcast();
	//
	//			explosionRange.w = range;
	//		}
	//		if (!m_Descriptor.grid->GetCell(cellRow, cellCol).isWall
	//			&& uint8_t(explosionRange.w) == range - uint8_t(1))
	//		{
	//			explosionRange.w = range;
	//		}
	//	}
	//}
	//return explosionRange;

	return {
		ComputeDirection(0,  1, m_DestructionEvents[0].get()), // right
		ComputeDirection(1,  0, m_DestructionEvents[1].get()), // down
		ComputeDirection(0, -1, m_DestructionEvents[2].get()), // left
		ComputeDirection(-1, 0, m_DestructionEvents[3].get())  // up
	
	};
}

void Game::BombComponent::CreateExplosionInCell(REC::Scene* scene, REC::GameObject* root, glm::vec2 offset, bool end) // offset is relative to the root
{
	if (offset == glm::vec2{ 0.f, 0.f })
		return;

	std::string animationkey{};
	if (offset.x > 0.f)
		animationkey = "explosion_right";
	else if (offset.x < 0.f)
		animationkey = "explosion_left";
	else if (offset.y > 0.f)
		animationkey = "explosion_down";
	else if (offset.y < 0.f)
		animationkey = "explosion_up";

	if (end)
		animationkey += "_end";
	else 
		animationkey += "_middle";

	REC::GameObjectDescriptor explosionDescriptor{};
	explosionDescriptor.id = Game::ObjectIds::Explosion;
	explosionDescriptor.startPosX = offset.x;
	explosionDescriptor.startPosY = offset.y;
	explosionDescriptor.renderLayer = std::to_underlying(RenderLayer::Placables);
	explosionDescriptor.parent = root;

	// destroyed upon animation end by the root
	auto explosion = scene->CreateGameObject(explosionDescriptor);

	REC::SpriteDescriptor explosionSpriteDescriptor{};
	explosionSpriteDescriptor.drawHeight = 50;
	explosionSpriteDescriptor.textureKey = "generalSprites";
	explosionSpriteDescriptor.frameDataFileKey = "characterData";
	explosionSpriteDescriptor.drawPointX = 0.5f;
	explosionSpriteDescriptor.drawPointY = 0.5f;

	explosion->AddComponent<REC::SpriteRenderComponent>(explosionSpriteDescriptor);

	REC::AnimationDescriptor animation{};
	animation.animationDataFileKey = "characterData";
	animation.animationKey = animationkey;
	animation.startOnStartup = true;

	explosion->AddComponent<REC::AnimatedSpriteComponent>(animation);

	REC::CollisionDescriptor explosionCollisionDesc{};
	explosionCollisionDesc.collisionType = REC::CollisionType::Static;
	explosionCollisionDesc.bounds.push_back(REC::CollisionBound{ REC::Rect{ -25.f, -25.f, 50.f, 50.f }, true });

	explosion->AddCollisionComponent<REC::CollisionComponent>(explosionCollisionDesc);
	explosion->AddComponent<Game::DebugBoundsRenderComponent>(REC::Color{ 255,0,0 });
}

uint8_t Game::BombComponent::ComputeDirection(int rowStep, int colStep, REC::Event* destructionEvent) const
{
	auto& bombCell = m_Descriptor.grid->GetCell(
		GetOwner()->GetTransform()->GetWorldPosition());

	for (uint8_t range = 1; range <= uint8_t(m_Descriptor.explosionRange); ++range)
	{
		uint8_t row = uint8_t(bombCell.row + rowStep * range);
		uint8_t col = uint8_t(bombCell.col + colStep * range);

		if (!m_Descriptor.grid->IsRowColValid(row, col))
			return range - 1;

		auto& cell = m_Descriptor.grid->GetCell(row, col);

		if (cell.isWall)
			return range - 1;

		if (cell.isDestructableWall)
		{
			auto* args = static_cast<GridEventArgs*>(destructionEvent->GetArgs());
			args->row = row;
			args->col = col;

			destructionEvent->Broadcast();

			return range;
		}
	}

	return uint8_t(m_Descriptor.explosionRange);
}

