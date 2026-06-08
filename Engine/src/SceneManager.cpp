#include <SceneManager.h>

#include <Scene.h>
#include "Renderer.h"

void REC::SceneManager::Update(float deltaT)
{
	m_pActiveScene->Update(deltaT);
}

void REC::SceneManager::Render()
{
	Renderer::GetInstance().Render(m_pActiveScene);
}

REC::Scene* REC::SceneManager::CreateScene(SceneId id)
{
	for (const auto& scene : m_pScenes)
	{
		if (scene->Is(id))
		{
			scene->RemoveAll();
			return scene.get();
		}
	}

	m_pScenes.emplace_back(new Scene(id));

	if (m_pActiveScene == nullptr) 
		m_pActiveScene = m_pScenes.back().get();

	return m_pScenes.back().get();
}

void REC::SceneManager::SetActiveScene(Scene* scene)
{
	if (scene != nullptr)
		m_pActiveScene = scene;
}
