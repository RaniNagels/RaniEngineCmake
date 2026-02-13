#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace REC
{
	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene();

		void Update(float deltaT);
		void Render();

		void SetActiveScene(Scene* scene) { m_pActiveScene = scene; }

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes{};
		Scene* m_pActiveScene = nullptr;
	};
}
