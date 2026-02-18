#pragma once
#include <cstdint>

namespace REC
{
	// an SDL independant color struct
	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		Color()
		{
			r = g = b = 0;
			a = 255;
		}
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
	};
}