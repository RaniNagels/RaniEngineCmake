#pragma once
#include <memory>
#include <string>
#include <vector>

#include <GameObject.h>

namespace REC
{
	using SceneId = unsigned int;

	class IRenderer;
	// only active sccene gets rendered AND updated!
	// TODO: create a reset function to handle switching between scenes more fluently.
	class Scene final
	{
	public:
		explicit Scene(SceneId id) : m_Id(id) {}
		~Scene() = default;

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator= (const Scene&) = delete;
		Scene& operator= (const Scene&&) = delete;

		//GameObject* CreateGameObject(float x = 0.f, float y = 0.f, float z = 0.f);
		GameObject* CreateGameObject(const GameObjectDescriptor& descriptor);
		GameObject* GetGameObject(ObjectId id) const; // returns the first gameobject with the given id, nullptr if not found
		std::vector<GameObject*> GetGameObjects(ObjectId id) const; // returns all gameobjects with the given id, empty vector if not found

		// TODO: maybe have this be set in the GameObject to make it easier for childeren to inherit the layer
		void SetRenderLayer(GameObject* object, uint8_t layer); // 255 possible layers, layer 0 is rendered first, layer 255 is rendered last (on top)
		void RemoveAll();

		void Update(float deltaT);
		void Render(const IRenderer *const renderer) const;

		bool Contains(GameObject* object) const;
		bool Is(SceneId id) const { return m_Id == id; }

	private:
		void RemoveMarkedObjects();
		void ReorderRenderOrder();

		bool m_RenderOrderDirty{ false };

		SceneId m_Id{};

		std::vector<std::unique_ptr<GameObject>> m_Objects{}; // owns the gameObjects in its scene TODO: maybe not do this
		std::vector<uint8_t> m_RenderLayers{}; // at the same place of the index of the object
		std::vector<size_t> m_RenderOrder{}; // contains the indices of the gameobject in the order they should be rendered. each index should only appear once!
	};

}
