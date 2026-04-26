#pragma once
#include <string>
#include <memory>
#include "Components/Component.h"
#include <vector>
#include <assert.h>
#include <glm/glm.hpp>
#include <GameObjectDescriptor.h>
#include <Components/CollisionComponent.h>
#include <stdexcept>

namespace REC
{
	using ObjectId = unsigned int;

	template <typename C>
	concept Cpt = std::derived_from<C, Component> && !std::derived_from<C, CollisionComponent>;

	template <typename CC>
	concept CCpt = std::derived_from<CC, CollisionComponent>;

	//https://en.cppreference.com/w/cpp/language/constraints.html
	// a concept is a named set of requirements for template arguments
	// must appear in the SAME namespace as the template that uses it

	class Texture2D;
	class TransformComponent;

	// a static gameobject may not descended from a non static gameObject!
	// children may be non static

	class GameObject final
	{
	public:
		explicit GameObject(const GameObjectDescriptor& descriptor);
		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Destroy();
		bool IsAboutToBeDestroyed() const { return m_IsAboutToBeDestroyed; }
		void Update(float deltaT);
		void Render() const;

		bool Is(ObjectId id) const { if (m_Descriptor.id.has_value()) return m_Descriptor.id.value() == id; else return false; }

		//== SCENE GRAPH ==============================================================================================

		void SetParent(GameObject* parent, bool keepWorldPosition = false);
		void RemoveParent(bool keepWorldPosition = false) { SetParent(nullptr, keepWorldPosition); }
		GameObject* GetParent() const { return m_pParent; }
		const std::vector<GameObject*>& GetChildren() const { return m_pChildren; }

		//== COMPONENTS ===============================================================================================

		// arguments must match exactly the constructor of the component!!
		// passing an int when the constructor expects a float -> C4244
		template <Cpt C, typename... Args>
		C* AddComponent(Args&&... args)
		{
			for (const auto& component : m_Components)
			{
				if (typeid(component.get()) == typeid(C))
				{
					assert(false && "Cannot add 2 components of the same type");
					return nullptr;
				}
			}

			//https://en.cppreference.com/w/cpp/utility/forward.html
			auto component = std::make_unique<C>(this, std::forward<Args>(args)...);

			auto* compPtr = component.get();
			m_Components.emplace_back(std::move(component));

			return compPtr;
		}

		template <CCpt CC, typename ... Args>
		CC* AddCollisionComponent(Args&&... args)
		{
			// only allow 1 collision component per gameobject
			if (m_pCollisionComponent != nullptr)
			{
				throw std::logic_error("GameObject already has a collision component! Only 1 collision component per GameObject is allowed!");
			}

			auto collisionComp = std::make_unique<CC>(this, std::forward<Args>(args)...);
			auto CompPtr = collisionComp.get();
			m_Components.emplace_back(std::move(collisionComp));
			m_pCollisionComponent = CompPtr;
			return CompPtr;
		}

		template <Cpt C>
		bool RemoveComponent()
		{
			for (unsigned int i{}; i < m_Components.size(); ++i)
			{
				if (typeid(m_Components[i].get()) == typeid(C))
				{
					m_Components[i]->Destroy();
					return true;
				}
			}

			return false;
		}

		// returns a component pointer, nullptr if not found
		template <Cpt C>
		C* GetComponent() const
		{
			for (const auto& component : m_Components)
			{
				if (C* castComp = dynamic_cast<C*>(component.get()))
					return castComp;
			}

			return nullptr;
		}

		TransformComponent* GetTransform() const { return m_pTransform; }
		CollisionComponent* GetCollisionComponent() const { return m_pCollisionComponent; }

		// checks if a gameobject has a component of type C
		template <Cpt C>
		bool HasComponent() const
		{
			for (const auto& component : m_Components)
			{
				if (dynamic_cast<C*>(component.get()) != nullptr)
					return true;
			}
			return false;
		}

		// remove any component that has been marked for destruction
		void CleanUpComponents();

		void ShouldCleanUpComponents() { m_ShouldCleanUpComponents = true; }

	private:
		//== SCENE GRAPH ==============================================================================================

		bool IsChild(const GameObject* object) const;
		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);

		//== DATA MEMBERS =============================================================================================
		std::vector<std::unique_ptr<Component>> m_Components{};
		TransformComponent* m_pTransform = nullptr; // store transform pointer seperatly for quick look-ups, vector still owns it
		CollisionComponent* m_pCollisionComponent = nullptr; // store collision component pointer seperatly for quick look-ups, vector still owns it

		bool m_ShouldCleanUpComponents = false;
		bool m_IsAboutToBeDestroyed = false;

		GameObject* m_pParent = nullptr;
		std::vector<GameObject*> m_pChildren{};

		const GameObjectDescriptor m_Descriptor;
	};
}
