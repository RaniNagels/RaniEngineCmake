#include "Player.h"

#include <Scene.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <Input/InputSystem.h>
#include <Input/InputBinding.h>
#include <Events/EventSystem.h>

#include <Components/SpriteRenderComponent.h>
#include <Components/AnimatedSpriteComponent.h>
#include <Components/HealthComponent.h>
#include <Components/LivesComponent.h>

#include "Commands/MoveCommand.h"
#include "Commands/PlaceBombCommand.h"
#include "Components/GridComponent.h"
#include <sdbm_hash.h>

Game::Player::Player(REC::Scene* scene, const PlayerDescriptor& descriptor)
	: m_Descriptor{descriptor}
	, m_Components{}
	, m_Commands{}
{
	REC::GameObjectDescriptor goDesc{};
	if (descriptor.name != 0)
		goDesc.id = descriptor.name;
	goDesc.renderLayer = descriptor.renderLayer;
	goDesc.startPosX = descriptor.startPosition.x;
	goDesc.startPosY = descriptor.startPosition.y;
	m_pGameObject = scene->CreateGameObject(goDesc);
	m_Components.spriteComp = m_pGameObject->AddComponent<REC::SpriteRenderComponent>(m_Descriptor.spriteDesc);
	m_Components.animComp   = m_pGameObject->AddComponent<REC::AnimatedSpriteComponent>(m_Descriptor.animDesc);
	m_Components.healthComp = m_pGameObject->AddComponent<REC::HealthComponent>(m_Descriptor.maxHealth, m_Descriptor.maxHealth);
	m_Components.livesComp  = m_pGameObject->AddComponent<REC::LivesComponent>(m_Descriptor.amountOfLives);

	scene->SetRenderLayer(m_pGameObject, m_Descriptor.renderLayer);
}

Game::Player::~Player() = default;

void Game::Player::CreateInputBindings(REC::InputSystem* inputsystem, REC::SceneManager* sceneManager, float movementSpeed, GridComponent* playfield)
{
	for (int i{}; i < std::to_underlying(InputBindingIndex::Count); ++i)
		m_InputBindings.push_back(inputsystem->CreateInputBinding());
	
	m_InputBindings[std::to_underlying(InputBindingIndex::Up)]->AddCommand<Game::MoveCommand>(m_pGameObject, glm::vec2{ 0, -1 }, movementSpeed, playfield);
	m_InputBindings[std::to_underlying(InputBindingIndex::Down)]->AddCommand<Game::MoveCommand>(m_pGameObject, glm::vec2{ 0, 1 }, movementSpeed, playfield);
	m_InputBindings[std::to_underlying(InputBindingIndex::Left)]->AddCommand<Game::MoveCommand>(m_pGameObject, glm::vec2{ -1, 0 }, movementSpeed, playfield);
	m_InputBindings[std::to_underlying(InputBindingIndex::Right)]->AddCommand<Game::MoveCommand>(m_pGameObject, glm::vec2{ 1, 0 }, movementSpeed, playfield); 
	m_Commands.placeBombCmd = m_InputBindings[std::to_underlying(InputBindingIndex::PlaceBomb)]->AddCommand<Game::PlaceBombCommand>(m_pGameObject, sceneManager);
}
