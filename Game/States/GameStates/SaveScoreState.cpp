#include "SaveScoreState.h"
#include "../../Ids.h"
#include <Events/Event.h>
#include <Scene.h>
#include <GameObject.h>
#include <Util.h>
#include "MainMenuState.h"
#include "../../RenderLayers.h"
#include <Components/LabeledStatComponent.h>
#include "../../UIMarker.h"
#include <Components/TextRenderComponent.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <Input/InputSystem.h>
#include <EngineContext.h>

Game::SaveScoreState::SaveScoreState(const REC::EngineContext& context, uint32_t scoreP1, uint32_t scoreP2)
	: REC::GameState(context)
	, m_ScoreP1(scoreP1)
	, m_ScoreP2(scoreP2)
{}

void Game::SaveScoreState::Enter()
{
	auto* scene = CreateScene(Game::SceneIds::SaveScoreScene);
	SubscribeToEvent({ EventIds::SaveScoreEvent, Game::EventIds::MarkerSelectedEvent });

	// root 
	REC::GameObjectDescriptor rootDesc{};
	rootDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	rootDesc.startPosX = 50.f;
	rootDesc.startPosY = 50.f;

	auto* root = scene->CreateGameObject(rootDesc);

	// player Score UI
	REC::LabeledStatDescriptor playerScoreDesc{};
	playerScoreDesc.fontkey = "dogicapixel20";
	playerScoreDesc.label = "Player 1 Score";
	playerScoreDesc.initialValue = m_ScoreP1;
	playerScoreDesc.color = REC::Color{ 255,255,255 };

	REC::GameObjectDescriptor playerScoreGODesc{};
	playerScoreGODesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	playerScoreGODesc.startPosX = 550.f;
	playerScoreGODesc.startPosY = 20.f;
	playerScoreGODesc.parent = root;

	auto* player1ScoreGO = scene->CreateGameObject(playerScoreGODesc);
	player1ScoreGO->AddComponent<REC::LabeledStatComponent>(playerScoreDesc);
	
	if (m_ScoreP2 > 0)
	{
		playerScoreDesc.label = "Player 2 Score";
		playerScoreDesc.initialValue = m_ScoreP2;
		playerScoreGODesc.startPosY = 60.f;

		auto* player2ScoreGO = scene->CreateGameObject(playerScoreGODesc);
		player2ScoreGO->AddComponent<REC::LabeledStatComponent>(playerScoreDesc);
	}

	std::vector<glm::vec2> markerPositions{};

	// player Selection
	if (m_ScoreP2 > 0)
	{
		REC::TextDescriptor playerSelectDesc{};
		playerSelectDesc.color = REC::Color{ 255,255,0 };
		playerSelectDesc.fontKey = "dogicapixel32";
		playerSelectDesc.text = "P1";

		REC::GameObjectDescriptor playerSelectGODesc{};
		playerSelectGODesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
		playerSelectGODesc.startPosX = 50.f;
		playerSelectGODesc.startPosY = 200.f;
		playerSelectGODesc.parent = root;

		markerPositions.push_back({ playerSelectGODesc.startPosX - 30.f, playerSelectGODesc.startPosY });

		auto* playerSelectGO = scene->CreateGameObject(playerSelectGODesc);
		m_pP1Letter = playerSelectGO->AddComponent<REC::TextRenderComponent>(playerSelectDesc);
		
		playerSelectDesc.color = REC::Color{ 255,255,255 };
		playerSelectDesc.text = "P2";
		playerSelectGODesc.startPosY = 250.f;

		auto* player2SelectGO = scene->CreateGameObject(playerSelectGODesc);
		m_pP2Letter = player2SelectGO->AddComponent<REC::TextRenderComponent>(playerSelectDesc);

		markerPositions.push_back({ playerSelectGODesc.startPosX - 30.f, playerSelectGODesc.startPosY });
	}

	// name input
	REC::TextDescriptor nameInputDesc{};
	nameInputDesc.color = REC::Color{ 255,255,255 };
	nameInputDesc.fontKey = "dogicapixel32";
	nameInputDesc.text = "P1: ";

	REC::GameObjectDescriptor nameInputGODesc{};
	nameInputGODesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	nameInputGODesc.startPosX = 550.f;
	nameInputGODesc.startPosY = 200.f;
	nameInputGODesc.parent = root;

	auto* nameInputGO = scene->CreateGameObject(nameInputGODesc);
	m_pP1Name = nameInputGO->AddComponent<REC::TextRenderComponent>(nameInputDesc);

	if (m_ScoreP2 > 0)
	{
		nameInputDesc.text = "P2: ";
		nameInputGODesc.startPosY = 250.f;
		nameInputGO = scene->CreateGameObject(nameInputGODesc);
		m_pP2Name = nameInputGO->AddComponent<REC::TextRenderComponent>(nameInputDesc);
	}

	// letters
	REC::TextDescriptor letterDesc{};
	letterDesc.color = REC::Color{ 255,255,255 };
	letterDesc.fontKey = "dogicapixel32";

	REC::GameObjectDescriptor letterDescGO{};
	letterDescGO.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	letterDescGO.parent = root;

	for (size_t i{}; i < m_Letters.size(); ++i)
	{
		// two columns of letters, so the x position is determined by the column and the y position is determined by the row
		float x = 150.f + (i / 13) * 70.f; // 13 letters per column
		float y = 30.f + (i % 13) * 50.f; 
		PrintLetter(letterDesc, letterDescGO, x, y, m_Letters[i]);
		markerPositions.push_back({ x - 30.f, y });
	}

	// marker
	UIMarkerDescriptor markerDesc{};
	markerDesc.positions = markerPositions;
	markerDesc.scene = scene;
	markerDesc.root = root;
	markerDesc.drawSize = { 20.f, 20.f };

	m_pUIMarker = std::make_unique<UIMarker>(markerDesc);
	m_pUIMarker->CreateInputBindings(GetContext().inputSystem);

	// highscores
	OpenSaveFile();
	REC::TextDescriptor highscoreDesc{};
	highscoreDesc.color = REC::Color{ 255,50,255 };
	highscoreDesc.fontKey = "dogicapixel20";
	highscoreDesc.text = "Highscores:";

	REC::GameObjectDescriptor highscoreGODesc{};
	highscoreGODesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	highscoreGODesc.startPosX = 1000.f;
	highscoreGODesc.startPosY = 20.f;
	highscoreGODesc.parent = root;
	
	auto* highscoreGO = scene->CreateGameObject(highscoreGODesc);
	highscoreGO->AddComponent<REC::TextRenderComponent>(highscoreDesc);

	auto split = [](const std::string& str, char delimiter) -> std::vector<std::string>
		{
			std::vector<std::string> tokens;
			std::string token;
			std::istringstream tokenStream(str);
			while (std::getline(tokenStream, token, delimiter))
			{
				tokens.push_back(token);
			}
			return tokens;
		};

	std::vector<std::string> highscoreEntries = split(m_FileContent, ',');
	// split m_FileContent into lines
	float entryStartY = 40.f; // start below the "Highscores:" text
	for (auto& entry : highscoreEntries)
	{
		REC::TextDescriptor entryDesc{};
		entryDesc.color = REC::Color{ 255,50,255 };
		entryDesc.fontKey = "dogicapixel20";
		entryDesc.text = entry;

		REC::GameObjectDescriptor entryGODesc{};
		entryGODesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
		entryGODesc.startPosX = 0.f;
		entryGODesc.startPosY = entryStartY;
		entryStartY += 30.f; 
		entryGODesc.parent = highscoreGO;

		auto* entryGO = scene->CreateGameObject(entryGODesc);
		entryGO->AddComponent<REC::TextRenderComponent>(entryDesc);
	}

	// save instructions
	REC::TextDescriptor saveDesc{};
	saveDesc.color = REC::Color{ 255,255,255 };
	saveDesc.fontKey = "dogicapixel32";
	saveDesc.text = "Press Y to save your score!";

	REC::GameObjectDescriptor saveGODesc{};
	saveGODesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	saveGODesc.startPosX = 550.f;
	saveGODesc.startPosY = 400.f;
	saveGODesc.parent = root;
	
	auto* saveGO = scene->CreateGameObject(saveGODesc);
	saveGO->AddComponent<REC::TextRenderComponent>(saveDesc);

	m_InputBindings.push_back(GetContext().inputSystem->CreateInputBinding());
	m_InputBindings.back()->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_Y, REC::ButtonState::Up);
	m_InputBindings.back()->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::Gamepad_Y, REC::ButtonState::Up);
	m_InputBindings.back()->AddEvent<REC::Event>(EventIds::SaveScoreEvent);
}

std::optional<std::unique_ptr<REC::GameState>> Game::SaveScoreState::OnEvent(REC::Event* event)
{
	if (event->IsEvent(EventIds::SaveScoreEvent))
	{
		SaveFile();
		return std::make_unique<MainMenuState>(GetContext());
	}
	else if (event->IsEvent(EventIds::MarkerSelectedEvent))
	{
		uint8_t markerIndex = m_pUIMarker->GetCurrentIndex();
		if (markerIndex == 0 && m_ScoreP2 > 0)
		{
			m_FirstPlayer = true;
			m_pP1Letter->SetColor(REC::Color{ 255,255,0 });
			m_pP2Letter->SetColor(REC::Color{ 255,255,255 });
			return{};
		}
		else if (markerIndex == 1 && m_ScoreP2 > 0)
		{
			m_FirstPlayer = false;
			m_pP2Letter->SetColor(REC::Color{ 255,255,0 });
			m_pP1Letter->SetColor(REC::Color{ 255,255,255 });
			return{};
		}


		uint8_t letterOffset = m_ScoreP2 > 0 ? 2 : 0; 

		REC::TextRenderComponent* nameComp = nullptr;
		if (m_FirstPlayer)
			nameComp = m_pP1Name;
		else
			nameComp = m_pP2Name;

		if (markerIndex == m_pUIMarker->GetAmountOfPositions() - 1)
		{
			auto text = nameComp->GetText();
			// only remove last character if there is more than just "P1: " or "P2: "
			if (text.size() > 4)
			{
				text = text.substr(0, text.size() - 1); // remove last character
				nameComp->SetText(text);
			}
			return {};
		}

		nameComp->SetText(nameComp->GetText() + m_Letters[markerIndex - letterOffset]);
	}
	return {};
}

void Game::SaveScoreState::Exit()
{
	UnsubscribeFromEvent({ EventIds::SaveScoreEvent, Game::EventIds::MarkerSelectedEvent });
	m_pUIMarker->DeleteInputBindings(GetContext().inputSystem);

	UnsubscribeFromEvent({ Game::EventIds::MarkerSelectedEvent });
	GetScene()->RemoveAll();
}

void Game::SaveScoreState::PrintLetter(REC::TextDescriptor& desc, REC::GameObjectDescriptor& goDesc, float x, float y, char letter)
{
	desc.text = std::string(1, letter);
	goDesc.startPosX = x;
	goDesc.startPosY = y;

	REC::GameObject* letterGO = GetScene()->CreateGameObject(goDesc);
	letterGO->AddComponent<REC::TextRenderComponent>(desc);
}

void Game::SaveScoreState::OpenSaveFile()
{
	std::ifstream file(m_SafeFilePath);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file: " + m_SafeFilePath);

	std::stringstream buffer;
	buffer << file.rdbuf();

	m_FileContent = buffer.str();
}

void Game::SaveScoreState::SaveFile()
{
	if (m_pP1Name)
	{
		auto name = m_pP1Name->GetText();
		// remove "P1: " from the name
		name = name.substr(4);
		m_FileContent += name + " " + std::to_string(m_ScoreP1) + ",";
	}
	
	if (m_pP2Name)
	{
		auto name = m_pP2Name->GetText();
		name = name.substr(4);
		m_FileContent += name + " " + std::to_string(m_ScoreP2) + ",";
	}

	std::ofstream file(m_SafeFilePath);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file: " + m_SafeFilePath);

	file << m_FileContent;
}
