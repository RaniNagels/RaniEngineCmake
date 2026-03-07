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
#include "MoveCommand.h"
#include <Components/ControllerComponent.h>

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
	std::vector<REC::ResourceCreateInfo*> infos{};

	REC::TextureResourceCreateInfo background{};
	background.name = "background";
	background.filePath = "NES - Bomberman - Backgrounds - Playfield.png";
	infos.emplace_back(&background);

	REC::TextureResourceCreateInfo generalSprites{};
	generalSprites.name = "generalSprites";
	generalSprites.filePath = "NES - Bomberman - Miscellaneous - General Sprites.png";
	infos.emplace_back(&generalSprites);

	REC::TextureResourceCreateInfo logo{};
	logo.name = "logo";
	logo.filePath = "logo.png";
	infos.emplace_back(&logo);

	REC::FontResourceCreateInfo font{};
	font.name = "lingua36";
	font.filePath = "Lingua.otf";
	font.size = uint8_t(36);
	infos.emplace_back(&font);

	REC::FontResourceCreateInfo debugFont{};
	debugFont.name = "dogicapixel20";
	debugFont.filePath = "dogicapixel.otf";
	debugFont.size = uint8_t(20);
	infos.emplace_back(&debugFont);

	REC::FileResourceCreateInfo dataFile{};
	dataFile.filePath = "characterFramesData.Json";
	dataFile.dataTypes = REC::FileResourceCreateInfo::LoadTypes::Frames | REC::FileResourceCreateInfo::LoadTypes::Animations;
	infos.emplace_back(&dataFile);

	REC::FileResourceCreateInfo titleScreenDataFile{};
	titleScreenDataFile.filePath = "TitleScreenFramesData.json";
	titleScreenDataFile.dataTypes = REC::FileResourceCreateInfo::LoadTypes::Frames | REC::FileResourceCreateInfo::LoadTypes::TextureFont;
	infos.emplace_back(&titleScreenDataFile);

	REC::TextureResourceCreateInfo titleScreen{};
	titleScreen.name = "titleScreen";
	titleScreen.filePath = "NES - Bomberman - Miscellaneous - Title Screen & Text.png";
	infos.emplace_back(&titleScreen);
	
	engine->AddResources(infos);

	// === SCENE =======================================================================================
	auto* SM = engine->GetSceneManager();
	//auto* startScreen = SM->CreateScene();
	//
	//REC::SpriteDescriptor startScreenBackdrop{};
	//startScreenBackdrop.drawHeight = 750;
	//startScreenBackdrop.dataResourceFile = "titleScreenDataFile";
	//startScreenBackdrop.textureKey = "titleScreen";
	//startScreenBackdrop.spriteDataKey = "start_up_screen_1987";
	//
	//auto* stsc = startScreen->CreateGameObject(125,0);
	//stsc->AddComponent<REC::SpriteRenderComponent>(startScreenBackdrop);

	auto* scene = SM->CreateScene();

	REC::GridDescriptor grid{};
	grid.cellHeight = uint8_t(51);
	grid.cellWidth = uint8_t(51);
	grid.rows = uint8_t(13);
	grid.cols = uint8_t(31);

	REC::SpriteDescriptor backdrop{};
	backdrop.drawHeight = uint16_t(grid.cellHeight)*uint16_t(grid.rows);
	backdrop.frameKey = "background";
	backdrop.textureKey = "background";

	auto go = scene->CreateGameObject(0.f, 80.f);
	go->AddComponent<REC::SpriteRenderComponent>(backdrop);
	go->AddComponent<REC::GridComponent>(grid);
	go->AddComponent<REC::DebugGridRenderComponent>(REC::Color{ uint8_t(20),uint8_t(30),uint8_t(120) });

	//go = scene->CreateGameObject(810.f, 10.f); 
	//go->AddComponent<REC::SpriteRenderComponent>("logo", uint16_t(0), uint16_t(60));

	auto instructions = scene->CreateGameObject(200.f, 20.f);

	auto instructionsBalloom = scene->CreateGameObject();
	instructionsBalloom->AddComponent<REC::TextRenderComponent>("Use the D-Pad or left Thumb Stick to move Balloom", "dogicapixel20");
	instructionsBalloom->SetParent(instructions);

	auto instructionsBomberman = scene->CreateGameObject(0.f, 28.f);
	instructionsBomberman->AddComponent<REC::TextRenderComponent>("Use WASD to move Bomberman", "dogicapixel20");
	instructionsBomberman->SetParent(instructions);

	go = scene->CreateGameObject(20.f, 20.f); 
	go->AddComponent<REC::FPSComponent>("dogicapixel20");

	REC::SpriteDescriptor character1{};
	character1.drawHeight = 50;
	character1.textureKey = "generalSprites";

	REC::AnimationDescriptor animation1{};
	animation1.animationKey = "bomberman_walk_left";
	animation1.startOnStartup = true;

	auto parent = scene->CreateGameObject(200.f, 200.f); 
	parent->AddComponent<REC::SpriteRenderComponent>(character1);
	parent->AddComponent<REC::AnimatedSpriteComponent>(animation1);

	REC::SpriteDescriptor character2{};
	character2.drawHeight = 50;
	character2.textureKey = "generalSprites";

	REC::AnimationDescriptor animation2{};
	animation2.animationKey = "balloom_look_left";
	animation2.startOnStartup = true;
	
	auto child = scene->CreateGameObject(50.f, 50.f); 
	child->AddComponent<REC::SpriteRenderComponent>(character2);
	child->AddComponent<REC::AnimatedSpriteComponent>(animation2);
	child->AddComponent<REC::ControllerComponent>(uint8_t(0));

	// === INPUT =======================================================================================
	float char1_speed{ 3.f };

	auto* char1_right = engine->CreateInputAction();
	char1_right->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_D, REC::ButtonState::Pressed);
	char1_right->AddCommand<Game::MoveCommand>(parent, glm::vec2{ 1, 0 }, char1_speed);

	auto* char1_left = engine->CreateInputAction();
	char1_left->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_A, REC::ButtonState::Pressed);
	char1_left->AddCommand<Game::MoveCommand>(parent, glm::vec2{ -1, 0 }, char1_speed);

	auto* char1_up = engine->CreateInputAction();
	char1_up->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_W, REC::ButtonState::Pressed);
	char1_up->AddCommand<Game::MoveCommand>(parent, glm::vec2{ 0, -1 }, char1_speed);

	auto* char1_down = engine->CreateInputAction();
	char1_down->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_S, REC::ButtonState::Pressed);
	char1_down->AddCommand<Game::MoveCommand>(parent, glm::vec2{ 0, 1 }, char1_speed);

	float char2_speed{ 6.f };

	auto* char2_right = engine->CreateInputAction();
	char2_right->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::GamePad_DPad_Right, REC::ButtonState::Pressed);
	char2_right->AddInputAction<REC::ControllerRangeAction>(REC::Input::Controller::Range::Gamepad_LeftStick_X);
	char2_right->AddCommand<Game::MoveCommand>(child, glm::vec2{ 1, 0 }, char2_speed);

	auto* char2_left = engine->CreateInputAction();
	char2_left->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::GamePad_DPad_Left, REC::ButtonState::Pressed);
	char2_left->AddCommand<Game::MoveCommand>(child, glm::vec2{ -1, 0 }, char2_speed);

	auto* char2_up = engine->CreateInputAction();
	char2_up->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::GamePad_DPad_Up, REC::ButtonState::Pressed);
	char2_up->AddInputAction<REC::ControllerRangeAction>(REC::Input::Controller::Range::Gamepad_LeftStick_Y);
	char2_up->AddCommand<Game::MoveCommand>(child, glm::vec2{ 0, -1 }, char2_speed);

	auto* char2_down = engine->CreateInputAction();
	char2_down->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::GamePad_DPad_Down, REC::ButtonState::Pressed);
	char2_down->AddCommand<Game::MoveCommand>(child, glm::vec2{ 0, 1 }, char2_speed);
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
