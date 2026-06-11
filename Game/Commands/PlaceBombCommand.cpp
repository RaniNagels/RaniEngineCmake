#include "PlaceBombCommand.h"

#include <GameObject.h>
#include <Components/TransformComponent.h>
#include <Components/SpriteRenderComponent.h>
#include <Components/AnimatedSpriteComponent.h>
#include <Components/CollisionComponent.h>
#include <Events/Event.h>
#include <SceneManager.h>
#include <ServiceLocator.h>
#include <Util.h>

#include "../RenderLayers.h"
#include "../Components/BombComponent.h"
#include "../Components/DebugBoundsRenderComponent.h"
#include "../Components/GridComponent.h"
#include "../Ids.h"

Game::PlaceBombCommand::PlaceBombCommand(REC::GameObject* actor, REC::SceneManager* sceneManager)
	: GameObjectInputCommand(actor)
	, m_pSceneManager{sceneManager}
{
	REC::GameObjectEventArgs args{};
	args.sender = GetGameObject();
	m_HasPlacedBombEvent = std::make_unique<REC::Event>(Game::EventIds::HasPlaceBombEvent, args);
}

Game::PlaceBombCommand::~PlaceBombCommand() = default;

void Game::PlaceBombCommand::Execute(float)
{
	auto activeScene = m_pSceneManager->GetActiveScene();

	REC::GameObject* go = activeScene->GetGameObject(Game::ObjectIds::Grid);
	auto* grid = go->GetComponent<Game::GridComponent>();

	auto PlayerPosition = GetGameObject()->GetTransform()->GetWorldPosition();
	glm::vec2 PlayerPosition2D{ PlayerPosition.x, PlayerPosition.y };
	auto& cell = grid->GetCell(PlayerPosition2D);
	auto bombPosition = grid->GetAbsoluteCellPosition(cell); // returns the top left corner
	auto centeredBombPosition = bombPosition + glm::vec2{ grid->GetCell(0,0).width / 2.f, grid->GetCell(0,0).height / 2.f };

	REC::GameObjectDescriptor bombDescriptor{};
	bombDescriptor.id = Game::ObjectIds::Bom;
	bombDescriptor.startPosX = centeredBombPosition.x;
	bombDescriptor.startPosY = centeredBombPosition.y;
	bombDescriptor.renderLayer = Util::to_underlying(Game::RenderLayer::Placables);

	auto bomb = activeScene->CreateGameObject(bombDescriptor);

	REC::SpriteDescriptor bombSpriteDescriptor{};
	bombSpriteDescriptor.drawHeight = 50;
	bombSpriteDescriptor.textureKey = "generalSprites";
	bombSpriteDescriptor.frameDataFileKey = "characterData";
	bombSpriteDescriptor.frameKey = "bom_0";
	bombSpriteDescriptor.drawPointX = 0.5f;
	bombSpriteDescriptor.drawPointY = 0.5f;

	bomb->AddComponent<REC::SpriteRenderComponent>(bombSpriteDescriptor);

	REC::AnimationDescriptor animation{};
	animation.animationDataFileKey = "characterData";
	animation.animationKey = "bom";
	animation.startOnStartup = true;

	bomb->AddComponent<REC::AnimatedSpriteComponent>(animation);

	Game::BombDescriptor bombCompDesc{};
	bombCompDesc.lifeTime = 1.3f;
	bombCompDesc.grid = grid;
	bombCompDesc.scene = activeScene;

	bomb->AddComponent<Game::BombComponent>(bombCompDesc);

	REC::CollisionDescriptor bombCollisionDesc{};
	bombCollisionDesc.collisionType = REC::CollisionType::Static;
	//bombCollisionDesc.bounds.emplace_back(REC::Rect{ -20.f, -20.f, 40.f, 40.f }); // centered on the bomb
	// no bounds yet, only on detonation the bounds are needed and added

	bomb->AddCollisionComponent<REC::CollisionComponent>(bombCollisionDesc);
	bomb->AddComponent<Game::DebugBoundsRenderComponent>(REC::Color{255,0,0});

	REC::ServiceLocator::GetSoundSystem().Play("placeBombSound", 0.5f);

	m_HasPlacedBombEvent->Broadcast();
}
