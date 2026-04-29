// Main.cpp file cannot contain pragma once

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include <IEngine.h>
#include <EngineSettings.h>
#include <SceneManager.h>
#include <Input/InputSystem.h>
#include <Events/EventSystem.h>
#include <Resources/IResourceManager.h>

#include <ComponentDescriptors.h>

#include <Components/TransformComponent.h>
#include <Components/AnimatedSpriteComponent.h>
#include <Components/SpriteRenderComponent.h>
#include <Components/LabeledStatComponent.h>
#include <Components/TextRenderComponent.h>
#include <Components/FPSComponent.h>
#include <Components/RotatorComponent.h>
#include <Components/HealthComponent.h>
#include <Components/LivesComponent.h>

#include <Commands/ChangeSceneCommand.h>

#include <filesystem>
#include <sdbm_hash.h>

#include "Commands/MoveCommand.h"
#include "Commands/PlaceBombCommand.h"

#include "Components/UILivesComponent.h"
#include "Components/UIScoreComponent.h"
#include "Components/GridComponent.h"
#include "Components/DebugGridRenderComponent.h"

#include "RenderLayers.h"
#include "Player.h"
#include <utility>
#include <memory>
#include <iostream>
#include "Components/BombermanCollisionComponent.h"
#include "Components/DebugBoundsRenderComponent.h"

namespace fs = std::filesystem;

static void load(REC::IEngine* engine)
{
	// === ENGINE SETTINGS =============================================================================
	REC::EngineSettings engineData{};
	engineData.frameRate = uint8_t(60);
	engineData.windowTitle = "Bomberman";
	engineData.windowWidth = uint16_t(1000);
	engineData.windowHeight = uint16_t(750);
	engine->SetEngineSettings(engineData);

	// === RESOURCES ===================================================================================
#pragma region Resources
	// !! double check all filepaths !! Json != json (will not give an error in MSCV or clang, but will cause vague JavaScript error)
	auto* RM = engine->GetContext().resourceManager;

	REC::TextureResourceCreateInfo background{};
	background.name = "background";
	background.filePath = "NES - Bomberman - Backgrounds - Playfield.png";
	if (!RM->AddResource(background))
		throw std::runtime_error("Failed to load background texture");

	REC::TextureResourceCreateInfo generalSprites{};
	generalSprites.name = "generalSprites";
	generalSprites.filePath = "NES - Bomberman - Miscellaneous - General Sprites.png";
	if (!RM->AddResource(generalSprites))
		throw std::runtime_error("Failed to load general sprites texture");

	REC::FontResourceCreateInfo font{};
	font.name = "lingua36";
	font.filePath = "Lingua.otf";
	font.size = uint8_t(36);
	if (!RM->AddResource(font))
		throw std::runtime_error("Failed to load font");

	REC::FontResourceCreateInfo debugFont{};
	debugFont.name = "dogicapixel16";
	debugFont.filePath = "dogicapixel.otf";
	debugFont.size = uint8_t(16);
	if (!RM->AddResource(debugFont))
		throw std::runtime_error("Failed to load debug font");

	REC::FontResourceCreateInfo debugFont20{};
	debugFont20.name = "dogicapixel20";
	debugFont20.filePath = "dogicapixel.otf";
	debugFont20.size = uint8_t(20);
	if (!RM->AddResource(debugFont20))
		throw std::runtime_error("Failed to load debug font 20");

	REC::FileResourceCreateInfo dataFile{};
	dataFile.name = "characterData";
	dataFile.filePath = "characterFramesData.json";
	dataFile.dataTypes = REC::LoadTypes::Frames | REC::LoadTypes::Animations;
	if (!RM->AddResource(dataFile))
		throw std::runtime_error("Failed to load character data file");

	REC::FileResourceCreateInfo titleScreenDataFile{};
	titleScreenDataFile.name = "startScreenData";
	titleScreenDataFile.filePath = "TitleScreenFramesData.json";
	titleScreenDataFile.dataTypes = REC::LoadTypes::Frames | REC::LoadTypes::TextureFont;
	if (!RM->AddResource(titleScreenDataFile))
		throw std::runtime_error("Failed to load title screen data file");

	REC::TextureResourceCreateInfo titleScreen{};
	titleScreen.name = "titleScreen";
	titleScreen.filePath = "NES - Bomberman - Miscellaneous - Title Screen & Text.png";
	if (!RM->AddResource(titleScreen))
		throw std::runtime_error("Failed to load title screen texture");
#pragma endregion Resources

	// === SCENE =======================================================================================
	auto* SM = engine->GetContext().sceneManager;

#pragma region StartScreen
	auto* startScreen = SM->CreateScene();
	
	REC::SpriteDescriptor startScreenBackdrop{};
	startScreenBackdrop.drawHeight = 750;
	startScreenBackdrop.textureKey = "titleScreen";
	startScreenBackdrop.frameDataFileKey = "startScreenData";
	startScreenBackdrop.frameKey = "start_up_screen_1987";

	REC::GameObjectDescriptor startScreenBackdropDesc{};
	startScreenBackdropDesc.startPosX = 125.f;
	startScreenBackdropDesc.startPosY = 0.f;
	startScreenBackdropDesc.renderLayer = std::to_underlying(Game::RenderLayer::Background); // C++23 feature
	
	auto* stsc = startScreen->CreateGameObject(startScreenBackdropDesc);
	stsc->AddComponent<REC::SpriteRenderComponent>(startScreenBackdrop);

	REC::TextDescriptor startScreenTextInstructionDesc{};
	startScreenTextInstructionDesc.color = REC::Color{ 0,255,255 };
	startScreenTextInstructionDesc.fontKey = "dogicapixel20";
	startScreenTextInstructionDesc.text = "Press '5' to start";

	REC::GameObjectDescriptor startScreenInstructionDesc{};
	startScreenInstructionDesc.startPosX = 75.f;
	startScreenInstructionDesc.startPosY = 320.f;
	startScreenInstructionDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui); // C++23 feature

	auto* stscInstruction = startScreen->CreateGameObject(startScreenInstructionDesc);
	stscInstruction->AddComponent<REC::TextRenderComponent>(startScreenTextInstructionDesc);
#pragma endregion StartScreen

	auto* scene = SM->CreateScene();

	Game::GridDescriptor grid{};
	grid.cellHeight = uint8_t(45); //51
	grid.cellWidth = uint8_t(45);  //51
	grid.rows = uint8_t(13);
	grid.cols = uint8_t(31);

	REC::SpriteDescriptor backdrop{};
	backdrop.drawHeight = uint16_t(grid.cellHeight)*uint16_t(grid.rows);
	backdrop.frameDataFileKey = "characterData";
	backdrop.frameKey = "background";
	backdrop.textureKey = "background";

	REC::GameObjectDescriptor backdropObjectDesc{};
	backdropObjectDesc.startPosX = 0.f;
	backdropObjectDesc.startPosY = 80.f;
	backdropObjectDesc.renderLayer = std::to_underlying(Game::RenderLayer::Background); // C++23 feature
	//backdropObjectDesc.bounds = REC::Rect{ 0.f, 0.f, float(grid.cellWidth) * float(grid.cols), float(grid.cellHeight) * float(grid.rows) };

	auto* go = scene->CreateGameObject(backdropObjectDesc);
	go->AddComponent<REC::SpriteRenderComponent>(backdrop);
	auto* playfield = go->AddComponent<Game::GridComponent>(grid);
	go->AddComponent<Game::DebugGridRenderComponent>(REC::Color{20, 30, 120, 200});

	REC::GameObjectDescriptor instructionObjectDesc{};
	instructionObjectDesc.startPosX = 20.f;
	instructionObjectDesc.startPosY = 20.f;
	instructionObjectDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	auto* instructions = scene->CreateGameObject(instructionObjectDesc);
	
	REC::GameObjectDescriptor instrBalloomDesc{};
	instrBalloomDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	instrBalloomDesc.parent = instructions;
	auto* instructionsBalloom = scene->CreateGameObject(instrBalloomDesc);
	instructionsBalloom->AddComponent<REC::TextRenderComponent>("Use the D-Pad or left Thumb Stick to move Balloom", "dogicapixel16");
	
	REC::GameObjectDescriptor instrBombermanDesc{};
	instrBombermanDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	instrBombermanDesc.parent = instructions;
	instrBombermanDesc.startPosY = 28.f;
	auto* instructionsBomberman = scene->CreateGameObject(instrBombermanDesc);
	instructionsBomberman->AddComponent<REC::TextRenderComponent>("Use WASD to move Bomberman", "dogicapixel16");

	REC::GameObjectDescriptor instrBalloomBombDesc{};
	instrBalloomBombDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	instrBalloomBombDesc.parent = instructions;
	instrBalloomBombDesc.startPosX = 30.f;
	instrBalloomBombDesc.startPosY = 110.f;
	auto intstructionsBalloomBomb = scene->CreateGameObject(instrBalloomBombDesc);
	intstructionsBalloomBomb->AddComponent<REC::TextRenderComponent>("BALLOOM: Press A to place bomb, place 2 bombs and kill bomberman", "dogicapixel16");

	REC::GameObjectDescriptor instrBombermanBombDesc{};
	instrBombermanBombDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	instrBombermanBombDesc.parent = instructions;
	instrBombermanBombDesc.startPosX = 30.f;
	instrBombermanBombDesc.startPosY = 130.f;
	auto intstructionsbombermanBomb = scene->CreateGameObject(instrBombermanBombDesc);
	intstructionsbombermanBomb->AddComponent<REC::TextRenderComponent>("BOMBERMAN: Press 'space' to place bomb, place 2 bombs and kill balloom", "dogicapixel16");

	REC::GameObjectDescriptor UIdesc{};
	UIdesc.startPosX = 20.f;
	UIdesc.startPosY = 680.f;
	UIdesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	auto UI = scene->CreateGameObject(UIdesc);

	REC::LabeledStatDescriptor livesStatDesciptor{};
	livesStatDesciptor.fontkey = "dogicapixel16";
	livesStatDesciptor.label = "   Remaining Lives";
	livesStatDesciptor.initialValue = 3;
	livesStatDesciptor.color = REC::Color{ 255,255,255 };

	REC::SpriteDescriptor bombermanIcon{};
	bombermanIcon.drawHeight = 20;
	bombermanIcon.textureKey = "generalSprites";
	bombermanIcon.frameDataFileKey = "characterData";
	bombermanIcon.frameKey = "bomberman_walk_front_0";

	REC::GameObjectDescriptor bombermanUILivesDesc{};
	bombermanUILivesDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	bombermanUILivesDesc.parent = UI;
	auto bombermanUILives = scene->CreateGameObject(bombermanUILivesDesc);
	auto bomberman_livesStatComp = bombermanUILives->AddComponent<Game::UILivesComponent>(livesStatDesciptor);
	bombermanUILives->AddComponent<REC::SpriteRenderComponent>(bombermanIcon);

	REC::SpriteDescriptor balloomIcon{};
	balloomIcon.drawHeight = 20;
	balloomIcon.textureKey = "generalSprites";
	balloomIcon.frameDataFileKey = "characterData";
	balloomIcon.frameKey = "balloom_look_right_0";

	REC::GameObjectDescriptor balloomUILivesDesc{};
	balloomUILivesDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	balloomUILivesDesc.parent = UI;
	balloomUILivesDesc.startPosX = 0.f;
	balloomUILivesDesc.startPosY = 30.f;
	auto balloomUILives = scene->CreateGameObject(balloomUILivesDesc);
	auto balloom_livesStatComp = balloomUILives->AddComponent<Game::UILivesComponent>(livesStatDesciptor);
	balloomUILives->AddComponent<REC::SpriteRenderComponent>(balloomIcon);

	REC::LabeledStatDescriptor scoreStatDesciptor{};
	scoreStatDesciptor.fontkey = "dogicapixel16";
	scoreStatDesciptor.label = "Score";
	scoreStatDesciptor.initialValue = 0;
	scoreStatDesciptor.color = REC::Color{ 255,255,255 };

	REC::GameObjectDescriptor bombermanUIScoreDesc{};
	bombermanUIScoreDesc.startPosX = 350.f;
	bombermanUIScoreDesc.startPosY = 0.f;
	bombermanUIScoreDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	bombermanUIScoreDesc.parent = UI;
	auto bombermanUIScore = scene->CreateGameObject(bombermanUIScoreDesc);
	auto bomberman_scoreStatComp = bombermanUIScore->AddComponent<Game::UIScoreComponent>(scoreStatDesciptor);

	REC::GameObjectDescriptor balloomUIScoreDesc{};
	balloomUIScoreDesc.startPosX = 350.f;
	balloomUIScoreDesc.startPosY = 30.f;
	balloomUIScoreDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	balloomUIScoreDesc.parent = UI;
	auto balloomUIScore = scene->CreateGameObject(balloomUIScoreDesc);
	auto balloom_scoreStatComp = balloomUIScore->AddComponent<Game::UIScoreComponent>(scoreStatDesciptor);

	REC::GameObjectDescriptor fpsDesc{};
	fpsDesc.startPosX = 880.f;
	fpsDesc.startPosY = 20.f;
	fpsDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	auto fps = scene->CreateGameObject(fpsDesc); 
	fps->AddComponent<REC::FPSComponent>("dogicapixel20");

	// Create a player class in game that does this instead, to avoid repetition
	REC::SpriteDescriptor charactersSpriteDescriptors{};
	charactersSpriteDescriptors.drawHeight = 50;
	charactersSpriteDescriptors.frameDataFileKey = "characterData";
	charactersSpriteDescriptors.textureKey = "generalSprites";
	charactersSpriteDescriptors.hasColorTransparency = true;
	charactersSpriteDescriptors.transparentColor = REC::Color{ 56,135,0 };
	charactersSpriteDescriptors.drawPointX = 0.5f;
	charactersSpriteDescriptors.drawPointY = 0.5f;

	REC::AnimationDescriptor bombermanWalkAnimDesc{};
	bombermanWalkAnimDesc.animationDataFileKey = "characterData";
	bombermanWalkAnimDesc.animationKey = "bomberman_walk_left";
	bombermanWalkAnimDesc.startOnStartup = true;

	Game::PlayerDescriptor bombermanDescriptor{};
	bombermanDescriptor.name = REC::make_sdbm_hash("Bomberman");
	bombermanDescriptor.amountOfLives = 3;
	bombermanDescriptor.animDesc = bombermanWalkAnimDesc;
	bombermanDescriptor.spriteDesc = charactersSpriteDescriptors;
	bombermanDescriptor.maxHealth = 100.f;
	bombermanDescriptor.renderLayer = std::to_underlying(Game::RenderLayer::Player);
	bombermanDescriptor.startPosition = { 230.f, 230.f };

	Game::Player bomberman{ scene, bombermanDescriptor };
	bomberman_livesStatComp->SetConnectedPlayer(bomberman.Get());
	bomberman_scoreStatComp->SetConnectedPlayer(bomberman.Get());

	REC::CollisionDescriptor collisionDescriptor{};
	collisionDescriptor.collisionType = REC::CollisionType::Dynamic;
	collisionDescriptor.bounds.emplace_back(REC::Rect{ -20.f, -20.f, 40.f, 40.f }); // centered on the player

	bomberman.Get()->AddCollisionComponent<Game::BombermanCollisionComponent>(collisionDescriptor);
	bomberman.Get()->AddComponent<Game::DebugBoundsRenderComponent>(REC::Color{ 255, 0, 0 });

	uint8_t balloomControllerId{ 0 };

	REC::AnimationDescriptor balloomAnimDesc{};
	balloomAnimDesc.animationDataFileKey = "characterData";
	balloomAnimDesc.animationKey = "balloom_look_left";
	balloomAnimDesc.startOnStartup = true;

	Game::PlayerDescriptor balloomDescriptor{};
	balloomDescriptor.name = REC::make_sdbm_hash("Balloom");
	balloomDescriptor.amountOfLives = 3;
	balloomDescriptor.animDesc = balloomAnimDesc;
	balloomDescriptor.spriteDesc = charactersSpriteDescriptors;
	balloomDescriptor.maxHealth = 100.f;
	balloomDescriptor.renderLayer = std::to_underlying(Game::RenderLayer::Enemies);
	balloomDescriptor.startPosition = { 350.f, 250.f };

	Game::Player balloom{ scene, balloomDescriptor };
	balloom_livesStatComp->SetConnectedPlayer(balloom.Get());
	balloom_scoreStatComp->SetConnectedPlayer(balloom.Get());

	balloom.Get()->AddCollisionComponent<REC::CollisionComponent>(collisionDescriptor); // empty collision component. needed to register to the collision system
	balloom.Get()->AddComponent<Game::DebugBoundsRenderComponent>(REC::Color{ 255, 0, 0 });

	// === INPUT =======================================================================================
	auto* input = engine->GetContext().inputSystem;
	input->SetNumberOfActiveControllers(1);

	using namespace REC::Input;

	bomberman.CreateInputBindings(input, SM, 100.f, playfield);
	Game::PlayerInputActions<REC::KeyboardButtonAction> bombermanInputActions{};
	bombermanInputActions.right		= std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_D,		REC::ButtonState::Pressed);
	bombermanInputActions.left		= std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_A,		REC::ButtonState::Pressed);
	bombermanInputActions.up		= std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_W,		REC::ButtonState::Pressed);
	bombermanInputActions.down		= std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_S,		REC::ButtonState::Pressed);
	bombermanInputActions.placeBomb = std::make_unique<REC::KeyboardButtonAction>(Keyboard::Button::Keyboard_Space, REC::ButtonState::Up);
	bomberman.AddInputActions(bombermanInputActions);

	balloom.CreateInputBindings(input, SM, 150.f, playfield);
	Game::PlayerInputActions<REC::ControllerButtonAction> balloomInputActions_btn{};
	balloomInputActions_btn.right		= std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Right, REC::ButtonState::Pressed,  balloomControllerId);
	balloomInputActions_btn.left		= std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Left,	REC::ButtonState::Pressed,  balloomControllerId);
	balloomInputActions_btn.up			= std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Up,	REC::ButtonState::Pressed,  balloomControllerId);
	balloomInputActions_btn.down		= std::make_unique<REC::ControllerButtonAction>(Controller::Button::GamePad_DPad_Down,	REC::ButtonState::Pressed,  balloomControllerId);
	balloomInputActions_btn.placeBomb	= std::make_unique<REC::ControllerButtonAction>(Controller::Button::Gamepad_A,			REC::ButtonState::Up,		balloomControllerId);
	balloom.AddInputActions(balloomInputActions_btn);

	Game::PlayerInputActions<REC::ControllerRangeAction> balloomInputActions_rng{};
	balloomInputActions_rng.right = std::make_unique<REC::ControllerRangeAction>(Controller::Range::Gamepad_LeftStick_X, balloomControllerId);
	balloomInputActions_rng.up    = std::make_unique<REC::ControllerRangeAction>(Controller::Range::Gamepad_LeftStick_Y, balloomControllerId);
	balloom.AddInputActions(balloomInputActions_rng);

	auto* changeScene = input->CreateInputBinding();
	changeScene->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_5, REC::ButtonState::Up);
	changeScene->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keypad_5, REC::ButtonState::Up);
	changeScene->AddCommand<REC::ChangeSceneCommand>(engine->GetContext(), scene, startScreen);
}

int main(int, char*[]) 
{
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif

	// TODO: smart pointer (lots of memory errors with smart pointer)
	REC::IEngine* engine = nullptr;
	try
	{
		engine = REC::CreateEngine(data_location);
		engine->Run(load);
	}
	catch (const std::exception& e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
	}
	REC::DestroyEngine(engine);

    return 0;
}

/*
NOTES:
-- Collision Theories
Needed Data: for each object: position, bounds, static?
Detection: When an object moves, check - once per frame - if it collides with any other object. a central Collision system class?
Response: Messenger System? each object decides what happens on collision

AABB Collision: check if the bounding boxes of 2 objects overlap. only works for rectangles.

=> TODO: Collision System class. each GameObject has an input struct where the user can say if it has collision and if yes, static. (static is default true)
=> TODO: Collision System creates and sends events. (OnEntry (once), OnExit (once), Overlap (persitent) )
*/