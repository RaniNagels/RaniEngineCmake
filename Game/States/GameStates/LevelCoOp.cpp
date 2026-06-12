#include "LevelCoOp.h"
#include "../../Ids.h"
#include "../../RenderLayers.h"
#include "../../Components/GridComponent.h"
#include "../../Components/BombermanComponent.h"
#include "../../Player.h"
#include "../../LevelUI.h"
#include "../../Components/BombermanCollisionComponent.h"
#include <Components/RigidBodyComponent.h>
#include <Components/AnimationStateComponent.h>
#include "../BombermanStates.h"
#include <Input/InputBinding.h>
#include <Input/InputSystem.h>

#ifdef _DEBUG
#include "../../Components/DebugBoundsRenderComponent.h"
#endif // _DEBUG


Game::LevelCoOp::LevelCoOp(const REC::EngineContext& context)
	: LevelState(context)
{}

void Game::LevelCoOp::Enter()
{
	LevelState::Enter();
	SetPlayerMode(PlayerMode::CoOp);

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
	auto startCell2 = playfield->GetLevelInfo()->player2StartCell;
	auto starPosition = playfield->GetAbsoluteCellPosition(startCell.first, startCell.second);
	auto startPosition2 = playfield->GetAbsoluteCellPosition(startCell2.first, startCell2.second);
	float offset = playfield->GetCellSize().x / 2.f; // to center the player in the cell
	starPosition += glm::vec2{ offset, offset };
	startPosition2 += glm::vec2{ offset, offset };

	Game::PlayerDescriptor bombermanDescriptor{};
	bombermanDescriptor.name = Game::ObjectIds::Bomberman;
	bombermanDescriptor.amountOfLives = 4;
	bombermanDescriptor.animDesc = bombermanWalkAnimDesc;
	bombermanDescriptor.spriteDesc = charactersSpriteDescriptors;
	bombermanDescriptor.renderLayer = Util::to_underlying(Game::RenderLayer::Player);
	bombermanDescriptor.startPosition = starPosition;

	auto* player1 = AddPlayer(bombermanDescriptor);
	auto* player1GO = player1->Get();

	bombermanDescriptor.startPosition = startPosition2;
	auto* player2 = AddPlayer(bombermanDescriptor);
	auto* player2GO = player2->Get();

	REC::CollisionDescriptor collisionDescriptor{};
	collisionDescriptor.collisionType = REC::CollisionType::Dynamic;
	collisionDescriptor.bounds.emplace_back(REC::CollisionBound{ REC::Rect{ -18.f, -18.f, 36.f, 36.f}, true });

	player1GO->AddCollisionComponent<BombermanCollisionComponent>(collisionDescriptor);
	player2GO->AddCollisionComponent<BombermanCollisionComponent>(collisionDescriptor);

	player1GO->AddComponent<BombermanComponent>();
	player2GO->AddComponent<BombermanComponent>();

	player1GO->AddComponent<REC::RigidBodyComponent>();
	player2GO->AddComponent<REC::RigidBodyComponent>();

	player1GO->AddComponent<REC::AnimationStateComponent>(std::make_unique<BombermanIdleState>(player1GO));
	player2GO->AddComponent<REC::AnimationStateComponent>(std::make_unique<BombermanIdleState>(player2GO));

#ifdef _DEBUG
	player1GO->AddComponent<DebugBoundsRenderComponent>(REC::Color{ 255, 0, 0 });
	player2GO->AddComponent<DebugBoundsRenderComponent>(REC::Color{ 0, 255, 0 });
#endif // _DEBUG

	auto* gridComp = GetPlayfield()->GetComponent<GridComponent>();

	using namespace REC::Input;
	player1->CreateInputBindings(GetContext().inputSystem, GetContext().sceneManager, 100.f, gridComp);
	Game::PlayerInputActions<REC::KeyboardButtonAction> keyboardInputActions1{};
	keyboardInputActions1.right = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_D, REC::ButtonState::Pressed);
	keyboardInputActions1.left = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_A, REC::ButtonState::Pressed);
	keyboardInputActions1.up = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_W, REC::ButtonState::Pressed);
	keyboardInputActions1.down = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_S, REC::ButtonState::Pressed);
	keyboardInputActions1.placeBomb = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_Space, REC::ButtonState::Up);
	player1->AddInputActions(keyboardInputActions1);

	uint8_t player1ControllerId{ 0 };
	Game::PlayerInputActions<REC::ControllerButtonAction> controllerInputActions_btn1{};
	controllerInputActions_btn1.right = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Right, REC::ButtonState::Pressed, player1ControllerId);
	controllerInputActions_btn1.left = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Left, REC::ButtonState::Pressed, player1ControllerId);
	controllerInputActions_btn1.up = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Up, REC::ButtonState::Pressed, player1ControllerId);
	controllerInputActions_btn1.down = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Down, REC::ButtonState::Pressed, player1ControllerId);
	controllerInputActions_btn1.placeBomb = std::make_unique<REC::ControllerButtonAction>(Controller::Button::Gamepad_A, REC::ButtonState::Up, player1ControllerId);
	player1->AddInputActions(controllerInputActions_btn1);

	Game::PlayerInputActions<REC::ControllerRangeAction> controllerInputActions_rng1{};
	controllerInputActions_rng1.right = std::make_unique<REC::ControllerRangeAction>(Controller::Range::Gamepad_LeftStick_X, player1ControllerId);
	controllerInputActions_rng1.up = std::make_unique<REC::ControllerRangeAction>(Controller::Range::Gamepad_LeftStick_Y, player1ControllerId);
	player1->AddInputActions(controllerInputActions_rng1);

	player2->CreateInputBindings(GetContext().inputSystem, GetContext().sceneManager, 100.f, gridComp);
	Game::PlayerInputActions<REC::KeyboardButtonAction> keyboardInputActions{};
	keyboardInputActions.right = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_Right, REC::ButtonState::Pressed);
	keyboardInputActions.left = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_Left, REC::ButtonState::Pressed);
	keyboardInputActions.up = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_Up, REC::ButtonState::Pressed);
	keyboardInputActions.down = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_Down, REC::ButtonState::Pressed);
	keyboardInputActions.placeBomb = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_Return, REC::ButtonState::Up);
	player2->AddInputActions(keyboardInputActions);

	uint8_t player2ControllerId{ 0 };
	Game::PlayerInputActions<REC::ControllerButtonAction> controllerInputActions_btn{};
	controllerInputActions_btn.right = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Right, REC::ButtonState::Pressed, player2ControllerId);
	controllerInputActions_btn.left = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Left, REC::ButtonState::Pressed, player2ControllerId);
	controllerInputActions_btn.up = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Up, REC::ButtonState::Pressed, player2ControllerId);
	controllerInputActions_btn.down = std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Down, REC::ButtonState::Pressed, player2ControllerId);
	controllerInputActions_btn.placeBomb = std::make_unique<REC::ControllerButtonAction>(Controller::Button::Gamepad_A, REC::ButtonState::Up, player2ControllerId);
	player2->AddInputActions(controllerInputActions_btn);

	Game::PlayerInputActions<REC::ControllerRangeAction> controllerInputActions_rng{};
	controllerInputActions_rng.right = std::make_unique<REC::ControllerRangeAction>(Controller::Range::Gamepad_LeftStick_X, player2ControllerId);
	controllerInputActions_rng.up = std::make_unique<REC::ControllerRangeAction>(Controller::Range::Gamepad_LeftStick_Y, player2ControllerId);
	player2->AddInputActions(controllerInputActions_rng);

	GetContext().inputSystem->SetNumberOfActiveControllers(2);
	CreateLevelUI(player1, player2);
}

void Game::LevelCoOp::Exit()
{
	LevelState::Exit();
}