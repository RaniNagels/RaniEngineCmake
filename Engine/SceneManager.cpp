#include "SceneManager.h"
#include "Scene.h"

void REC::SceneManager::Update(float deltaT)
{
	m_pActiveScene->Update(deltaT);
}

void REC::SceneManager::Render()
{
	m_pActiveScene->Render();
}

REC::Scene* REC::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());

	if (m_pActiveScene == nullptr) 
		m_pActiveScene = m_scenes.back().get();

	return m_scenes.back().get();
}
