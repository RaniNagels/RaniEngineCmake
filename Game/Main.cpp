#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "../Engine/Engine.h"
#include "../Engine/SceneManager.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/Scene.h"
#include "../Engine/Font.h"
#include "../Engine/Texture2D.h"
#include "../Engine/inc/TransformComponent.h"
#include "../Engine/inc/SpriteRenderComponent.h"
#include "../Engine/inc/TextRenderComponent.h"
#include "../Engine/inc/ResourceDescriptors.h"
#include "../Engine/inc/FPSRenderComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	// === RESOURCES ===================================================================================
	REC::TextureResourceDesc background{};
	background.name = "background";
	background.filePath = "background.png";

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
	auto* scene = REC::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>();
	go->AddComponent<REC::SpriteRenderComponent>("background");
	scene->Add(std::move(go));

	go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>(358.f, 180.f);
	go->AddComponent<REC::SpriteRenderComponent>("logo");
	scene->Add(std::move(go));

	go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>(292.f, 20.f);
	go->AddComponent<REC::TextRenderComponent>("Programming 4 Assignment", "lingua36", SDL_Color{ 255, 255, 0, 255 });
	scene->Add(std::move(go));

	go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>(20.f, 20.f);
	go->AddComponent<REC::FPSRenderComponent>("lingua36");
	scene->Add(std::move(go));
}

int main(int, char*[]) {
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
