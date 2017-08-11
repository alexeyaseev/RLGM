#include <cstdint>

#pragma once
class ColorCorrection2bit
{
public:
	uint32_t colorsRGBA[256];

	ColorCorrection2bit(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
	~ColorCorrection2bit();
};

