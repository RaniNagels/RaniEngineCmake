#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "inc/Component.h"

namespace REC
{
	template <typename C>
	concept Cpt = std::derived_from<C, Component>;

	class Texture2D;

	class GameObject
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

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		//template <Cpt C>
		//void AddComponent(std::unique_ptr<C> component)
		//{
		//
		//}

	private:
		Transform m_transform{};
		Texture2D* m_texture = nullptr;
	};
}
