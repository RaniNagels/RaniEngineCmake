#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

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
#include "../Engine/inc/Components/FPSRenderComponent.h"

#include "../Engine/inc/ResourceDescriptors.h"
#include "../Engine/inc/EngineDescriptor.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load(REC::Minigin* engine)
{
	// === ENGINE SETTINGS =============================================================================
	REC::EngineDesc engineData{};
	engineData.frameRate = 60;
	engineData.windowTitle = "Bomberman";
	engineData.windowWidth = 1000;
	engineData.windowHeight = 600;
	engine->SetEngineData(engineData);

	// === RESOURCES ===================================================================================
	REC::TextureResourceDesc background{};
	background.name = "background";
	background.filePath = "NES - Bomberman - Backgrounds - Playfield.png";

	REC::TextureResourceDesc logo{};
	logo.name = "logo";
	logo.filePath = "logo.png";

	REC::FontResourceDesc font{};
	font.name = "lingua36";
	font.filePath = "Lingua.otf";
	font.size = 36;

	auto& RM = REC::ResourceManager::GetInstance();
	RM.AddResource(background);
	RM.AddResource(logo);
	RM.AddResource(font);

	// === SCENE =======================================================================================
	auto* SM = engine->GetSceneManager();
	auto* scene = SM->CreateScene();

	auto go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>(0.f, 80.f);
	go->AddComponent<REC::SpriteRenderComponent>("background", 0, 565);
	scene->Add(std::move(go));

	go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>(810.f, 10.f);
	go->AddComponent<REC::SpriteRenderComponent>("logo", 0, 60);
	scene->Add(std::move(go));

	go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>(292.f, 20.f);
	go->AddComponent<REC::TextRenderComponent>("Programming 4 Assignment", "lingua36", REC::Color{ 255, 255, 0 });
	scene->Add(std::move(go));

	go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>(20.f, 20.f);
	go->AddComponent<REC::FPSRenderComponent>("lingua36");
	scene->Add(std::move(go));
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
