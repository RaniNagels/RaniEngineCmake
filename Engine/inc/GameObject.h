#pragma once
#include <string>
#include <memory>
#include "Components/Component.h"
#include <vector>
#include <assert.h>
#include <glm/glm.hpp>

namespace REC
{
	template <typename C>
	concept Cpt = std::derived_from<C, Component>;

	//https://en.cppreference.com/w/cpp/language/constraints.html
	// a concept is a named set of requirements for template arguments
	// must appear in the SAME namespace as the template that uses it

	class Texture2D;
	class TransformComponent;

	class GameObject final
	{
	public:
		explicit GameObject();
		explicit GameObject(float x, float y, float z = 0);
		explicit GameObject(glm::vec3 position);
		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Destroy();
		bool IsAboutToBeDestroyed() { return m_IsAboutToBeDestroyed; }
		void Update(float deltaT);
		void Render() const;

		void SetParent(GameObject* parent, bool keepWorldPosition = false);
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
		// dynamic_cast is slower than typeid
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

		// checks if a gameobject has a component of type C
		// typeid is faster than dynamic_cast
		template <Cpt C>
		bool HasComponent() const
		{
			for (const auto& component : m_Components)
			{
				if (typeid(component.get()) == typeid(C))
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

		bool m_ShouldCleanUpComponents = false;
		bool m_IsAboutToBeDestroyed = false;

		GameObject* m_pParent = nullptr;
		std::vector<GameObject*> m_pChildren{};
	};
}
