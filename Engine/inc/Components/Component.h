#pragma once

namespace REC
{
	class GameObject;

	// a base class for all components
	// should not be used directly
	class Component
	{
	protected:
		// enforce that only derived classes can create components!
		explicit Component(GameObject* owner);

	public:
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;
		
		virtual void Update(float deltaT) = 0;

		GameObject* GetOwner() const { return m_pOwner; }

		virtual void Destroy();
		virtual bool IsAboutToBeDestroyed() const { return m_IsAboutToBeDestroyed; }
		virtual bool CanRender() const { return false; }

	private:
		GameObject* m_pOwner = nullptr;
		bool m_IsAboutToBeDestroyed = false;
	};
}