#include "LevelUI.h"

#include "Player.h"
#include "States/GameStates.h"
#include "Ids.h"
#include "RenderLayers.h"
#include "Components/UILivesComponent.h"
#include "Components/UIScoreComponent.h"

#include <SceneManager.h>
#include <Util.h>
#include <GameObject.h>
#include <ComponentDescriptors.h>
#include <Components/LivesComponent.h>

Game::LevelUI::LevelUI(const REC::EngineContext& context, Player* p1, Player* p2)
{
	auto* scene = context.sceneManager->GetActiveScene();

	REC::GameObjectDescriptor UIParentDesc{};
	UIParentDesc.startPosX = 50.f;
	UIParentDesc.startPosY = 10.f;
	UIParentDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Ui);
	UIParentDesc.parent = nullptr;

	m_UI = scene->CreateGameObject(UIParentDesc);

	CreatePlayerUI(scene, m_UI, p1, ObjectIds::Player1UI, "P1", 10.f, 10.f);

	if (p2 != nullptr)
	{
		CreatePlayerUI(scene, m_UI, p2, ObjectIds::Player2UI, "P2", 750.f, 10.f);
	}
}

void Game::LevelUI::CreatePlayerUI(REC::Scene* scene, REC::GameObject* parent, Player* player, REC::ObjectId id, const std::string& prefix, float x, float y)
{
	REC::GameObjectDescriptor desc{};
	desc.id = id;
	desc.startPosX = x;
	desc.startPosY = y;
	desc.renderLayer = Util::to_underlying(RenderLayer::Ui);
	desc.parent = parent;

	auto* ui = scene->CreateGameObject(desc);

	REC::LabeledStatDescriptor stat{};
	stat.color = { 255,255,255 };
	stat.fontkey = "dogicapixel20";
	stat.initialValue = player->GetComponents().livesComp->GetLives();
	stat.label = prefix + " Lives";

	auto* lives = ui->AddComponent<UILivesComponent>(stat);
	lives->SetConnectedPlayer(player->Get());

	desc.startPosX = 0.f;
	desc.startPosY = 30.f;
	desc.parent = ui;

	auto* scoreObj = scene->CreateGameObject(desc);

	stat.label = prefix + " Score";
	stat.initialValue = 0;

	auto* score = scoreObj->AddComponent<UIScoreComponent>(stat);
	score->SetConnectedPlayer(player->Get());
}
