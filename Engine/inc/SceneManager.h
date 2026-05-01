#pragma once
#include <vector>
#include <string>
#include <memory>

#include <Scene.h>
#include <GameObject.h>

namespace REC
{
	class SceneManager final
	{
	public:
		explicit SceneManager() = default;
		~SceneManager() = default;

		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator= (const SceneManager&) = delete;
		SceneManager& operator= (const SceneManager&&) = delete;

		void Update(float deltaT);
		void Render();

		Scene* CreateScene();
		Scene* GetActiveScene() const { return m_pActiveScene; }
		void SetActiveScene(Scene* scene);

	private:
		//std::vector<std::unique_ptr<GameObject*>> m_pGameObjects{};
		std::vector<std::unique_ptr<Scene>> m_pScenes{};
		Scene* m_pActiveScene = nullptr;
	};
}
