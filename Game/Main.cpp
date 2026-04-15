// Main.cpp file cannot contain pragma once

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include <Engine.h>
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

namespace fs = std::filesystem;

static void load(REC::Engine* engine)
{
	// === ENGINE SETTINGS =============================================================================
	REC::EngineSettings engineData{};
	engineData.frameRate = uint8_t(60);
	engineData.windowTitle = "Bomberman";
	engineData.windowWidth = uint16_t(1000);
	engineData.windowHeight = uint16_t(750);
	engine->SetEngineData(engineData);

	// === RESOURCES ===================================================================================
#pragma region Resources
	// !! double check all filepaths !! Json != json (will not give an error in MSCV or clang, but will cause vague JavaScript error)
	auto* RM = engine->GetResourceManager();

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
	auto* SM = engine->GetSceneManager();

#pragma region StartScreen
	auto* startScreen = SM->CreateScene();
	
	REC::SpriteDescriptor startScreenBackdrop{};
	startScreenBackdrop.drawHeight = 750;
	startScreenBackdrop.textureKey = "titleScreen";
	startScreenBackdrop.frameDataFileKey = "startScreenData";
	startScreenBackdrop.frameKey = "start_up_screen_1987";
	
	auto* stsc = startScreen->CreateGameObject(125,0);
	stsc->AddComponent<REC::SpriteRenderComponent>(startScreenBackdrop);
	startScreen->SetRenderLayer(stsc, Game::GetLayer(Game::RenderLayer::Background));

	auto* stscInstruction = startScreen->CreateGameObject(75, 320);
	stscInstruction->AddComponent<REC::TextRenderComponent>("Press '5' to start", "dogicapixel20", REC::Color{0,255,255});
	startScreen->SetRenderLayer(stscInstruction, Game::GetLayer(Game::RenderLayer::Ui));
#pragma endregion StartScreen

	auto* scene = SM->CreateScene();
	auto* ES = engine->GetEventSystem();

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
	backdropObjectDesc.startPosX = 0;
	backdropObjectDesc.startPosY = 80;
	backdropObjectDesc.renderLayer = std::to_underlying(Game::RenderLayer::Background); // C++23 feature
	backdropObjectDesc.bounds = REC::Rect{ 0.f, 0.f, float(grid.cellWidth) * float(grid.cols), float(grid.cellHeight) * float(grid.rows) };

	auto* go = scene->CreateGameObject(backdropObjectDesc);
	go->AddComponent<REC::SpriteRenderComponent>(backdrop);
	auto* playfield = go->AddComponent<Game::GridComponent>(grid);
	go->AddComponent<Game::DebugGridRenderComponent>(engine->GetRenderer(), REC::Color{ uint8_t(20),uint8_t(30),uint8_t(120)});

	REC::GameObjectDescriptor instructionObjectDesc{};
	instructionObjectDesc.startPosX = 20;
	instructionObjectDesc.startPosY = 20;
	instructionObjectDesc.renderLayer = std::to_underlying(Game::RenderLayer::Ui);
	auto* instructions = scene->CreateGameObject(instructionObjectDesc);
	
	auto* instructionsBalloom = scene->CreateGameObject();
	instructionsBalloom->AddComponent<REC::TextRenderComponent>("Use the D-Pad or left Thumb Stick to move Balloom", "dogicapixel16");
	instructionsBalloom->SetParent(instructions);
	scene->SetRenderLayer(instructionsBalloom, Game::GetLayer(Game::RenderLayer::Ui));
	
	auto* instructionsBomberman = scene->CreateGameObject(0.f, 28.f);
	instructionsBomberman->AddComponent<REC::TextRenderComponent>("Use WASD to move Bomberman", "dogicapixel16");
	instructionsBomberman->SetParent(instructions);
	scene->SetRenderLayer(instructionsBomberman, Game::GetLayer(Game::RenderLayer::Ui));

	auto intstructionsBalloomBomb = scene->CreateGameObject(30.f, 110.f);
	intstructionsBalloomBomb->AddComponent<REC::TextRenderComponent>("BALLOOM: Press A to place bomb, place 2 bombs and kill bomberman", "dogicapixel16");
	intstructionsBalloomBomb->SetParent(instructions);
	scene->SetRenderLayer(intstructionsBalloomBomb, Game::GetLayer(Game::RenderLayer::Ui));

	auto intstructionsbombermanBomb = scene->CreateGameObject(30.f, 130.f);
	intstructionsbombermanBomb->AddComponent<REC::TextRenderComponent>("BOMBERMAN: Press 'space' to place bomb, place 2 bombs and kill balloom", "dogicapixel16");
	intstructionsbombermanBomb->SetParent(instructions);
	scene->SetRenderLayer(intstructionsbombermanBomb, Game::GetLayer(Game::RenderLayer::Ui));

	auto UI = scene->CreateGameObject(20.f, 680.f);
	scene->SetRenderLayer(UI, Game::GetLayer(Game::RenderLayer::Ui));

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

	auto bombermanUILives = scene->CreateGameObject();
	auto bomberman_livesStatComp = bombermanUILives->AddComponent<Game::UILivesComponent>(livesStatDesciptor);
	ES->Subscribe(bomberman_livesStatComp, { REC::make_sdbm_hash("LostLiveEvent") });
	bombermanUILives->AddComponent<REC::SpriteRenderComponent>(bombermanIcon);
	bombermanUILives->SetParent(UI);
	scene->SetRenderLayer(bombermanUILives, Game::GetLayer(Game::RenderLayer::Ui));

	REC::SpriteDescriptor balloomIcon{};
	balloomIcon.drawHeight = 20;
	balloomIcon.textureKey = "generalSprites";
	balloomIcon.frameDataFileKey = "characterData";
	balloomIcon.frameKey = "balloom_look_right_0";

	auto balloomUILives = scene->CreateGameObject(0.f, 30.f);
	auto balloom_livesStatComp = balloomUILives->AddComponent<Game::UILivesComponent>(livesStatDesciptor);
	ES->Subscribe(balloom_livesStatComp, { REC::make_sdbm_hash("LostLiveEvent") });
	balloomUILives->AddComponent<REC::SpriteRenderComponent>(balloomIcon);
	balloomUILives->SetParent(UI);
	scene->SetRenderLayer(balloomUILives, Game::GetLayer(Game::RenderLayer::Ui));

	REC::LabeledStatDescriptor scoreStatDesciptor{};
	scoreStatDesciptor.fontkey = "dogicapixel16";
	scoreStatDesciptor.label = "Score";
	scoreStatDesciptor.initialValue = 0;
	scoreStatDesciptor.color = REC::Color{ 255,255,255 };

	auto bombermanUIScore = scene->CreateGameObject(350.f, 0);
	auto bomberman_scoreStatComp = bombermanUIScore->AddComponent<Game::UIScoreComponent>(scoreStatDesciptor);
	bombermanUIScore->SetParent(UI);
	scene->SetRenderLayer(bombermanUIScore, Game::GetLayer(Game::RenderLayer::Ui));

	auto balloomUIScore = scene->CreateGameObject(350.f, 30.f);
	auto balloom_scoreStatComp = balloomUIScore->AddComponent<Game::UIScoreComponent>(scoreStatDesciptor);
	balloomUIScore->SetParent(UI);
	scene->SetRenderLayer(balloomUIScore, Game::GetLayer(Game::RenderLayer::Ui));

	auto fps = scene->CreateGameObject(880.f, 20.f); 
	fps->AddComponent<REC::FPSComponent>("dogicapixel20");
	scene->SetRenderLayer(fps, Game::GetLayer(Game::RenderLayer::Ui));

	// Create a player class in game that does this instead, to avoid repetition
	REC::SpriteDescriptor charactersSpriteDescriptors{};
	charactersSpriteDescriptors.drawHeight = 50;
	charactersSpriteDescriptors.frameDataFileKey = "characterData";
	charactersSpriteDescriptors.textureKey = "generalSprites";

	REC::AnimationDescriptor bombermanWalkAnimDesc{};
	bombermanWalkAnimDesc.animationDataFileKey = "characterData";
	bombermanWalkAnimDesc.animationKey = "bomberman_walk_left";
	bombermanWalkAnimDesc.startOnStartup = true;

	Game::PlayerDescriptor bombermanDescriptor{};
	bombermanDescriptor.amountOfLives = 3;
	bombermanDescriptor.animDesc = bombermanWalkAnimDesc;
	bombermanDescriptor.spriteDesc = charactersSpriteDescriptors;
	bombermanDescriptor.maxHealth = 100.f;
	bombermanDescriptor.renderLayer = Game::GetLayer(Game::RenderLayer::Player);
	bombermanDescriptor.startPosition = { 230.f, 230.f };

	Game::Player bomberman{ scene, ES, bombermanDescriptor };
	bomberman_livesStatComp->SetConnectedPlayer(bomberman.Get());
	bomberman_scoreStatComp->SetConnectedPlayer(bomberman.Get());
	ES->Subscribe(bomberman.GetComponents().livesComp, { REC::make_sdbm_hash("HasZeroHealthEvent") });

	uint8_t balloomControllerId{ 0 };

	REC::AnimationDescriptor balloomAnimDesc{};
	balloomAnimDesc.animationDataFileKey = "characterData";
	balloomAnimDesc.animationKey = "balloom_look_left";
	balloomAnimDesc.startOnStartup = true;

	Game::PlayerDescriptor balloomDescriptor{};
	balloomDescriptor.amountOfLives = 3;
	balloomDescriptor.animDesc = balloomAnimDesc;
	balloomDescriptor.spriteDesc = charactersSpriteDescriptors;
	balloomDescriptor.maxHealth = 100.f;
	balloomDescriptor.renderLayer = Game::GetLayer(Game::RenderLayer::Enemies);
	balloomDescriptor.startPosition = { 350.f, 250.f };

	Game::Player balloom{ scene, ES, balloomDescriptor };
	balloom_livesStatComp->SetConnectedPlayer(balloom.Get());
	balloom_scoreStatComp->SetConnectedPlayer(balloom.Get());
	ES->Subscribe(balloom.GetComponents().livesComp, { REC::make_sdbm_hash("HasZeroHealthEvent") });
	

	// === INPUT =======================================================================================
	auto* input = engine->GetInputSystem();
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

	ES->Subscribe(bomberman_scoreStatComp, { REC::make_sdbm_hash("HasPlacedBombEvent") });
	ES->Subscribe(balloom_scoreStatComp, { REC::make_sdbm_hash("HasPlacedBombEvent") });

	auto* changeScene = input->CreateInputBinding();
	changeScene->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_5, REC::ButtonState::Up);
	changeScene->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keypad_5, REC::ButtonState::Up);
	changeScene->AddCommand<REC::ChangeSceneCommand>(engine->GetEngineContext(), scene, startScreen);
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
	REC::Engine engine(data_location);
	engine.Run(load);
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