#include "Texture2D.h"

#include <SDL3/SDL.h>
#include <stdexcept>

#include "Renderer.h"

REC::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

REC::Texture2D::Texture2D(const std::string &fullPath, std::optional<REC::Color> transparentColor)
{
    SDL_Surface* surface = SDL_LoadPNG(fullPath.c_str());
    if (!surface)
    {
        throw std::runtime_error(
            std::string("Failed to load PNG: ") + SDL_GetError()
        );
    }

    if (transparentColor.has_value())
    {
        const SDL_PixelFormatDetails* formatDetails = SDL_GetPixelFormatDetails(surface->format);
        auto colorkey = SDL_MapRGB(formatDetails, nullptr, transparentColor.value().r, transparentColor.value().g, transparentColor.value().b);
        SDL_SetSurfaceColorKey(surface, true, colorkey);
    }

    m_texture = SDL_CreateTextureFromSurface(
        Renderer::GetInstance().GetSDLRenderer(),
        surface
    );

	SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

    SDL_DestroySurface(surface);

    if (!m_texture)
    {
        throw std::runtime_error(
            std::string("Failed to create texture from surface: ") + SDL_GetError()
        );
    }
}

REC::Texture2D::Texture2D(SDL_Texture* texture)
    : m_texture{ texture }
{
	assert(m_texture != nullptr);
}

glm::vec2 REC::Texture2D::GetSize() const
{
    float w{}, h{};
    SDL_GetTextureSize(m_texture, &w, &h);
    return { w, h };
}

SDL_Texture* REC::Texture2D::GetSDLTexture() const
{
    return m_texture;
}