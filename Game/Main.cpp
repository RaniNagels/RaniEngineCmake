#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "../Engine/Minigin.h"
#include "../Engine/SceneManager.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/TextObject.h"
#include "../Engine/Scene.h"
#include "../Engine/Font.h"
#include "../Engine/Texture2D.h"
#include "../Engine/inc/TransformComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = REC::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>();
	go->SetTexture("background.png");
	scene.Add(std::move(go));

	go = std::make_unique<REC::GameObject>();
	go->AddComponent<REC::TransformComponent>(358.f, 180.f);
	go->SetTexture("logo.png");
	scene.Add(std::move(go));

	auto font = REC::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_unique<REC::TextObject>("Programming 4 Assignment", font);
	to->AddComponent<REC::TransformComponent>(292.f, 20.f);
	to->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(to));
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
