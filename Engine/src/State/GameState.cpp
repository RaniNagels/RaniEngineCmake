#include <State/GameState.h>
#include <SceneManager.h>
#include <Scene.h>

REC::Scene* REC::GameState::CreateScene(SceneId id)
{
	if (m_pScene)
		return m_pScene;

	m_pScene = GetContext().sceneManager->CreateScene(id);
	GetContext().sceneManager->SetActiveScene(m_pScene);
	return m_pScene;
}