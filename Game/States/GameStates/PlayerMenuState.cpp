#include "PlayerMenuState.h"
#include <Input/InputSystem.h>
#include <Input/InputBinding.h>
#include <Events/Event.h>
#include "../../Ids.h"
#include "../../Components/MarkerComponent.h"
#include <Util.h>
#include "../../RenderLayers.h"
#include "../../Commands/MoveMarkerCommand.h"
#include <Components/SpriteRenderComponent.h>
#include "../../UIMarker.h"
#include <Components/TextRenderComponent.h>
#include "LevelSinglePlayer.h"
#include "LevelCoOp.h"
#include "LevelVersus.h"

Game::PlayerMenuState::PlayerMenuState(const REC::EngineContext& context)
	: REC::GameState(context)
{}

void Game::PlayerMenuState::Enter()
{
	auto* scene = CreateScene(Game::SceneIds::PlayerMenuScene);
	SubscribeToEvent({ Game::EventIds::MarkerSelectedEvent });

	REC::TextDescriptor commonInstrDesc{};
	commonInstrDesc.color = REC::Color{ 255,255,255 };
	commonInstrDesc.fontKey = "dogicapixel20";

	// root
	REC::GameObjectDescriptor instrDesc{};
	instrDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	instrDesc.startPosX = 50.f;
	instrDesc.startPosY = 50.f;

	auto* root = scene->CreateGameObject(instrDesc);

	// Select Mode Instructions
	REC::GameObjectDescriptor instrTitleDesc{};
	instrTitleDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	instrTitleDesc.startPosX = 50.f;
	instrTitleDesc.startPosY = 50.f;
	instrTitleDesc.parent = root;

	REC::TextDescriptor instrTitleTextDesc{ commonInstrDesc };
	instrTitleTextDesc.text = "Select Mode";

	auto* instrTitle = scene->CreateGameObject(instrTitleDesc);
	instrTitle->AddComponent<REC::TextRenderComponent>(instrTitleTextDesc);

	// marker
	UIMarkerDescriptor markerDesc{};
	markerDesc.positions = { {50.f, 100.f}, {50.f, 150.f}, {50.f, 200.f} };
	markerDesc.scene = scene;
	markerDesc.root = root;
	markerDesc.drawSize = { 20.f, 20.f };

	m_pUIMarker = std::make_unique<UIMarker>(markerDesc);
	m_pUIMarker->CreateInputBindings(GetContext().inputSystem);

	// single player
	REC::GameObjectDescriptor singlePlayerDesc{};
	singlePlayerDesc.startPosX = 100.f;
	singlePlayerDesc.startPosY = 100.f;
	singlePlayerDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	singlePlayerDesc.parent = root;

	REC::TextDescriptor singlePlayerTextDesc{ commonInstrDesc };
	singlePlayerTextDesc.text = "Single Player";
	
	auto* singlePlayerOB = scene->CreateGameObject(singlePlayerDesc);
	singlePlayerOB->AddComponent<REC::TextRenderComponent>(singlePlayerTextDesc);

	// co op
	REC::GameObjectDescriptor coopPlayerDesc{};
	coopPlayerDesc.startPosX = 100.f;
	coopPlayerDesc.startPosY = 150.f;
	coopPlayerDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	coopPlayerDesc.parent = root;

	REC::TextDescriptor coopPlayerTextDesc{ commonInstrDesc };
	coopPlayerTextDesc.text = "Co Op";

	auto* coopPlayerOB = scene->CreateGameObject(coopPlayerDesc);
	coopPlayerOB->AddComponent<REC::TextRenderComponent>(coopPlayerTextDesc);

	// versus
	REC::GameObjectDescriptor versusPlayerDesc{};
	versusPlayerDesc.startPosX = 100.f;
	versusPlayerDesc.startPosY = 200.f;
	versusPlayerDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	versusPlayerDesc.parent = root;

	REC::TextDescriptor versusPlayerTextDesc{ commonInstrDesc };
	versusPlayerTextDesc.text = "Versus";

	auto* versusPlayerOB = scene->CreateGameObject(versusPlayerDesc);
	versusPlayerOB->AddComponent<REC::TextRenderComponent>(versusPlayerTextDesc);
}

std::optional<std::unique_ptr<REC::GameState>> Game::PlayerMenuState::OnEvent(REC::Event * event)
{
	if (event->IsEvent(Game::EventIds::MarkerSelectedEvent))
	{
		uint8_t markerIndex = m_pUIMarker->GetCurrentIndex();
		switch (markerIndex)
		{
		case 0:
			// single player
			return std::make_unique<LevelSinglePlayer>(GetContext());
		case 1:
			// Co op
			return std::make_unique<LevelCoOp>(GetContext());
		case 2:
			return std::make_unique<LevelVersus>(GetContext());
		}
	}
	return {};
}

void Game::PlayerMenuState::Exit()
{
	UnsubscribeFromEvent({ Game::EventIds::MarkerSelectedEvent });

	for (auto* inputBinding : m_InputBindings)
		GetContext().inputSystem->RemoveInputBinding(inputBinding);

	m_pUIMarker->DeleteInputBindings(GetContext().inputSystem);
}
