#include "LevelSinglePlayer.h"
#include "../../Ids.h"
#include "../../RenderLayers.h"
#include "../../Components/GridComponent.h"
#include "../../Player.h"
#include "../../LevelUI.h"
#include <Input/InputBinding.h>
#include <Input/InputSystem.h>
#include "../../Components/BombermanCollisionComponent.h"
#include "../BombermanStates.h"
#include <Components/AnimationStateComponent.h>
#include <Components/RigidBodyComponent.h>
#include <LevelInfo.h>

#ifdef _DEBUG
#include "../../Components/DebugBoundsRenderComponent.h"
#endif // _DEBUG


Game::LevelSinglePlayer::LevelSinglePlayer(const REC::EngineContext& context)
	: LevelState(context)
{}

void Game::LevelSinglePlayer::Enter()
{
	LevelState::Enter();

	SetPlayerMode(PlayerMode::SinglePlayer);

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

	auto* playfield = GetPlayfield()->GetComponent<GridComponent>();
	auto startCell = playfield->GetLevelInfo()->player1StartCell;
	auto starPosition = playfield->GetAbsoluteCellPosition(startCell.first, startCell.second);
	float offset = playfield->GetCellSize().x / 2.f; // to center the player in the cell
	starPosition += glm::vec2{ offset, offset };

	Game::PlayerDescriptor bombermanDescriptor{};
	bombermanDescriptor.name = Game::ObjectIds::Bomberman;
	bombermanDescriptor.amountOfLives = 4;
	bombermanDescriptor.animDesc = bombermanWalkAnimDesc;
	bombermanDescriptor.spriteDesc = charactersSpriteDescriptors;
	bombermanDescriptor.renderLayer = Util::to_underlying(Game::RenderLayer::Player);
	bombermanDescriptor.startPosition = starPosition;

	auto* player = AddPlayer(bombermanDescriptor);
	auto* playerGO = player->Get();

	REC::CollisionDescriptor collisionDescriptor{};
	collisionDescriptor.collisionType = REC::CollisionType::Dynamic;
	collisionDescriptor.bounds.emplace_back(REC::CollisionBound{ REC::Rect{ -18.f, -18.f, 36.f, 36.f}, true }); // centered on the player

	playerGO->AddCollisionComponent<BombermanCollisionComponent>(collisionDescriptor);
	playerGO->AddComponent<REC::RigidBodyComponent>();
	playerGO->AddComponent<REC::AnimationStateComponent>(std::make_unique<BombermanIdleState>(playerGO));
#ifdef _DEBUG
	playerGO->AddComponent<DebugBoundsRenderComponent>(REC::Color{ 255, 0, 0 });
#endif // _DEBUG

	auto* gridComp = GetPlayfield()->GetComponent<GridComponent>();

	using namespace REC::Input;
	player->CreateInputBindings(GetContext().inputSystem, GetContext().sceneManager, 100.f, gridComp);
	Game::PlayerInputActions<REC::KeyboardButtonAction> keyboardInputActions{};
	keyboardInputActions.right = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_D, REC::ButtonState::Pressed);
	keyboardInputActions.left = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_A, REC::ButtonState::Pressed);
	keyboardInputActions.up = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_W, REC::ButtonState::Pressed);
	keyboardInputActions.down = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_S, REC::ButtonState::Pressed);
	keyboardInputActions.placeBomb = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_Space, REC::ButtonState::Up);
	player->AddInputActions(keyboardInputActions);

	uint8_t controllerId{ 0 };
	Game::PlayerInputActions<REC::ControllerButtonAction> controllerInputActions_btn{};
	controllerInputActions_btn.right = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Right, REC::ButtonState::Pressed, controllerId);
	controllerInputActions_btn.left = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Left, REC::ButtonState::Pressed, controllerId);
	controllerInputActions_btn.up = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Up, REC::ButtonState::Pressed, controllerId);
	controllerInputActions_btn.down = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Down, REC::ButtonState::Pressed, controllerId);
	controllerInputActions_btn.placeBomb = std::make_unique<REC::ControllerButtonAction>(Controller::Button::Gamepad_A, REC::ButtonState::Up, controllerId);
	player->AddInputActions(controllerInputActions_btn);

	Game::PlayerInputActions<REC::ControllerRangeAction> controllerInputActions_rng{};
	controllerInputActions_rng.right = std::make_unique<REC::ControllerRangeAction>(Controller::Range::Gamepad_LeftStick_X, controllerId);
	controllerInputActions_rng.up = std::make_unique<REC::ControllerRangeAction>(Controller::Range::Gamepad_LeftStick_Y, controllerId);
	player->AddInputActions(controllerInputActions_rng);

	GetContext().inputSystem->SetNumberOfActiveControllers(1);
	CreateLevelUI(player, nullptr);
}

void Game::LevelSinglePlayer::Exit()
{
	LevelState::Exit();
}
