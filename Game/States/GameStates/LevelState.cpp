#include "LevelState.h"
#include "../../Ids.h"
#include "../../RenderLayers.h"
#include "../../LevelUI.h"
#include "../../Components/GridComponent.h"
#include <Util.h>
#include <Components/SpriteRenderComponent.h>
#include "GameOverState.h"
#include <Events/Event.h>
#include <Components/LivesComponent.h>
#include <Components/CollisionComponent.h>
#include <Components/RigidBodyComponent.h>
#include <Components/AnimationStateComponent.h>
#include "../BombermanStates.h"
#include "../BalloomStates.h"
#include <vector>
#include <Events/Event.h>

#ifdef _DEBUG
#include "../../Components/DebugGridRenderComponent.h"
#include "../../Components/DebugBoundsRenderComponent.h"
#endif // DEBUG


Game::LevelState::LevelState(const REC::EngineContext& context)
	: REC::GameState(context)
{
}

void Game::LevelState::Enter()
{
	auto* scene = CreateScene(Game::SceneIds::LevelScene);
	SubscribeToEvent({ REC::EventIds::LostLive, EventIds::VeryDeathEvent});

	// ---- grid and walls --------------------------------------------------------------------------------
	Game::GridDescriptor gridDesc{};
	gridDesc.cellHeight = uint8_t(48); //51
	gridDesc.cellWidth = uint8_t(48);  //51
	gridDesc.rows = uint8_t(13);
	gridDesc.cols = uint8_t(31);

	REC::SpriteDescriptor backdrop{};
	backdrop.drawHeight = uint16_t(gridDesc.cellHeight) * uint16_t(gridDesc.rows);
	backdrop.frameDataFileKey = "characterData";
	backdrop.frameKey = "background";
	backdrop.textureKey = "background";

	REC::GameObjectDescriptor backdropObjectDesc{};
	backdropObjectDesc.id = Game::ObjectIds::Grid;
	backdropObjectDesc.startPosX = 0.f;
	backdropObjectDesc.startPosY = 80.f;
	backdropObjectDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Background);

	m_pGridObject = scene->CreateGameObject(backdropObjectDesc);
	m_pGridObject->AddComponent<REC::SpriteRenderComponent>(backdrop);
	auto* playfield = m_pGridObject->AddComponent<Game::GridComponent>(gridDesc);
#ifdef _DEBUG
	m_pGridObject->AddComponent<Game::DebugGridRenderComponent>(REC::Color{ 20, 30, 120, 200 });
#endif // DEBUG

	REC::GameObjectDescriptor WallCollisionGODesc{};
	WallCollisionGODesc.id = Game::ObjectIds::Grid;
	WallCollisionGODesc.renderLayer = Util::to_underlying(Game::RenderLayer::Background);
	WallCollisionGODesc.parent = m_pGridObject;

	REC::CollisionDescriptor wallCollisionDesc{};
	wallCollisionDesc.collisionType = REC::CollisionType::Static;
	wallCollisionDesc.bounds = playfield->GetWallCollisionBounds();

	auto* wall_grid = scene->CreateGameObject(WallCollisionGODesc);
	wall_grid->AddCollisionComponent<REC::CollisionComponent>(wallCollisionDesc);
	wall_grid->AddComponent<REC::RigidBodyComponent>();
#ifdef _DEBUG
	wall_grid->AddComponent<Game::DebugBoundsRenderComponent>(REC::Color{ 255,50,0 });
#endif // DEBUG

}

std::optional<std::unique_ptr<REC::GameState>> Game::LevelState::OnEvent(REC::Event* event)
{
	if (event->IsEvent(REC::EventIds::LostLive)) // freese level
	{
		for (auto& player : m_Players)
		{
			player->DisableInputBindings();
			auto* collisionComp = player->Get()->GetCollisionComponent();
			if (collisionComp)
			{
				if (collisionComp->Enabled())
					collisionComp->ToggleEnabled(); // disable collision to prevent further deaths while the death animation is playing
			}
		}
	}
	else if (event->IsEvent(Game::EventIds::VeryDeathEvent)) // reset level OR Game over
	{
		REC::LivesComponent* livesComp = nullptr;
		bool bothPlayersDead = true;
		switch (m_PlayerMode)
		{
		case PlayerMode::SinglePlayer:
			livesComp = m_Players[0]->GetComponents().livesComp;
			if (livesComp->HasLivesLeft())
			{
				ResetLevel();
				return {};
			}
			else
				return std::make_unique<GameOverState>(GetContext());
			break;
		case PlayerMode::CoOp:
			for (auto& player : m_Players)
			{
				livesComp = player->GetComponents().livesComp;
				if (livesComp->HasLivesLeft())
				{
					bothPlayersDead = false;
					break;
				}
			}
			if (!bothPlayersDead)
			{
				ResetLevel();
				return {};
			}
			else
				return std::make_unique<GameOverState>(GetContext());
			break;
		case PlayerMode::Versus:
			for (auto& player : m_Players)
			{
				livesComp = player->GetComponents().livesComp;
				if (!livesComp->HasLivesLeft())
				{
					return std::make_unique<GameOverState>(GetContext());
				}
			}
			ResetLevel();
			break;
		}
	}
	return {};
}

void Game::LevelState::Exit()
{
	for (auto& player : m_Players)
		player->RemoveInputBindings(GetContext().inputSystem);

	UnsubscribeFromEvent({ REC::EventIds::LostLive, EventIds::VeryDeathEvent });
	m_Players.clear();
	m_pLevelUI.reset();
	GetScene()->RemoveAll();
}

void Game::LevelState::CreateLevelUI(Player* player1, Player* player2)
{
	m_pLevelUI = std::make_unique<LevelUI>(GetContext(), player1, player2);
}

Game::Player* Game::LevelState::AddPlayer(const PlayerDescriptor& descriptor)
{
	m_Players.push_back(std::make_unique<Player>(GetScene(), descriptor));
	return m_Players.back().get();
}

void Game::LevelState::ResetLevel()
{
	std::vector<REC::GameObject*> boms = GetScene()->GetGameObjects(ObjectIds::Bom);
	for (auto* bom : boms)
	{
		if (bom->GetCollisionComponent()->Enabled())
			bom->GetCollisionComponent()->ToggleEnabled();

		bom->Destroy();
	}

	for (auto& player : m_Players)
	{
		player->ResetPosition();
		player->EnableInputBindings();
		auto* collisionComp = player->Get()->GetCollisionComponent();
		if (collisionComp)
		{
			if (!collisionComp->Enabled())
				collisionComp->ToggleEnabled(); // re-enable collision after the death animation is done and the level is reset
		}
		auto* aniState = player->Get()->GetComponent<REC::AnimationStateComponent>();
		if (aniState)
		{
			if (player->Get()->Is(ObjectIds::Bomberman))
				aniState->ChangeState(std::make_unique<BombermanIdleState>(player->Get()));
			else if (player->Get()->Is(ObjectIds::Balloom))
				aniState->ChangeState(std::make_unique<BalloomFloatingState>(player->Get()));
		}
	}
}
