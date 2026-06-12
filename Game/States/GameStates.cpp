#include "GameStates.h"
#include "../Ids.h"
#include "../RenderLayers.h"

#include <SceneManager.h>
#include <Events/Event.h>
#include <Input/InputSystem.h>
#include <Util.h>
#include <GameObject.h>

#include <Components/SpriteRenderComponent.h>
#include <Components/TextRenderComponent.h>
#include <Components/RigidBodyComponent.h>
#include <Components/AnimatedSpriteComponent.h>
#include <Components/AnimationStateComponent.h>
#include <Components/LivesComponent.h>
#include <Components/CollisionComponent.h>

#include "../Components/DebugGridRenderComponent.h"
#include "../Components/DebugBoundsRenderComponent.h"
#include "../Components/BombermanCollisionComponent.h"
#include "../Components/GridComponent.h"

#include "BombermanStates.h"
#include "GameStates/GameOverState.h"
#include "GameStates/MainMenuState.h"

// ============================================================================================================================
Game::LevelState::LevelState(const REC::EngineContext& context)
	: REC::GameState(context)
{}

void Game::LevelState::Enter()
{
	auto* scene = CreateScene(Game::SceneIds::LevelScene);

	SubscribeToEvent({ REC::EventIds::LostLive, EventIds::VeryDeathEvent });

	// ---- grid and walls --------------------------------------------------------------------------------
	Game::GridDescriptor gridDesc{};
	gridDesc.cellHeight = uint8_t(50); //51
	gridDesc.cellWidth = uint8_t(50);  //51
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

	auto* go_grid = scene->CreateGameObject(backdropObjectDesc);
	go_grid->AddComponent<REC::SpriteRenderComponent>(backdrop);
	auto* playfield = go_grid->AddComponent<Game::GridComponent>(gridDesc);
	go_grid->AddComponent<Game::DebugGridRenderComponent>(REC::Color{ 20, 30, 120, 200 });

	REC::GameObjectDescriptor WallCollisionGODesc{};
	WallCollisionGODesc.id = Game::ObjectIds::Grid;
	WallCollisionGODesc.renderLayer = Util::to_underlying(Game::RenderLayer::Background);
	WallCollisionGODesc.parent = go_grid;

	REC::CollisionDescriptor wallCollisionDesc{};
	wallCollisionDesc.collisionType = REC::CollisionType::Static;
	wallCollisionDesc.bounds = playfield->GetWallCollisionBounds();

	auto* wall_grid = scene->CreateGameObject(WallCollisionGODesc);
	wall_grid->AddCollisionComponent<REC::CollisionComponent>(wallCollisionDesc);
	wall_grid->AddComponent<REC::RigidBodyComponent>();
	wall_grid->AddComponent<Game::DebugBoundsRenderComponent>(REC::Color{ 255,50,0 });

	// ---- Players --------------------------------------------------------------------------------------
	REC::SpriteDescriptor charactersSpriteDescriptors{};
	charactersSpriteDescriptors.drawHeight = 50;
	charactersSpriteDescriptors.frameDataFileKey = "characterData";
	charactersSpriteDescriptors.textureKey = "generalSprites";
	charactersSpriteDescriptors.drawPointX = 0.5f;
	charactersSpriteDescriptors.drawPointY = 0.5f;

	REC::AnimationDescriptor bombermanWalkAnimDesc{};
	bombermanWalkAnimDesc.animationDataFileKey = "characterData";
	bombermanWalkAnimDesc.animationKey = "bomberman_walk_left";
	bombermanWalkAnimDesc.startOnStartup = true;

	Game::PlayerDescriptor bombermanDescriptor{};
	bombermanDescriptor.name = Game::ObjectIds::Bomberman;
	bombermanDescriptor.amountOfLives = 3;
	bombermanDescriptor.animDesc = bombermanWalkAnimDesc;
	bombermanDescriptor.spriteDesc = charactersSpriteDescriptors;
	bombermanDescriptor.renderLayer = Util::to_underlying(Game::RenderLayer::Player);
	bombermanDescriptor.startPosition = { 250.f, 250.f };

	m_Players.reserve(2);
	m_Players.push_back(std::make_unique<Game::Player>(scene, bombermanDescriptor));
	auto& bomberman = *m_Players.back().get();
	auto bombermanGO = bomberman.Get();

	REC::CollisionDescriptor collisionDescriptor{};
	collisionDescriptor.collisionType = REC::CollisionType::Dynamic;
	collisionDescriptor.bounds.emplace_back(REC::CollisionBound{ REC::Rect{ -18.f, -18.f, 36.f, 36.f}, true }); // centered on the player

	bombermanGO->AddCollisionComponent<Game::BombermanCollisionComponent>(collisionDescriptor);
	bombermanGO->AddComponent<REC::RigidBodyComponent>();
	bombermanGO->AddComponent<REC::AnimationStateComponent>(std::make_unique<Game::BombermanIdleState>(bombermanGO));
	bombermanGO->AddComponent<Game::DebugBoundsRenderComponent>(REC::Color{ 255, 0, 0 });

	REC::AnimationDescriptor balloomAnimDesc{};
	balloomAnimDesc.animationDataFileKey = "characterData";
	balloomAnimDesc.animationKey = "balloom_look_left";
	balloomAnimDesc.startOnStartup = true;

	Game::PlayerDescriptor balloomDescriptor{};
	balloomDescriptor.name = Game::ObjectIds::Balloom;
	balloomDescriptor.amountOfLives = 3;
	balloomDescriptor.animDesc = balloomAnimDesc;
	balloomDescriptor.spriteDesc = charactersSpriteDescriptors;
	balloomDescriptor.renderLayer = Util::to_underlying(Game::RenderLayer::Enemies);
	balloomDescriptor.startPosition = { 330.f, 240.f };

	m_Players.push_back(std::make_unique<Game::Player>(scene, balloomDescriptor));
	auto& balloom = *m_Players.back().get();
	auto balloomGO = balloom.Get();

	balloomGO->AddCollisionComponent<REC::CollisionComponent>(collisionDescriptor); // empty collision component. needed to register to the collision system
	balloomGO->AddComponent<REC::RigidBodyComponent>();
	balloomGO->AddComponent<Game::DebugBoundsRenderComponent>(REC::Color{ 255, 0, 0 });
	
	using namespace REC::Input;
	GetContext().inputSystem->SetNumberOfActiveControllers(1);

	bomberman.CreateInputBindings(GetContext().inputSystem, GetContext().sceneManager, 100.f, playfield);
	Game::PlayerInputActions<REC::KeyboardButtonAction> bombermanInputActions{};
	bombermanInputActions.right = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_D, REC::ButtonState::Pressed);
	bombermanInputActions.left = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_A, REC::ButtonState::Pressed);
	bombermanInputActions.up = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_W, REC::ButtonState::Pressed);
	bombermanInputActions.down = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_S, REC::ButtonState::Pressed);
	bombermanInputActions.placeBomb = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_Space, REC::ButtonState::Up);
	bomberman.AddInputActions(bombermanInputActions);

	uint8_t balloomControllerId{ 0 };

	balloom.CreateInputBindings(GetContext().inputSystem, GetContext().sceneManager, 150.f, playfield);
	Game::PlayerInputActions<REC::ControllerButtonAction> balloomInputActions_btn{};
	balloomInputActions_btn.right = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Right, REC::ButtonState::Pressed, balloomControllerId);
	balloomInputActions_btn.left = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Left, REC::ButtonState::Pressed, balloomControllerId);
	balloomInputActions_btn.up = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Up, REC::ButtonState::Pressed, balloomControllerId);
	balloomInputActions_btn.down = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Down, REC::ButtonState::Pressed, balloomControllerId);
	balloomInputActions_btn.placeBomb = std::make_unique<REC::ControllerButtonAction>(Controller::Button::Gamepad_A, REC::ButtonState::Up, balloomControllerId);
	balloom.AddInputActions(balloomInputActions_btn);

	Game::PlayerInputActions<REC::ControllerRangeAction> balloomInputActions_rng{};
	balloomInputActions_rng.right = std::make_unique<REC::ControllerRangeAction>(Controller::Range::Gamepad_LeftStick_X, balloomControllerId);
	balloomInputActions_rng.up = std::make_unique<REC::ControllerRangeAction>(Controller::Range::Gamepad_LeftStick_Y, balloomControllerId);
	balloom.AddInputActions(balloomInputActions_rng);

	// ---- UI -------------------------------------------------------------------------------------------
	m_pLevelUI = std::make_unique<LevelUI>(GetContext(), &bomberman, &balloom);
}

std::optional<std::unique_ptr<REC::GameState>> Game::LevelState::OnEvent(REC::Event* event)
{
	if (event->IsEvent(REC::EventIds::LostLive))
	{
		// play death animation for the player that lost a life
		auto* args = event->GetArgs();
		auto* goArgs = dynamic_cast<REC::GameObjectEventArgs*>(args);
		goArgs->sender->GetComponent<REC::AnimationStateComponent>()->ChangeState(std::make_unique<Game::BombermanDeadState>(goArgs->sender));
		// will trigger the veryDeahtEvent after the animation is done, which will reset the players positions

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
	else if (event->IsEvent(Game::EventIds::VeryDeathEvent))
	{
		// reset both players
		bool bothPlayersDead = true;
		std::vector<int> indicesOfPlayersToRemove{};
		int index{ 0 };
		for (auto& player : m_Players)
		{
			auto* livesComp = player->GetComponents().livesComp;
			if (livesComp->HasLivesLeft())
			{
				bothPlayersDead = false;
				player->ResetPosition();
				auto* animStateComp = player->Get()->GetComponent<REC::AnimationStateComponent>();
				if (animStateComp)
					animStateComp->ChangeState(std::make_unique<Game::BombermanIdleState>(player->Get()));
				player->EnableInputBindings();
				auto* collisionComp = player->Get()->GetCollisionComponent();
				if (collisionComp)
				{
					if (!collisionComp->Enabled())
						collisionComp->ToggleEnabled(); // disable collision to prevent further deaths while the death animation is playing
				}
			}
			else
			{
				return std::make_unique<GameOverState>(GetContext()); // TODO: temp
				//player->Get()->Destroy();
				//indicesOfPlayersToRemove.push_back(index);
			}
			++index;
		}
		
		// remove the players that are out of lives from the player vector
		for (int i = static_cast<int>(indicesOfPlayersToRemove.size()) - 1; i >= 0; --i) 
		{
			m_Players.erase(m_Players.begin() + indicesOfPlayersToRemove[i]);
		}

		// if both players are death
		if (bothPlayersDead)
			return std::make_unique<GameOverState>(GetContext());
	}
	return {};
}

void Game::LevelState::Exit()
{
	for (auto& player : m_Players)
		player->RemoveInputBindings(GetContext().inputSystem);
	UnsubscribeFromEvent({ REC::EventIds::LostLive, EventIds::VeryDeathEvent });
	GetScene()->RemoveAll();
}

// ============================================================================================================================


