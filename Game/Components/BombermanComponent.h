#pragma once
#include <Components/Component.h>

namespace Game
{
	class BombermanComponent final : public REC::Component
	{
	public:
		explicit BombermanComponent(REC::GameObject* owner);
		virtual ~BombermanComponent() = default;

		BombermanComponent(const BombermanComponent& other) = delete;
		BombermanComponent(BombermanComponent&& other) = delete;
		BombermanComponent& operator=(const BombermanComponent& other) = delete;
		BombermanComponent& operator=(BombermanComponent&& other) = delete;

		void SetPickupExtraBomb(bool has) { m_HasPickupExtraBomb = has; }
		void SetPickupFlames(bool has) { m_HasPickupFlames = has; }
		void SetPickupDetonator(bool has) { m_HasPickupDetonator = has; }

		bool HasPickupExtraBomb() const { return m_HasPickupExtraBomb; }
		bool HasPickupFlames() const { return m_HasPickupFlames; }
		bool HasPickupDetonator() const { return m_HasPickupDetonator; }

	private:
		bool m_HasPickupExtraBomb{ false };
		bool m_HasPickupFlames{ false };
		bool m_HasPickupDetonator{ false };
	};
}