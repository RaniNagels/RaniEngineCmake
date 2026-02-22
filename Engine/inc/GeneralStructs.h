#pragma once
#include <glm/glm.hpp>

namespace REC
{
	// the pixel coordinates on a texture
	struct TextureRegion final
	{
		glm::vec2 topLeft{};
		glm::vec2 bottomRight{};

		float width() const { return bottomRight.x - topLeft.x; }
		float height() const { return bottomRight.y - topLeft.y; }

		TextureRegion() = default;
		TextureRegion(float x1, float y1, float x2, float y2)
			: topLeft{x1, y1}
			, bottomRight{x2, y2}
		{ }

		bool IsValid() const
		{
			if (bottomRight.x < topLeft.x || bottomRight.y < topLeft.y) return false;
			else if (topLeft.x == 0 && topLeft.y == 0 && bottomRight.x == 0 && bottomRight.y == 0) return false;
			else return true;
		}
	};

	struct Rect final
	{
		float x;
		float y;
		float width;
		float height;

		Rect() = default;
		Rect(float x, float y, float width, float height)
			:x{x}, y{y}, width{width}, height{height}
		{ }

		Rect(const TextureRegion& region)
			: x{region.topLeft.x}
			, y{region.topLeft.y}
			, width{region.width()}
			, height{region.height()}
		{ }

		Rect& operator=(const TextureRegion& region)
		{
			x = region.topLeft.x;
			y = region.topLeft.y;
			width = region.width();
			height = region.height();
			return *this;
		}
	};
}