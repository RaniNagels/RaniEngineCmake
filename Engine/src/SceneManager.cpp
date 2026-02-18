#include "../inc/SceneManager.h"
#include "../inc/Scene.h"
#include "../src/Renderer.h"

void REC::SceneManager::Update(float deltaT)
{
	m_pActiveScene->Update(deltaT);
}

void REC::SceneManager::Render()
{
	Renderer::GetInstance().Render(m_pActiveScene);
}

REC::Scene* REC::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());

	if (m_pActiveScene == nullptr) 
		m_pActiveScene = m_scenes.back().get();

	return m_scenes.back().get();
}
