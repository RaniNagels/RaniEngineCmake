// Main.cpp file cannot contain pragma once

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include <IEngine.h>
#include <EngineSettings.h>
#include <Resources/IResourceManager.h>

#include <filesystem>
#include <sdbm_hash.h>
#include <memory>
#include <iostream>

#include "Ids.h"
#include "States/GameStates/MainMenuState.h"

namespace fs = std::filesystem;

static void load(REC::IEngine* engine)
{
	// === ENGINE SETTINGS =============================================================================
	REC::EngineSettings engineData{};
	engineData.frameRate = uint8_t(60);
	engineData.windowTitle = "Bomberman";
	engineData.windowWidth = uint16_t(1488);
	engineData.windowHeight = uint16_t(750);
	engine->SetEngineSettings(engineData);
	engine->SetGameState(std::make_unique<Game::MainMenuState>(engine->GetContext()));

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
	generalSprites.transparentColor = REC::Color{ 56,135,0 };
	if (!RM->AddResource(generalSprites))
		throw std::runtime_error("Failed to load general sprites texture");

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

	REC::SoundResourceCreateInfo placeBombSound{};
	placeBombSound.name = "placeBombSound";
	placeBombSound.filePath = "Sound/bomb_lay.wav";
	if (!RM->AddResource(placeBombSound))
		throw std::runtime_error("Failed to load place bomb sound");

	REC::SoundResourceCreateInfo explosionSound{};
	explosionSound.name = "explosionSound";
	explosionSound.filePath = "Sound/bomb_explosion.wav";
	if (!RM->AddResource(explosionSound))
		throw std::runtime_error("Failed to load explosion sound");

	REC::SoundResourceCreateInfo stepHorizontalSound{};
	stepHorizontalSound.name = "stepHorizontalSound";
	stepHorizontalSound.filePath = "Sound/step_horizontal.wav";
	if (!RM->AddResource(stepHorizontalSound))
		throw std::runtime_error("Failed to load step horizontal sound");

	REC::SoundResourceCreateInfo stepVerticalSound{};
	stepVerticalSound.name = "stepVerticalSound";
	stepVerticalSound.filePath = "Sound/step_vertical.wav";
	if (!RM->AddResource(stepVerticalSound))
		throw std::runtime_error("Failed to load step vertical sound");
#pragma endregion Resources
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

	std::unique_ptr<REC::IEngine, void(*)(REC::IEngine*)> engine{ REC::CreateEngine(data_location), REC::DestroyEngine };
	try
	{
		engine->Run(load);
	}
	catch (const std::exception& e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
	}

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


-- A* pathfinding
NPC's: will use A* pathfinding. 
=> Grid will need connections + weights? 
=> Grid will need to know which cells are walls or blocked!
*/