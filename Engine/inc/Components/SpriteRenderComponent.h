#pragma once
#include "RenderComponent.h"
#include <string>
#include <ComponentDescriptors.h>
#include "../GeneralStructs.h"

namespace REC
{
	class Texture2D;
	struct FrameInfo;

	class SpriteRenderComponent : public RenderComponent
	{
	public:
		explicit SpriteRenderComponent(GameObject* owner, Texture2D* texture, uint16_t width = 0, uint16_t height = 0);
		explicit SpriteRenderComponent(GameObject* owner, const std::string& textureName, uint16_t width = 0, uint16_t height = 0);
		explicit SpriteRenderComponent(GameObject* owner, const SpriteDescriptor& descriptor);
		virtual ~SpriteRenderComponent() = default;

		SpriteRenderComponent(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent(SpriteRenderComponent&& other) = delete;
		SpriteRenderComponent& operator=(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent& operator=(SpriteRenderComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		virtual void Render() override;

		void SetTexture(Texture2D* texture);
		Texture2D* RequestTexture(const std::string& textureName);

		void SetFrame(const FrameInfo* info);
		FrameInfo* RequestFrameInfo(const std::string& file, const std::string& key);
	private:
		Texture2D* m_pTexture;
		SpriteDescriptor m_Descriptor;
		const FrameInfo* m_pFrameInfo;

		Rect GetSrcRect() const;
	};
}