#include "LevelUI.h"

#include "Player.h"
#include "States/GameStates.h"
#include "Ids.h"
#include "RenderLayers.h"
#include "Components/UILivesComponent.h"

#include <SceneManager.h>
#include <Util.h>
#include <GameObject.h>
#include <ComponentDescriptors.h>
#include <Components/LivesComponent.h>

Game::LevelUI::LevelUI(const REC::EngineContext& context, Player* p1, Player* p2)
{
	auto* scene = context.sceneManager->GetActiveScene();

	REC::GameObjectDescriptor UIParentDesc{};
	UIParentDesc.startPosX = 0.f;
	UIParentDesc.startPosY = 10.f;
	UIParentDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	UIParentDesc.parent = nullptr;

	m_UI = scene->CreateGameObject(UIParentDesc);

	// p1 needs to valid, but p2 can be null (if playing single player)
	REC::GameObjectDescriptor player1UIDesc{};
	player1UIDesc.id = Game::ObjectIds::Player1UI;
	player1UIDesc.startPosX = 10.f;
	player1UIDesc.startPosY = 10.f;
	player1UIDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	player1UIDesc.parent = m_UI;

	REC::GameObject* uiP1 = scene->CreateGameObject(player1UIDesc);

	REC::LabeledStatDescriptor livesDesc{};
	livesDesc.color = REC::Color{ 255, 255, 255 };
	livesDesc.fontkey = "dogicapixel20";
	livesDesc.initialValue = p1->GetComponents().livesComp->GetLives();
	livesDesc.label = "P1 Lives";
	auto* uiP1Lives = uiP1->AddComponent<UILivesComponent>(livesDesc);
	uiP1Lives->SetConnectedPlayer(p1->Get());

	if (p2 != nullptr)
	{
		REC::GameObjectDescriptor player2UIDesc{};
		player2UIDesc.id = Game::ObjectIds::Player2UI;
		player2UIDesc.startPosX = 500.f;
		player2UIDesc.startPosY = 10.f;
		player2UIDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
		player2UIDesc.parent = m_UI;

		REC::GameObject* uiP2 = scene->CreateGameObject(player2UIDesc);

		livesDesc.label = "P2 Lives";
		livesDesc.initialValue = p2->GetComponents().livesComp->GetLives();
		uiP2->AddComponent<UILivesComponent>(livesDesc);
	}
}
