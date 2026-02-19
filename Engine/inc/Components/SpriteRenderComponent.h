#pragma once
#include "RenderComponent.h"
#include <string>

namespace REC
{
	class Texture2D;

	class SpriteRenderComponent : public RenderComponent
	{
	public:
		explicit SpriteRenderComponent(GameObject* owner, Texture2D* texture, uint16_t width = 0, uint16_t height = 0);
		explicit SpriteRenderComponent(GameObject* owner, const std::string& textureName, uint16_t width = 0, uint16_t height = 0);
		virtual ~SpriteRenderComponent() = default;

		SpriteRenderComponent(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent(SpriteRenderComponent&& other) = delete;
		SpriteRenderComponent& operator=(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent& operator=(SpriteRenderComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		virtual void Render() override;

		void SetTexture(Texture2D* texture) { m_pTexture = texture; }
		void SetTexture(const std::string& textureName);

		void SetDrawSize(uint16_t width = 0, uint16_t height = 0);

	private:
		Texture2D* m_pTexture;
		uint16_t m_DrawWidth;
		uint16_t m_DrawHeight;
	};
}