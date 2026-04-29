#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"

namespace REC
{
	class IRenderer;
	// only active sccene gets rendered AND updated!
	// TODO: create a reset function to handle switching between scenes more fluently.
	class Scene final
	{
	public:
		explicit Scene() = default;
		~Scene() = default;

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator= (const Scene&) = delete;
		Scene& operator= (const Scene&&) = delete;

		//GameObject* CreateGameObject(float x = 0.f, float y = 0.f, float z = 0.f);
		GameObject* CreateGameObject(const GameObjectDescriptor& descriptor);

		// TODO: maybe have this be set in the GameObject to make it easier for childeren to inherit the layer
		void SetRenderLayer(GameObject* object, uint8_t layer); // 255 possible layers, layer 0 is rendered first, layer 255 is rendered last (on top)
		void RemoveAll();

		void Update(float deltaT);
		void Render(const IRenderer *const renderer) const;

		bool Contains(GameObject* object) const;

	private:
		void RemoveMarkedObjects();
		void ReorderRenderOrder();

		bool m_RenderOrderDirty{ false };

		std::vector<std::unique_ptr<GameObject>> m_Objects{}; // owns the gameObjects in its scene TODO: maybe not do this
		std::vector<uint8_t> m_RenderLayers{}; // at the same place of the index of the object
		std::vector<size_t> m_RenderOrder{}; // contains the indices of the gameobject in the order they should be rendered. each index should only appear once!
	};

}
