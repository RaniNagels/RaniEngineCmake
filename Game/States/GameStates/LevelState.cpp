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
#include "../../Components/DoorCollisionComponent.h"
#include <Components/RigidBodyComponent.h>
#include <Components/AnimationStateComponent.h>
#include "../BombermanStates.h"
#include "../BalloomStates.h"
#include <vector>
#include <Resources/IResourceManager.h>
#include "../../Components/SoftBlockComponent.h"
#include <Input/InputSystem.h>
#include <Input/InputAction.h>

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
	SubscribeToEvent({ REC::EventIds::LostLive, EventIds::VeryDeathEvent, EventIds::DoorOpenEvent, EventIds::SkipLevelEvent });

	// ---- grid and walls --------------------------------------------------------------------------------
	Game::GridDescriptor gridDesc{};
	gridDesc.cellHeight = uint8_t(48); //51
	gridDesc.cellWidth = uint8_t(48);  //51
	gridDesc.rows = uint8_t(13);
	gridDesc.cols = uint8_t(31);
	gridDesc.levelInfo = GetContext().resourceManager->GetLevelInfo("1");

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

	LoadLevel();

	auto* skipLevel = GetContext().inputSystem->CreateInputBinding();
	skipLevel->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_F1, REC::ButtonState::Up);
	skipLevel->AddEvent<REC::Event>(EventIds::SkipLevelEvent);
}

std::optional<std::unique_ptr<REC::GameState>> Game::LevelState::OnEvent(REC::Event* event)
{
	if (event->IsEvent(REC::EventIds::LostLive)) // freese level
	{
		DisablePlayers();
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
				return std::make_unique<GameOverState>(GetContext(), m_pLevelUI->GetPlayer1Score(), m_pLevelUI->GetPlayer2Score());
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
				return std::make_unique<GameOverState>(GetContext(), m_pLevelUI->GetPlayer1Score(), m_pLevelUI->GetPlayer2Score());
			break;
		case PlayerMode::Versus:
			for (auto& player : m_Players)
			{
				livesComp = player->GetComponents().livesComp;
				if (!livesComp->HasLivesLeft())
				{
					return std::make_unique<GameOverState>(GetContext(), m_pLevelUI->GetPlayer1Score(), m_pLevelUI->GetPlayer2Score());
				}
			}
			ResetLevel();
			break;
		}
	}
	else if (event->IsEvent(EventIds::DoorOpenEvent) || event->IsEvent(EventIds::SkipLevelEvent)) // next level
	{
		DisablePlayers(); 
		ResetLevel();
		auto* playfield = m_pGridObject->GetComponent<Game::GridComponent>();
		if (!playfield->LoadNextLevel(GetContext()))
			return std::make_unique<GameOverState>(GetContext(), m_pLevelUI->GetPlayer1Score(), m_pLevelUI->GetPlayer2Score());

		LoadLevel();
	}
	return {};
}

void Game::LevelState::Exit()
{
	for (auto& player : m_Players)
		player->RemoveInputBindings(GetContext().inputSystem);

	UnsubscribeFromEvent({ REC::EventIds::LostLive, EventIds::VeryDeathEvent, EventIds::DoorOpenEvent, EventIds::SkipLevelEvent });
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

	EnablePlayers();
}

void Game::LevelState::LoadLevel()
{
	auto* playfield = m_pGridObject->GetComponent<Game::GridComponent>();
	playfield->ResetGrid();
	auto* levelInfo = playfield->GetLevelInfo();

	// destroy soft blocks, doors, and powerups
	std::vector<REC::GameObject*> softBlocks = GetScene()->GetGameObjects(ObjectIds::DestructableWall);
	for (auto* softBlock : softBlocks)
		softBlock->Destroy();

	std::vector<REC::GameObject*> doors = GetScene()->GetGameObjects(ObjectIds::Door); // only one should be in the scene
	for (auto* door : doors)
		door->Destroy(); 

	std::vector<REC::GameObject*> powerUps = GetScene()->GetGameObjects(ObjectIds::PickUp);
	for (auto* powerUp : powerUps)
		powerUp->Destroy();

	// recreate new soft blocks
	for (const auto& brick : levelInfo->bricks)
		CreateSoftBlock(brick.first, brick.second);

	// set cell with exit door
	auto index = levelInfo->exitBrickIndex;
	softBlocks.clear();
	softBlocks = GetScene()->GetGameObjects(ObjectIds::DestructableWall);
	if (index < softBlocks.size())
	{
		auto* softBlockComp = softBlocks[index]->GetComponent<SoftBlockComponent>();
		playfield->ModifyCell(softBlockComp->GetRow() , softBlockComp->GetCol(), true, true);
	}

	// idem for pickup
	auto pickupIndex = levelInfo->powerUps;
	if (pickupIndex.first < softBlocks.size())
	{
		uint8_t pickupType = uint8_t(-1);
		if (pickupIndex.second == "ExtraBomb")
			pickupType = 0;
		else if (pickupIndex.second == "Detonator")
			pickupType = 1;
		else if (pickupIndex.second == "Flames")
			pickupType = 2;

		auto* softBlockComp = softBlocks[pickupIndex.first]->GetComponent<SoftBlockComponent>();
		playfield->ModifyCell(softBlockComp->GetRow(), softBlockComp->GetCol(), true, false, true, pickupType);
	}
}

void Game::LevelState::DisablePlayers()
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

void Game::LevelState::EnablePlayers()
{
	auto* playfield = m_pGridObject->GetComponent<Game::GridComponent>();
	auto* levelInfo = playfield->GetLevelInfo();
	glm::vec2 startPosition1{};
	glm::vec2 startPosition2{};

	startPosition1 = playfield->GetAbsoluteCellPosition(levelInfo->player1StartCell.first, levelInfo->player1StartCell.second);
	startPosition2 = playfield->GetAbsoluteCellPosition(levelInfo->player2StartCell.first, levelInfo->player2StartCell.second);
	float offset = playfield->GetCellSize().x / 2.f; // to center the player in the cell
	startPosition1 += glm::vec2{ offset, offset };
	startPosition2 += glm::vec2{ offset, offset };

	bool firstPlayer = true;
	for (auto& player : m_Players)
	{
		glm::vec2 newPos = firstPlayer ? startPosition1 : startPosition2;
		player->ResetPosition(newPos);
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
		firstPlayer = false;
	}
}

void Game::LevelState::CreateSoftBlock(uint8_t col, uint8_t row)
{
	auto* grid = m_pGridObject->GetComponent<Game::GridComponent>();
	
	auto position = grid->GetAbsoluteCellPosition(row, col);
	grid->ModifyCell(row, col, true);

	REC::GameObjectDescriptor softBlockDesc{};
	softBlockDesc.id = Game::ObjectIds::DestructableWall;
	softBlockDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Placables);
	softBlockDesc.startPosX = position.x;
	softBlockDesc.startPosY = position.y;

	REC::SpriteDescriptor softBlockSpriteDesc{};
	softBlockSpriteDesc.drawHeight = uint16_t(grid->GetCellSize().y);
	softBlockSpriteDesc.drawWidth = uint16_t(grid->GetCellSize().x);
	softBlockSpriteDesc.textureKey = "generalSprites";
	softBlockSpriteDesc.frameDataFileKey = "characterData";
	softBlockSpriteDesc.frameKey = "soft_block";

	auto* softBlock = GetScene()->CreateGameObject(softBlockDesc);
	softBlock->AddComponent<REC::SpriteRenderComponent>(softBlockSpriteDesc);
	
	REC::CollisionDescriptor softBlockCollisionDesc{};
	softBlockCollisionDesc.collisionType = REC::CollisionType::Static;
	softBlockCollisionDesc.bounds.emplace_back(REC::CollisionBound{ REC::Rect{ 0.f, 0.f, float(grid->GetCellSize().x), float(grid->GetCellSize().y) }, false }); 
	softBlock->AddCollisionComponent<REC::CollisionComponent>(softBlockCollisionDesc);
	softBlock->AddComponent<REC::RigidBodyComponent>();
	softBlock->AddComponent<SoftBlockComponent>(GetScene(), row, col);
}
