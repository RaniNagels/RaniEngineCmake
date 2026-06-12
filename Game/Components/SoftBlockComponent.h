#pragma once
#include <Components/Component.h>
#include <Components/IListener.h>
#include <Events/Event.h>

namespace REC
{
	class GameObject;
	class Scene;
}

namespace Game
{
	class GridComponent;
	class SoftBlockComponent final : public REC::Component, public REC::IListener
	{
	public:
		explicit SoftBlockComponent(REC::GameObject* owner, REC::Scene* scene, uint8_t row, uint8_t col);
		virtual ~SoftBlockComponent();

		SoftBlockComponent(const SoftBlockComponent& other) = delete;
		SoftBlockComponent(SoftBlockComponent&& other) = delete;
		SoftBlockComponent& operator=(const SoftBlockComponent& other) = delete;
		SoftBlockComponent& operator=(SoftBlockComponent&& other) = delete;

		virtual void Notify(REC::Event* event) override;
		virtual void Update(float) override {}

	private:
		void CreateDoor(GridComponent* grid, uint8_t row, uint8_t col);
		void CreatePowerUp(GridComponent* grid, uint8_t row, uint8_t col);

		REC::Scene* m_pScene{ nullptr };
		uint8_t m_row{};
		uint8_t m_col{};
	};
}