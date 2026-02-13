#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "../Engine/Minigin.h"
#include "../Engine/SceneManager.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/Scene.h"
#include "../Engine/Font.h"
#include "../Engine/Texture2D.h"
#include "../Engine/inc/TransformComponent.h"
#include "../Engine/inc/SpriteRenderComponent.h"
#include "../Engine/inc/TextRenderComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = REC::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>();
	auto* background = REC::ResourceManager::GetInstance().LoadTexture("background.png");
	go->AddComponent<REC::SpriteRenderComponent>(background);
	scene.Add(std::move(go));

	go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>(358.f, 180.f);
	auto* logo = REC::ResourceManager::GetInstance().LoadTexture("logo.png");
	go->AddComponent<REC::SpriteRenderComponent>(logo);
	scene.Add(std::move(go));

	auto font = REC::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>(292.f, 20.f);
	go->AddComponent<REC::TextRenderComponent>("Programming 4 Assignment", font, SDL_Color{ 255, 255, 0, 255 });
	scene.Add(std::move(go));
}

int main(int, char*[]) {
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
