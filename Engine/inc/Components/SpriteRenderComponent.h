#pragma once
#include "RenderComponent.h"
#include <string>
#include "../SpriteDescriptor.h"
#include "../GeneralStructs.h"

namespace REC
{
	class Texture2D;
	struct SpriteInfo;

	class SpriteRenderComponent : public RenderComponent
	{
	public:
		explicit SpriteRenderComponent(GameObject* owner, Texture2D* texture, uint16_t width = 0, uint16_t height = 0);
		explicit SpriteRenderComponent(GameObject* owner, const std::string& textureName, uint16_t width = 0, uint16_t height = 0);
		explicit SpriteRenderComponent(GameObject* owner, const std::string& textureName, const SpriteDescriptor& descriptor);
		explicit SpriteRenderComponent(GameObject* owner, Texture2D* texture, const SpriteDescriptor& descriptor);
		virtual ~SpriteRenderComponent() = default;

		SpriteRenderComponent(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent(SpriteRenderComponent&& other) = delete;
		SpriteRenderComponent& operator=(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent& operator=(SpriteRenderComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		virtual void Render() override;

		void SetTexture(Texture2D* texture);
		void RequestTexture(const std::string& textureName);

	private:
		Texture2D* m_pTexture;
		SpriteDescriptor m_Descriptor;
		const SpriteInfo* m_pSpriteInfo;

		void RequestSpriteInfo();
		Rect GetSrcRect() const;
	};
}