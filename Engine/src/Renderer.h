#pragma once
#include <SDL3/SDL.h>
#include "../inc/RECColor.h"
#include "../inc/Singleton.h"
#include "../inc/SpriteDescriptor.h"
#include "../inc/GeneralStructs.h"

namespace REC
{
	class Texture2D;
	class Scene;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render(Scene* scene) const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, const Rect& textureRegion, const Rect& screenRegion) const;

		void RenderLine(const Color& color, glm::vec2 start, glm::vec2 end) const;
		void RenderRect(const Color& color, const Rect& rect, bool fill = false) const;

		bool RenderPlotWindow(const std::string& windowTitle, int& sampleCount, const std::vector<float>& plotData) const;
		void RenderPlotsWindow(const std::string& windowTitle, int& sampleCount, bool& button1, bool& button2, const std::vector<float>& plotData1, const std::vector<float>& plotData2);

		SDL_Renderer* GetSDLRenderer() const;

		const Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const Color& color) { m_ClearColor = color; }

	private:
		SDL_Renderer* m_pRenderer{};
		SDL_Window* m_pWindow{};
		Color m_ClearColor{};	
	};
}

