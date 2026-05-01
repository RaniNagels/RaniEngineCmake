#include <Scene.h>

#include <algorithm>

REC::GameObject* REC::Scene::CreateGameObject(const GameObjectDescriptor& descriptor)
{
	m_Objects.emplace_back(std::make_unique<GameObject>(descriptor));
	m_RenderLayers.emplace_back(descriptor.renderLayer);
	m_RenderOrder.emplace_back(m_RenderLayers.size() - 1);
	m_RenderOrderDirty = true;
	return m_Objects.back().get();
}

void REC::Scene::SetRenderLayer(GameObject* object, uint8_t layer)
{
	for (size_t i{}; i < m_Objects.size(); ++i)
	{
		if (m_Objects[i].get() == object)
		{
			m_RenderLayers[i] = layer;
			m_RenderOrderDirty = true;
			return;
		}
	}

}

void REC::Scene::RemoveMarkedObjects()
{
	for (int i{ static_cast<int>(m_Objects.size()) - 1 }; i >= 0; --i)
	{
		if (m_Objects[i]->IsAboutToBeDestroyed())
		{
			m_Objects.erase(m_Objects.begin() + i);
			m_RenderLayers.erase(m_RenderLayers.begin() + i);
			m_RenderOrder.erase(std::remove_if(m_RenderOrder.begin(), m_RenderOrder.end(), [i](size_t idx) {return static_cast<int>(idx) == i; }), m_RenderOrder.end());

			for (size_t& idx : m_RenderOrder)
			{
				if (idx > static_cast<size_t>(i))
					--idx;
			}
		}
	}
}

void REC::Scene::RemoveAll()
{
	for (auto& object : m_Objects)
	{
		object->Destroy();
	}
}

void REC::Scene::Update(float deltaT)
{
	for(auto& object : m_Objects)
	{
		object->Update(deltaT);
	}

	RemoveMarkedObjects();

	if (m_RenderOrderDirty)
	{
		ReorderRenderOrder();
		m_RenderOrderDirty = false;
	}
}

void REC::Scene::Render(const IRenderer *const renderer) const
{
	// first render gameObjects on layer 0, then 1 ...
	for (auto index : m_RenderOrder)
	{
		m_Objects[index]->Render(renderer);
	}
}

void REC::Scene::ReorderRenderOrder()
{
	std::sort(m_RenderOrder.begin(), m_RenderOrder.end(),
		[&](size_t a, size_t b) { return m_RenderLayers[a] < m_RenderLayers[b]; });
}

bool REC::Scene::Contains(GameObject* object) const
{
	for (const auto& obj : m_Objects)
	{
		if (obj.get() == object)
			return true;
	}

	return false;
}
