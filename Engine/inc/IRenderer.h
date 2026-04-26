#pragma once
#include <glm/glm.hpp>
#include <RECColor.h>
#include <GeneralStructs.h>

namespace REC
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void RenderLine(const Color& color, glm::vec2 start, glm::vec2 end) const = 0;
		virtual void RenderRect(const Color& color, const Rect& rect, bool fill = false) const = 0;
		virtual void RenderCircle(const Color& color, glm::vec2 center, float radius) const = 0;
	};
}