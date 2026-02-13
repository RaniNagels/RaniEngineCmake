#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "inc/TransformComponent.h"
#include "inc/RenderComponent.h"

REC::GameObject::~GameObject() = default;

void REC::GameObject::Update(float deltaT)
{
	for (const auto& component : m_Components)
	{
		component->Update(deltaT);
	}
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
