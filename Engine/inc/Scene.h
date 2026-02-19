#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"

namespace REC
{
	class Scene final
	{
	public:
		explicit Scene() = default;
		~Scene() = default;

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator= (const Scene&) = delete;
		Scene& operator= (const Scene&&) = delete;

		void Add(std::unique_ptr<GameObject> object);
		void RemoveAll();

		void Update(float deltaT);
		void Render() const;

	private:
		void RemoveMarkedObjects();

		std::vector<std::unique_ptr<GameObject>> m_objects{};
	};

}
