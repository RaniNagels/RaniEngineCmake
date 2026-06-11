#include "GameOverState.h"
#include <SceneManager.h>
#include <Events/Event.h>
#include <Input/InputSystem.h>
#include "../../Ids.h"
#include <Util.h>
#include "../../RenderLayers.h"

#include <Components/TextRenderComponent.h>
#include <Components/LabeledStatComponent.h>
#include "../../Commands/MoveMarkerCommand.h"
#include "../../Components/MarkerComponent.h"

#include "../GameStates.h"
#include <Input/InputBinding.h>

Game::GameOverState::GameOverState(const REC::EngineContext& context)
	: REC::GameState(context)
{}

void Game::GameOverState::Enter()
{
	auto* scene = GetContext().sceneManager->CreateScene(Game::SceneIds::GameOverScene);
	SetScene(scene);
	GetContext().sceneManager->SetActiveScene(scene);

	SubscribeToEvent({ Game::EventIds::MarkerSelectedEvent });

	// root
	REC::GameObjectDescriptor instrDesc{};
	instrDesc.id = Game::ObjectIds::GameOverInstructions;
	instrDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	instrDesc.startPosX = 50.f;
	instrDesc.startPosY = 50.f;

	auto* root = scene->CreateGameObject(instrDesc);

	// marker
	glm::vec2 startPos{ 50.f, 100.f };
	REC::GameObjectDescriptor markerDesc{};
	markerDesc.id = Game::ObjectIds::Marker;
	markerDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Player);
	markerDesc.startPosX = startPos.x;
	markerDesc.startPosY = startPos.y;
	markerDesc.parent = root;

	MarkerDescriptor markerCompDesc{};
	markerCompDesc.positions = { startPos, {50.f, 150.f} };
	
	auto* marker = scene->CreateGameObject(markerDesc);
	marker->AddComponent<REC::TextRenderComponent>(">", "dogicapixel20", REC::Color(255, 0, 0));
	m_pMarkerComponent = marker->AddComponent<Game::MarkerComponent>(markerCompDesc);

	auto* moveUpMarker = GetContext().inputSystem->CreateInputBinding();
	moveUpMarker->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_W, REC::ButtonState::Up);
	moveUpMarker->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_Up, REC::ButtonState::Up);
	moveUpMarker->AddCommand<Game::MoveMarkerCommand>(marker, true);
	
	auto* moveDownMarker = GetContext().inputSystem->CreateInputBinding();
	moveDownMarker->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_S, REC::ButtonState::Up);
	moveDownMarker->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_Down, REC::ButtonState::Up);
	moveDownMarker->AddCommand<Game::MoveMarkerCommand>(marker, false);

	auto* selectMarker = GetContext().inputSystem->CreateInputBinding();
	selectMarker->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_Return, REC::ButtonState::Up);
	selectMarker->AddEvent<REC::Event>(Game::EventIds::MarkerSelectedEvent);

	m_InputBindings.push_back(moveUpMarker);
	m_InputBindings.push_back(moveDownMarker);
	m_InputBindings.push_back(selectMarker);

	// safe score instructions
	REC::TextDescriptor safeScoreDesc{};
	safeScoreDesc.color = REC::Color{ 255,255,255 };
	safeScoreDesc.fontKey = "dogicapixel20";
	safeScoreDesc.text = "Save Score";
	
	REC::GameObjectDescriptor safeScoreDescGO{};
	safeScoreDescGO.startPosX = 100.f;
	safeScoreDescGO.startPosY = 100.f;
	safeScoreDescGO.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	safeScoreDescGO.parent = root;

	auto* safeScoreDescGOObj = scene->CreateGameObject(safeScoreDescGO);
	safeScoreDescGOObj->AddComponent<REC::TextRenderComponent>(safeScoreDesc);

	// go to main menu instructions
	REC::TextDescriptor mainMenuDesc{};
	mainMenuDesc.color = REC::Color{ 255,255,255 };
	mainMenuDesc.fontKey = "dogicapixel20";
	mainMenuDesc.text = "Go to Main Menu";

	REC::GameObjectDescriptor mainMenuDescGO{};
	mainMenuDescGO.startPosX = 100.f;
	mainMenuDescGO.startPosY = 150.f;
	mainMenuDescGO.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	mainMenuDescGO.parent = root;

	auto* mainMenuDescGOObj = scene->CreateGameObject(mainMenuDescGO);
	mainMenuDescGOObj->AddComponent<REC::TextRenderComponent>(mainMenuDesc);
}

std::optional<std::unique_ptr<REC::GameState>> Game::GameOverState::OnEvent(REC::Event* event)
{
	if (event->IsEvent(Game::EventIds::MarkerSelectedEvent))
	{
		uint8_t markIndex = m_pMarkerComponent->GetCurrentIndex();
		switch (markIndex)
		{
		case 0:
			// save score
			return {};
		case 1:
			// go to main menu
			return std::make_unique<MainMenuState>(GetContext());
		}
	}
	return {};
}

void Game::GameOverState::Exit()
{
	for (auto* inputBinding : m_InputBindings)
		GetContext().inputSystem->RemoveInputBinding(inputBinding);

	UnsubscribeFromEvent({ Game::EventIds::MarkerSelectedEvent });
	GetScene()->RemoveAll();
}