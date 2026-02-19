#include <string>
#include "../inc/GameObject.h"
#include "../inc/ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "../inc/Components/TransformComponent.h"
#include "../inc/Components/RenderComponent.h"

REC::GameObject::~GameObject() = default;

void REC::GameObject::Update(float deltaT)
{
	for (const auto& component : m_Components)
	{
		component->Update(deltaT);
	}

	if (m_ShouldCleanUpComponents)
		CleanUpComponents();
}

void REC::GameObject::Destroy()
{
	m_IsAboutToBeDestroyed = true;
}

void REC::GameObject::Render() const
{
	for (const auto& component : m_Components)
	{
		if (component->CanRender())
		{
			// expensive
			dynamic_cast<RenderComponent*>(component.get())->Render();
		}
	}
}

void REC::GameObject::CleanUpComponents()
{
	m_Components.erase(
		std::remove_if(m_Components.begin(), m_Components.end(),
		[](const std::unique_ptr<Component>& component)
		{
			return component->IsAboutToBeDestroyed();
		}), 
		m_Components.end()
	);

	m_ShouldCleanUpComponents = false;
}
