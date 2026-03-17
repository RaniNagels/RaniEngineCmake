// Main.cpp file cannot contain pragma once

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include <Engine.h>
#include "../Engine/inc/SceneManager.h"
#include "../Engine/inc/Scene.h"

#include "../Engine/inc/Components/TransformComponent.h"
#include "../Engine/inc/Components/SpriteRenderComponent.h"
#include "../Engine/inc/Components/TextRenderComponent.h"
#include "../Engine/inc/Components/FPSComponent.h"
#include "../Engine/inc/Components/RotatorComponent.h"

#include "../Engine/inc/ResourceCreateInfos.h"
#include "../Engine/inc/EngineSettings.h"

#include <filesystem>
#include <ComponentDescriptors.h>
#include "../Engine/inc/Components/GridComponent.h"
#include "../Engine/inc/Components/DebugGridRenderComponent.h"
#include <Components/AnimatedSpriteComponent.h>
#include "Commands/MoveCommand.h"
#include "Commands/PlaceBombCommand.h"
#include <Components/ControllerComponent.h>
#include <Commands/ChangeSceneCommand.h>
#include <Input/InputSystem.h>
#include <Components/LabeledStatComponent.h>

#include "RenderLayers.h"

namespace fs = std::filesystem;

void CreateUI(REC::Scene* scene);

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
	std::vector<REC::ResourceCreateInfo*> infos{};
	// !! double check all filepaths !! Json != json (will not give an error in MSCV or clang, but will cause vague JavaScript error)
	// TODO: pass ownership instead

	REC::TextureResourceCreateInfo background{};
	background.name = "background";
	background.filePath = "NES - Bomberman - Backgrounds - Playfield.png";
	infos.emplace_back(&background);

	REC::TextureResourceCreateInfo generalSprites{};
	generalSprites.name = "generalSprites";
	generalSprites.filePath = "NES - Bomberman - Miscellaneous - General Sprites.png";
	infos.emplace_back(&generalSprites);

	REC::FontResourceCreateInfo font{};
	font.name = "lingua36";
	font.filePath = "Lingua.otf";
	font.size = uint8_t(36);
	infos.emplace_back(&font);

	REC::FontResourceCreateInfo debugFont{};
	debugFont.name = "dogicapixel16";
	debugFont.filePath = "dogicapixel.otf";
	debugFont.size = uint8_t(16);
	infos.emplace_back(&debugFont);

	REC::FontResourceCreateInfo debugFont20{};
	debugFont20.name = "dogicapixel20";
	debugFont20.filePath = "dogicapixel.otf";
	debugFont20.size = uint8_t(20);
	infos.emplace_back(&debugFont20);

	REC::FileResourceCreateInfo dataFile{};
	dataFile.name = "characterData";
	dataFile.filePath = "characterFramesData.json";
	dataFile.dataTypes = REC::LoadTypes::Frames | REC::LoadTypes::Animations;
	infos.emplace_back(&dataFile);

	REC::FileResourceCreateInfo titleScreenDataFile{};
	titleScreenDataFile.name = "startScreenData";
	titleScreenDataFile.filePath = "TitleScreenFramesData.json";
	titleScreenDataFile.dataTypes = REC::LoadTypes::Frames | REC::LoadTypes::TextureFont;
	infos.emplace_back(&titleScreenDataFile);

	REC::TextureResourceCreateInfo titleScreen{};
	titleScreen.name = "titleScreen";
	titleScreen.filePath = "NES - Bomberman - Miscellaneous - Title Screen & Text.png";
	infos.emplace_back(&titleScreen);
	
	engine->AddResources(infos);

	// === SCENE =======================================================================================
	auto* SM = engine->GetSceneManager();
	auto* startScreen = SM->CreateScene();
	
	REC::SpriteDescriptor startScreenBackdrop{};
	startScreenBackdrop.drawHeight = 750;
	startScreenBackdrop.textureKey = "titleScreen";
	startScreenBackdrop.frameDataFileKey = "startScreenData";
	startScreenBackdrop.frameKey = "start_up_screen_1987";
	
	auto* stsc = startScreen->CreateGameObject(125,0);
	stsc->AddComponent<REC::SpriteRenderComponent>(startScreenBackdrop);
	startScreen->SetRenderLayer(stsc, Game::GetLayer(Game::RenderLayer::BACKGROUND));

	auto* stscInstruction = startScreen->CreateGameObject(75, 320);
	stscInstruction->AddComponent<REC::TextRenderComponent>("Press '5' to start", "dogicapixel20", REC::Color{0,255,255});
	startScreen->SetRenderLayer(stscInstruction, Game::GetLayer(Game::RenderLayer::UI));

	auto* scene = SM->CreateScene();

	REC::GridDescriptor grid{};
	grid.cellHeight = uint8_t(45); //51
	grid.cellWidth = uint8_t(45);  //51
	grid.rows = uint8_t(13);
	grid.cols = uint8_t(31);

	REC::SpriteDescriptor backdrop{};
	backdrop.drawHeight = uint16_t(grid.cellHeight)*uint16_t(grid.rows);
	backdrop.frameDataFileKey = "characterData";
	backdrop.frameKey = "background";
	backdrop.textureKey = "background";

	auto go = scene->CreateGameObject(0.f, 80.f);
	go->AddComponent<REC::SpriteRenderComponent>(backdrop);
	go->AddComponent<REC::GridComponent>(grid);
	go->AddComponent<REC::DebugGridRenderComponent>(REC::Color{ uint8_t(20),uint8_t(30),uint8_t(120) });
	scene->SetRenderLayer(go, Game::GetLayer(Game::RenderLayer::BACKGROUND));

	auto instructions = scene->CreateGameObject(20.f, 20.f);
	scene->SetRenderLayer(instructions, Game::GetLayer(Game::RenderLayer::UI));
	
	auto instructionsBalloom = scene->CreateGameObject();
	instructionsBalloom->AddComponent<REC::TextRenderComponent>("Use the D-Pad or left Thumb Stick to move Balloom", "dogicapixel16");
	instructionsBalloom->SetParent(instructions);
	
	auto instructionsBomberman = scene->CreateGameObject(0.f, 28.f);
	instructionsBomberman->AddComponent<REC::TextRenderComponent>("Use WASD to move Bomberman", "dogicapixel16");
	instructionsBomberman->SetParent(instructions);

	CreateUI(scene);

	auto fps = scene->CreateGameObject(880.f, 20.f); 
	fps->AddComponent<REC::FPSComponent>("dogicapixel20");
	scene->SetRenderLayer(fps, Game::GetLayer(Game::RenderLayer::UI));

	REC::SpriteDescriptor character1{};
	character1.drawHeight = 50;
	character1.frameDataFileKey = "characterData";
	character1.textureKey = "generalSprites";

	REC::AnimationDescriptor animation1{};
	animation1.animationDataFileKey = "characterData";
	animation1.animationKey = "bomberman_walk_left";
	animation1.startOnStartup = true;

	auto parent = scene->CreateGameObject(200.f, 200.f); 
	parent->AddComponent<REC::SpriteRenderComponent>(character1);
	parent->AddComponent<REC::AnimatedSpriteComponent>(animation1);
	scene->SetRenderLayer(parent, Game::GetLayer(Game::RenderLayer::PLAYER));

	uint8_t balloomControllerId{ 0 };

	REC::SpriteDescriptor balloomSpriteDesc{};
	balloomSpriteDesc.drawHeight = 50;
	balloomSpriteDesc.frameDataFileKey = "characterData";
	balloomSpriteDesc.textureKey = "generalSprites";

	REC::AnimationDescriptor balloomAnimDesc{};
	balloomAnimDesc.animationDataFileKey = "characterData";
	balloomAnimDesc.animationKey = "balloom_look_left";
	balloomAnimDesc.startOnStartup = true;
	
	auto balloom = scene->CreateGameObject(350.f, 250.f); 
	balloom->AddComponent<REC::SpriteRenderComponent>(balloomSpriteDesc);
	balloom->AddComponent<REC::AnimatedSpriteComponent>(balloomAnimDesc);
	scene->SetRenderLayer(balloom, Game::GetLayer(Game::RenderLayer::ENEMIES));

	// === INPUT =======================================================================================
	auto* input = engine->GetInputSystem();
	input->SetNumberOfActiveControllers(1);

	float char1_speed{ 100.f };

	auto* char1_right = input->CreateInputBinding();
	char1_right->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_D, REC::ButtonState::Pressed);
	char1_right->AddCommand<Game::MoveCommand>(parent, glm::vec2{ 1, 0 }, char1_speed);

	auto* char1_left = input->CreateInputBinding();
	char1_left->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_A, REC::ButtonState::Pressed);
	char1_left->AddCommand<Game::MoveCommand>(parent, glm::vec2{ -1, 0 }, char1_speed);

	auto* char1_up = input->CreateInputBinding();
	char1_up->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_W, REC::ButtonState::Pressed);
	char1_up->AddCommand<Game::MoveCommand>(parent, glm::vec2{ 0, -1 }, char1_speed);

	auto* char1_down = input->CreateInputBinding();
	char1_down->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_S, REC::ButtonState::Pressed);
	char1_down->AddCommand<Game::MoveCommand>(parent, glm::vec2{ 0, 1 }, char1_speed);

	float char2_speed{ char1_speed*2 };

	auto* char2_right = input->CreateInputBinding();
	char2_right->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::GamePad_DPad_Right, REC::ButtonState::Pressed, balloomControllerId);
	char2_right->AddInputAction<REC::ControllerRangeAction>(REC::Input::Controller::Range::Gamepad_LeftStick_X, balloomControllerId);
	char2_right->AddCommand<Game::MoveCommand>(balloom, glm::vec2{ 1, 0 }, char2_speed);

	auto* char2_left = input->CreateInputBinding();
	char2_left->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::GamePad_DPad_Left, REC::ButtonState::Pressed, balloomControllerId);
	char2_left->AddCommand<Game::MoveCommand>(balloom, glm::vec2{ -1, 0 }, char2_speed);

	auto* char2_up = input->CreateInputBinding();
	char2_up->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::GamePad_DPad_Up, REC::ButtonState::Pressed, balloomControllerId);
	char2_up->AddInputAction<REC::ControllerRangeAction>(REC::Input::Controller::Range::Gamepad_LeftStick_Y, balloomControllerId);
	char2_up->AddCommand<Game::MoveCommand>(balloom, glm::vec2{ 0, -1 }, char2_speed);

	auto* char2_down = input->CreateInputBinding();
	char2_down->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::GamePad_DPad_Down, REC::ButtonState::Pressed, balloomControllerId);
	char2_down->AddCommand<Game::MoveCommand>(balloom, glm::vec2{ 0, 1 }, char2_speed);

	auto* changeScene = input->CreateInputBinding();
	changeScene->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_5, REC::ButtonState::Up);
	changeScene->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keypad_5, REC::ButtonState::Up);
	changeScene->AddCommand<REC::ChangeSceneCommand>(engine->GetEngineContext(), scene, startScreen);

	auto* controllerDropBomb = input->CreateInputBinding();
	controllerDropBomb->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::Gamepad_A, REC::ButtonState::Up, balloomControllerId);
	controllerDropBomb->AddCommand<Game::PlaceBombCommand>(balloom, SM);

	auto* keyboardDropBomb = input->CreateInputBinding();
	keyboardDropBomb->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_Space, REC::ButtonState::Up);
	keyboardDropBomb->AddCommand<Game::PlaceBombCommand>(parent, SM);
}

void CreateUI(REC::Scene* scene)
{
	auto UI = scene->CreateGameObject(20.f, 680.f);
	scene->SetRenderLayer(UI, Game::GetLayer(Game::RenderLayer::UI));

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
	bombermanUILives->AddComponent<REC::LabeledStatComponent>(livesStatDesciptor);
	bombermanUILives->AddComponent<REC::SpriteRenderComponent>(bombermanIcon);
	bombermanUILives->SetParent(UI);

	REC::SpriteDescriptor balloomIcon{};
	balloomIcon.drawHeight = 20;
	balloomIcon.textureKey = "generalSprites";
	balloomIcon.frameDataFileKey = "characterData";
	balloomIcon.frameKey = "balloom_look_right_0";

	auto balloomUILives = scene->CreateGameObject(0.f, 30.f);
	balloomUILives->AddComponent<REC::LabeledStatComponent>(livesStatDesciptor);
	balloomUILives->AddComponent<REC::SpriteRenderComponent>(balloomIcon);
	balloomUILives->SetParent(UI);

	REC::LabeledStatDescriptor scoreStatDesciptor{};
	scoreStatDesciptor.fontkey = "dogicapixel16";
	scoreStatDesciptor.label = "Score";
	scoreStatDesciptor.initialValue = 0;
	scoreStatDesciptor.color = REC::Color{ 255,255,255 };

	auto bombermanUIScore = scene->CreateGameObject(350.f, 0);
	bombermanUIScore->AddComponent<REC::LabeledStatComponent>(scoreStatDesciptor);
	bombermanUIScore->SetParent(UI);

	auto balloomUIScore = scene->CreateGameObject(350.f, 30.f);
	balloomUIScore->AddComponent<REC::LabeledStatComponent>(scoreStatDesciptor);
	balloomUIScore->SetParent(UI);
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
