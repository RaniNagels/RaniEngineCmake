#include "SceneManager.h"
#include "Scene.h"

void REC::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void REC::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

REC::Scene& REC::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}
