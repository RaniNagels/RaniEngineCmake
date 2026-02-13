#pragma once
#include "RenderComponent.h"
#include <string>

namespace REC
{
	class Texture2D;

	class SpriteRenderComponent : public RenderComponent
	{
	public:
		explicit SpriteRenderComponent(Texture2D* texture);
		explicit SpriteRenderComponent(const std::string& textureName);
		virtual ~SpriteRenderComponent() = default;

		SpriteRenderComponent(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent(SpriteRenderComponent&& other) = delete;
		SpriteRenderComponent& operator=(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent& operator=(SpriteRenderComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		virtual void Render() override;

		void SetTexture(Texture2D* texture) { m_pTexture = texture; }
		void SetTexture(const std::string& textureName);

	private:
		Texture2D* m_pTexture;
	};
}