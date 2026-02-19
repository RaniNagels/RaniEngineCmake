#include <string>
#include "../inc/GameObject.h"
#include "../inc/ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "../inc/Components/TransformComponent.h"
#include "../inc/Components/RenderComponent.h"

REC::GameObject::GameObject()
	: GameObject(glm::vec3{})
{ }

REC::GameObject::GameObject(float x, float y, float z)
	: GameObject(glm::vec3{x, y, z})
{ }

REC::GameObject::GameObject(glm::vec3 position)
{
	m_pTransform = AddComponent<TransformComponent>(position);
}

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
	for (auto& component : m_Components)
	{
		component->Destroy();
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

void REC::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	// check new parent (validity)
	if (parent == this || m_pParent == parent || IsChild(parent)) return;

	// update position, rotation and scale
	if (parent == nullptr)
		GetTransform()->SetLocalPosition(GetTransform()->GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			GetTransform()->SetLocalPosition(GetTransform()->GetWorldPosition() - parent->GetTransform()->GetWorldPosition());
		GetTransform()->RequiresUpdate();
	}

	// remove itself from previous parent
	if (m_pParent)	m_pParent->RemoveChild(this);

	// set given parent on itself
	m_pParent = parent;

	// add itself as a child to the given parent
	if (m_pParent) m_pParent->AddChild(this);
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


bool REC::GameObject::IsChild(const GameObject* object) const
{
	for (auto* child : m_pChildren)
	{
		if (child == object || child->IsChild(object)) return true;
	}
	return false;
}

void REC::GameObject::AddChild(GameObject* child)
{
	// only called from SetParent!

	// check new child (validity)
	if (child == this || child == nullptr || IsChild(child) || m_pParent == child) return;

	// remove from previous child
	// set itself as parent
	// add to list of children
	m_pChildren.emplace_back(child);

	// update position, rotation and scale
}

void REC::GameObject::RemoveChild(GameObject* child)
{
	// only called from SetParent

	// check if the child is valid
	if (child == nullptr || child == this || child == m_pParent) return;

	// remove child from list
	for (auto* c : m_pChildren)
	{
		if (c == child)
		{
			m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), c), m_pChildren.end());
			return;
		}
	}

	// remove itself as parent of the child

	// update position, rotation and scale
}
