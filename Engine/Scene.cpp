#include <algorithm>
#include "Scene.h"

void REC::Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void REC::Scene::Remove(const GameObject& object)
{
	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[&object](const auto& ptr) { return ptr.get() == &object; }
		),
		m_objects.end()
	);
}

void REC::Scene::RemoveAll()
{
	m_objects.clear();
}

void REC::Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void REC::Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

