#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "../Engine/inc/Minigin.h"
#include "../Engine/inc/SceneManager.h"
#include "../Engine/inc/ResourceManager.h"
#include "../Engine/inc/Scene.h"
#include "../Engine/src/Font.h"
#include "../Engine/src/Texture2D.h"

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

namespace fs = std::filesystem;

static void load(REC::Minigin* engine)
{
	// === ENGINE SETTINGS =============================================================================
	REC::EngineSettings engineData{};
	engineData.frameRate = uint8_t(60);
	engineData.windowTitle = "Bomberman";
	engineData.windowWidth = uint16_t(1000);
	engineData.windowHeight = uint16_t(750);
	engine->SetEngineData(engineData);

	// === RESOURCES ===================================================================================
	auto& RM = REC::ResourceManager::GetInstance();

	REC::TextureResourceCreateInfo background{};
	background.name = "background";
	background.filePath = "NES - Bomberman - Backgrounds - Playfield.png";
	RM.AddResource(background);

	REC::TextureResourceCreateInfo generalSprites{};
	generalSprites.name = "generalSprites";
	generalSprites.filePath = "NES - Bomberman - Miscellaneous - General Sprites.png";
	RM.AddResource(generalSprites);

	REC::TextureResourceCreateInfo logo{};
	logo.name = "logo";
	logo.filePath = "logo.png";
	RM.AddResource(logo);

	REC::FontResourceCreateInfo font{};
	font.name = "lingua36";
	font.filePath = "Lingua.otf";
	font.size = uint8_t(36);
	RM.AddResource(font);

	REC::FontResourceCreateInfo debugFont{};
	debugFont.name = "dogicapixel20";
	debugFont.filePath = "dogicapixel.otf";
	debugFont.size = uint8_t(20);
	RM.AddResource(debugFont);

	REC::SpriteDataResourceCreateInfo spriteDataFile{};
	spriteDataFile.name = "generalSpritesData";
	spriteDataFile.filePath = "characterSpritesData.csv";
	spriteDataFile.separator = ';';
	RM.AddResource(spriteDataFile);

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

	auto go = std::make_unique<REC::GameObject>(0.f, 80.f);
	go->AddComponent<REC::SpriteRenderComponent>("background", backdrop);
	go->AddComponent<REC::GridComponent>(grid);
	go->AddComponent<REC::DebugGridRenderComponent>(REC::Color{ uint8_t(20),uint8_t(30),uint8_t(120) });
	scene->Add(std::move(go));

	go = std::make_unique<REC::GameObject>(810.f, 10.f);
	go->AddComponent<REC::SpriteRenderComponent>("logo", uint16_t(0), uint16_t(60));
	scene->Add(std::move(go));

	go = std::make_unique<REC::GameObject>(280.f, 20.f);
	go->AddComponent<REC::TextRenderComponent>("Programming 4 Assignment", "lingua36", REC::Color{ 255, 255, 0 });
	scene->Add(std::move(go));

	go = std::make_unique<REC::GameObject>(20.f, 20.f);
	go->AddComponent<REC::FPSComponent>("dogicapixel20");
	scene->Add(std::move(go));

	// if this is not set as the root of the parent and child, parent will rotate around 0,0 instead
	auto root = std::make_unique<REC::GameObject>(230.f, 200.f);

	REC::SpriteDescriptor character1{};
	character1.drawHeight = 50;
	character1.dataResourceFile = "generalSpritesData";
	character1.spriteDataKey = "bomberman";

	auto parent = std::make_unique<REC::GameObject>(200.f, 200.f);
	parent->AddComponent<REC::SpriteRenderComponent>("generalSprites", character1);
	parent->AddComponent<REC::RotatorComponent>(-3.f);
	parent->SetParent(root.get(), true);

	REC::SpriteDescriptor character2{};
	character2.drawHeight = 50;
	character2.dataResourceFile = "generalSpritesData";
	character2.spriteDataKey = "balloom";
	
	auto child = std::make_unique<REC::GameObject>(50.f, 50.f);
	child->AddComponent<REC::SpriteRenderComponent>("generalSprites", character2);
	child->AddComponent<REC::RotatorComponent>(3.f);
	child->SetParent(parent.get());

	scene->Add(std::move(root));
	scene->Add(std::move(parent));
	scene->Add(std::move(child));
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
	REC::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
