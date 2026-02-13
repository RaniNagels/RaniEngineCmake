#pragma once
#include <string>
#include <memory>
#include "inc/Component.h"
#include <vector>
#include <assert.h>

namespace REC
{
	template <typename C>
	concept Cpt = std::derived_from<C, Component>;

	//https://en.cppreference.com/w/cpp/language/constraints.html
	// a concept is a named set of requirements for template arguments
	// must appear in the SAME namespace as the template that uses it

	class Texture2D;

	class GameObject final
	{
	public:
		explicit GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Update(float deltaT);
		virtual void Render() const;

		//== COMPONENTS ===============================================================================================

		// arguments must match exactly the constructor of the component!!
		// passing an int when the constructor expects a float -> C4244
		template <Cpt C, typename... Args>
		C* AddComponent(Args&&... args)
		{
			for (const auto& component : m_Components)
			{
#ifndef __EMSCRIPTEN__
				if (typeid(*component) == typeid(C))
#else
				if (dynamic_cast<C*>(component.get()) != nullptr)
#endif
					assert(false && "Cannot add 2 components of the same type");
			}

			//https://en.cppreference.com/w/cpp/utility/forward.html
			auto component = std::make_unique<C>(std::forward<Args>(args)...);

			component->SetOwner(this);
			auto* compPtr = component.get();
			m_Components.emplace_back(std::move(component));

			return compPtr;
		}

		template <Cpt C>
		bool RemoveComponent()
		{
			for (unsigned int i{}; i < m_Components.size(); ++i)
			{
#ifndef __EMSCRIPTEN__
				if (typeid(*m_Components[i]) == typeid(C))
#else
				if (dynamic_cast<C*>(m_Components[i].get()) != nullptr)
#endif
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

		// checks if a gameobject has a component of type C
		// typeid is faster than dynamic_cast
		template <Cpt C>
		bool HasComponent() const
		{
			for (const auto& component : m_Components)
			{
#ifndef __EMSCRIPTEN__
				if (typeid(*component) == typeid(C))
#else
				if (dynamic_cast<C*>(component.get()) != nullptr)
#endif
					return true;
			}
			return false;
		}

		// remove any component that has been marked for destruction
		void CleanUpComponents();

		void ShouldCleanUpComponents() { m_ShouldCleanUpComponents = true; }

	private:
		std::vector<std::unique_ptr<Component>> m_Components{};
		bool m_ShouldCleanUpComponents = false;
	};
}
