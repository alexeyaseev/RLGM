#include "ColorCorrection2bit.h"

ColorCorrection2bit::ColorCorrection2bit(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4)
{
	colorsRGBA[0] = color1;
	colorsRGBA[1] = color2;
	colorsRGBA[2] = color3;
	colorsRGBA[3] = color4;
}

ColorCorrection2bit::~ColorCorrection2bit()
{
}
