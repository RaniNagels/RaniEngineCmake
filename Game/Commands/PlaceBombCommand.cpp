#include "PlaceBombCommand.h"
#include <GameObject.h>
#include <Components/TransformComponent.h>
#include <Components/SpriteRenderComponent.h>
#include <Components/AnimatedSpriteComponent.h>
#include <sdbm_hash.h>
#include "../RenderLayers.h"
#include <Event.h>

Game::PlaceBombCommand::PlaceBombCommand(REC::GameObject* actor, REC::SceneManager* sceneManager)
	: GameObjectInputCommand(actor)
	, m_pSceneManager{sceneManager}
	, m_HasPlacedBombEvent{std::make_unique<REC::ValueChangedEvent>(REC::make_sdbm_hash("HasPlacedBombEvent"))}
{
}

Game::PlaceBombCommand::~PlaceBombCommand() = default;

void Game::PlaceBombCommand::Execute(float)
{
	auto activeScene = m_pSceneManager->GetActiveScene();

	// TODO: place in grid, not just willy nilly on the GameObject
	auto PlayerPosition = GetGameObject()->GetTransform()->GetWorldPosition();
	auto bomb = activeScene->CreateGameObject(PlayerPosition.x, PlayerPosition.y);
	activeScene->SetRenderLayer(bomb, GetLayer(RenderLayer::Placables));

	REC::SpriteDescriptor bombSpriteDescriptor{};
	bombSpriteDescriptor.drawHeight = 50;
	bombSpriteDescriptor.textureKey = "generalSprites";
	bombSpriteDescriptor.frameDataFileKey = "characterData";
	bombSpriteDescriptor.frameKey = "bom_0";

	bomb->AddComponent<REC::SpriteRenderComponent>(bombSpriteDescriptor);

	REC::AnimationDescriptor animation{};
	animation.animationDataFileKey = "characterData";
	animation.animationKey = "bom";
	animation.startOnStartup = true;

	bomb->AddComponent<REC::AnimatedSpriteComponent>(animation);

	m_HasPlacedBombEvent->NotifyListeners();
}

void Game::PlaceBombCommand::SubscribeToEvents(REC::IListener* listener)
{
	m_HasPlacedBombEvent->Subscribe(listener);
}

void Game::PlaceBombCommand::UnSubscribeToEvents(REC::IListener* listener)
{
	m_HasPlacedBombEvent->Unsubscribe(listener);
}
