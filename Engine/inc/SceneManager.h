#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"

namespace REC
{
	class Scene;

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
		void SetActiveScene(Scene* scene) { m_pActiveScene = scene; }

	private:
		std::vector<std::unique_ptr<Scene>> m_scenes{};
		Scene* m_pActiveScene = nullptr;
	};
}
