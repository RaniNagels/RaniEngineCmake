#include <algorithm>
#include "../inc/Scene.h"

REC::GameObject* REC::Scene::CreateGameObject(float x, float y, float z)
{
	m_objects.emplace_back(std::make_unique<GameObject>(x, y, z));
	return m_objects.back().get();
}

void REC::Scene::RemoveMarkedObjects()
{
	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[](const std::unique_ptr<GameObject>& object) 
			{ return object->IsAboutToBeDestroyed(); }
		),
		m_objects.end()
	);
}

void REC::Scene::RemoveAll()
{
	for (auto& object : m_objects)
	{
		object->Destroy();
	}
}

void REC::Scene::Update(float deltaT)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaT);
	}

	RemoveMarkedObjects();
}

void REC::Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

