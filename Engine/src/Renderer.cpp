#include <stdexcept>
#include <cstring>
#include <iostream>
#include "Renderer.h"
#include "../inc/SceneManager.h"
#include "../inc/Scene.h"
#include "Texture2D.h"
#include "SDLHelpers.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

void REC::Renderer::Init(SDL_Window* window)
{
	m_pWindow = window;
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	m_pRenderer = SDL_CreateRenderer(window, nullptr);
	if (m_pRenderer == nullptr)
	{
		std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

#if __EMSCRIPTEN__
	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	io.IniFilename = NULL;
#endif

	ImGui_ImplSDL3_InitForSDLRenderer(window, m_pRenderer);
	ImGui_ImplSDLRenderer3_Init(m_pRenderer);
}

void REC::Renderer::Render(Scene* scene) const
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow(); // For demonstration purposes, do not keep this in your engine


	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_pRenderer);

	scene->Render();
	ImGui::Render();

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_pRenderer);
	SDL_RenderPresent(m_pRenderer);
}

void REC::Renderer::Destroy()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}

void REC::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	RenderTexture(texture, x, y, dst.w, dst.h);
}

void REC::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void REC::Renderer::RenderTexture(const Texture2D& texture, const Rect& textureRegion, const Rect& screenRegion) const
{
	if (!(textureRegion.width == 0 && textureRegion.height == 0))
	{
		SDL_FRect src = ToRect(textureRegion);
		SDL_FRect dst = ToRect(screenRegion);
		SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
	}
	else
	{
		RenderTexture(texture, screenRegion.x, screenRegion.y, screenRegion.width, screenRegion.height);
	}
}

void REC::Renderer::RenderLine(const Color& color, glm::vec2 start, glm::vec2 end) const
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderLine(m_pRenderer, start.x, start.y, end.x, end.y);
}

void REC::Renderer::RenderRect(const Color& color, const Rect& rect, bool fill) const
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_FRect sdl_rect = ToRect(rect);
	if (fill)
		SDL_RenderFillRect(m_pRenderer, &sdl_rect);
	else
		SDL_RenderRect(m_pRenderer, &sdl_rect);
}

bool REC::Renderer::RenderPlotWindow(const std::string& windowTitle, int& sampleCount, const std::vector<float>& plotData) const
{
	bool buttonPressed{ false };

	ImGui::Begin(windowTitle.c_str());

	ImGui::InputInt("# Samples", &sampleCount);
	if (ImGui::Button("Trash The Cache"))
	{
		buttonPressed = true;
	}
	
	ImGui::Separator();

	if (plotData.size() > 0)
		ImGui::PlotLines("##Plot", plotData.data(), int(plotData.size()),0,nullptr, 3.4028235E38F, 3.4028235E38F, {250, 100}, 4);

	ImGui::End();

	return buttonPressed;
}

void REC::Renderer::RenderPlotsWindow(const std::string& windowTitle, int& sampleCount, bool& button1, bool& button2, const std::vector<float>& plotData1, const std::vector<float>& plotData2)
{
	ImGui::Begin(windowTitle.c_str());

	ImGui::InputInt("# Samples", &sampleCount);
	button1 = ImGui::Button("Trash The Cache with gameObject");
	if (plotData1.size() > 0)
		ImGui::PlotLines("##PlotObj", plotData1.data(), int(plotData1.size()), 0, nullptr, 3.4028235E38F, 3.4028235E38F, { 250, 100 }, 4);

	ImGui::Separator();
	button2 = ImGui::Button("Trash The Cache with gameObjectAlt");
	if (plotData2.size() > 0)
		ImGui::PlotLines("##PlotObjAlt", plotData2.data(), int(plotData2.size()), 0, nullptr, 3.4028235E38F, 3.4028235E38F, { 250, 100 }, 4);
	
	ImGui::End();
}

SDL_Renderer* REC::Renderer::GetSDLRenderer() const { return m_pRenderer; }
