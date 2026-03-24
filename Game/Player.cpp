#include "Player.h"

#include <Scene.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <Input/InputSystem.h>
#include <Input/InputBinding.h>

#include <Components/SpriteRenderComponent.h>
#include <Components/AnimatedSpriteComponent.h>
#include <Components/HealthComponent.h>
#include <Components/LivesComponent.h>

#include "Commands/MoveCommand.h"
#include "Commands/PlaceBombCommand.h"
#include <Events/EventSystem.h>

Game::Player::Player(REC::Scene* scene, REC::EventSystem* eventSystem, const PlayerDescriptor& descriptor)
	: m_Descriptor{descriptor}
	, m_Components{}
	, m_Commands{}
	, m_pGameObject{ scene->CreateGameObject(descriptor.startPosition.x, descriptor.startPosition.y) }
	, m_pEventSystem{ eventSystem }
{
	m_Components.spriteComp = m_pGameObject->AddComponent<REC::SpriteRenderComponent>(m_Descriptor.spriteDesc);
	m_Components.animComp   = m_pGameObject->AddComponent<REC::AnimatedSpriteComponent>(m_Descriptor.animDesc);
	m_Components.healthComp = m_pGameObject->AddComponent<REC::HealthComponent>(m_Descriptor.maxHealth, m_Descriptor.maxHealth);
	m_Components.livesComp  = m_pGameObject->AddComponent<REC::LivesComponent>(m_Descriptor.amountOfLives);

	scene->SetRenderLayer(m_pGameObject, m_Descriptor.renderLayer);

	m_pEventSystem->Subscribe(m_Components.livesComp, { REC::make_sdbm_hash("LostLiveEvent")});
}

Game::Player::~Player() = default;

void Game::Player::CreateInputBindings(REC::InputSystem* inputsystem, REC::SceneManager* sceneManager, float movementSpeed)
{
	for (int i{}; i < InputBindingIndex::Count; ++i)
		m_InputBindings.push_back(inputsystem->CreateInputBinding());
	
	m_InputBindings[InputBindingIndex::Up]->AddCommand<Game::MoveCommand>(m_pGameObject, glm::vec2{ 0, -1 }, movementSpeed);
	m_InputBindings[InputBindingIndex::Down]->AddCommand<Game::MoveCommand>(m_pGameObject, glm::vec2{ 0, 1 }, movementSpeed);
	m_InputBindings[InputBindingIndex::Left]->AddCommand<Game::MoveCommand>(m_pGameObject, glm::vec2{ -1, 0 }, movementSpeed);
	m_InputBindings[InputBindingIndex::Right]->AddCommand<Game::MoveCommand>(m_pGameObject, glm::vec2{ 1, 0 }, movementSpeed); 
	m_Commands.placeBombCmd = m_InputBindings[InputBindingIndex::PlaceBomb]->AddCommand<Game::PlaceBombCommand>(m_pGameObject, sceneManager);
}
