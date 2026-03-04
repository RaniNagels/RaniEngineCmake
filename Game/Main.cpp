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
#include "../Engine/inc/SpriteDescriptor.h"
#include "../Engine/inc/Components/GridComponent.h"
#include "../Engine/inc/Components/DebugGridRenderComponent.h"
#include <Components/TrashTheCacheRenderComponent.h>

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

	REC::SpriteDataResourceCreateInfo spriteDataFile{};
	spriteDataFile.name = "generalSpritesData";
	spriteDataFile.filePath = "characterSpritesData.csv";
	spriteDataFile.type = REC::FileResourceCreateInfo::FileType::CSV;
	spriteDataFile.separator = ';';
	infos.emplace_back(&spriteDataFile);
	
	engine->AddResources(infos);

	// === SCENE =======================================================================================
	auto* SM = engine->GetSceneManager();
	auto* scene = SM->CreateScene();

	REC::GridDescriptor grid{};
	grid.cellHeight = uint8_t(51);
	grid.cellWidth = uint8_t(51);
	grid.rows = uint8_t(13);
	grid.cols = uint8_t(31);

	REC::SpriteDescriptor backdrop{};
	backdrop.drawHeight = uint16_t(grid.cellHeight)*uint16_t(grid.rows);
	backdrop.dataResourceFile = "generalSpritesData";
	backdrop.spriteDataKey = "background";

	auto go = scene->CreateGameObject(0.f, 80.f);
	go->AddComponent<REC::SpriteRenderComponent>("background", backdrop);
	go->AddComponent<REC::GridComponent>(grid);
	go->AddComponent<REC::DebugGridRenderComponent>(REC::Color{ uint8_t(20),uint8_t(30),uint8_t(120) });

	go = scene->CreateGameObject(810.f, 10.f); 
	go->AddComponent<REC::SpriteRenderComponent>("logo", uint16_t(0), uint16_t(60));

	go = scene->CreateGameObject(280.f, 20.f);
	go->AddComponent<REC::TextRenderComponent>("Programming 4 Assignment", "lingua36", REC::Color{ 255, 255, 0 });

	go = scene->CreateGameObject(20.f, 20.f); 
	go->AddComponent<REC::FPSComponent>("dogicapixel20");

	// if this is not set as the root of the parent and child, parent will rotate around 0,0 instead
	auto root = scene->CreateGameObject(230.f, 200.f);

	REC::SpriteDescriptor character1{};
	character1.drawHeight = 50;
	character1.dataResourceFile = "generalSpritesData";
	character1.spriteDataKey = "bomberman_walk_right_2";

	auto parent = scene->CreateGameObject(200.f, 200.f); 
	parent->AddComponent<REC::SpriteRenderComponent>("generalSprites", character1);
	parent->AddComponent<REC::RotatorComponent>(-3.f);
	parent->SetParent(root, true);

	REC::SpriteDescriptor character2{};
	character2.drawHeight = 50;
	character2.dataResourceFile = "generalSpritesData";
	character2.spriteDataKey = "balloom";
	
	auto child = scene->CreateGameObject(50.f, 50.f); 
	child->AddComponent<REC::SpriteRenderComponent>("generalSprites", character2);
	child->AddComponent<REC::RotatorComponent>(3.f);
	child->SetParent(parent);

	auto trashTheCash = scene->CreateGameObject(); 
	trashTheCash->AddComponent<REC::TrashTheCacheRenderComponent>();
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
