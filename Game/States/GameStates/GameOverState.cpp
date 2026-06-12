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
#include <Components/SpriteRenderComponent.h>

#include <Input/InputBinding.h>
#include "MainMenuState.h"
#include "../../UIMarker.h"

Game::GameOverState::GameOverState(const REC::EngineContext& context)
	: REC::GameState(context)
{}

void Game::GameOverState::Enter()
{
	REC::Scene* scene = CreateScene(Game::SceneIds::GameOverScene);

	SubscribeToEvent({ Game::EventIds::MarkerSelectedEvent });

	// root
	REC::GameObjectDescriptor instrDesc{};
	instrDesc.id = Game::ObjectIds::GameOverInstructions;
	instrDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	instrDesc.startPosX = 50.f;
	instrDesc.startPosY = 50.f;

	auto* root = scene->CreateGameObject(instrDesc);

	// marker
	UIMarkerDescriptor markerDesc{};
	markerDesc.positions = { {50.f, 100.f}, {50.f, 150.f} };
	markerDesc.scene = scene;
	markerDesc.root = root;
	markerDesc.drawSize = { 20.f, 20.f };

	m_pUIMarker = std::make_unique<UIMarker>(markerDesc);
	m_pUIMarker->CreateInputBindings(GetContext().inputSystem);

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
		uint8_t markIndex = m_pUIMarker->GetCurrentIndex();
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

	m_pUIMarker->DeleteInputBindings(GetContext().inputSystem);

	UnsubscribeFromEvent({ Game::EventIds::MarkerSelectedEvent });
	GetScene()->RemoveAll();
}