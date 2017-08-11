#include "ColorCorrectionTails.h"

double ColorCorrectionTails::GetLowThr()
{
	return m_lowThr;
}
void ColorCorrectionTails::SetLowThr(double value)
{
	ClampTo01(value);
	m_lowThr = value;
	Update();
}
double ColorCorrectionTails::GetMinLuma()
{
	return m_minLuma;
}
void ColorCorrectionTails::SetMinLuma(double value)
{
	ClampTo01(value);
	m_minLuma = value;
	Update();
}
double ColorCorrectionTails::GetHue() {
	return m_hue;
}
void   ColorCorrectionTails::SetHue(double value) {
	ClampTo01(value);
	m_hue = value;
	Update();
}
double ColorCorrectionTails::GetSaturation() {
	return m_saturation;
}
void   ColorCorrectionTails::SetSaturation(double value) {
	ClampTo01(value);
	m_saturation = value;
	Update();
}

void ColorCorrectionTails::ClampTo01(double& value) {
	if (value > 1.0) value = 1.0;
	if (value < 0.0) value = 0.0;
}

double ColorCorrectionTails::LumaCorrection(double value)
{
	if (value <= m_lowThr) return 0;
	if (m_lowThr  == 1) return 1;
	double a = (m_minLuma - 1) / (m_lowThr - 1);
	double b = 1 - a;
	return a * value + b;
}


void ColorCorrectionTails::Update()
{
	for (int i = 0; i <= MAX_VALUE; i++)
	{
		double luma = (double)i / MAX_VALUE;
		luma = LumaCorrection(luma);
		double alpha = 1;
		if (luma == 0) alpha = 0;
		colorsRGBA[i] = ColorCorrectionTails::HSVtoRGBA(m_hue, m_saturation, luma, alpha);
	}
}

uint32_t ColorCorrectionTails::HSVtoRGBA(double hue, double sat, double luma, double alpha)
{
	double r = 0, g = 0, b = 0;

	int Hi = (int)std::floor(hue * 6);
	double f = (hue * 6 - Hi);

	double p = luma * (1 - sat);
	double q = luma * (1 - f * sat);
	double t = luma * (1 - (1 - f) * sat);

	if (Hi == 0) { r = luma; g = t; b = p; };
	if (Hi == 1) { r = q; g = luma; b = p; };
	if (Hi == 2) { r = p; g = luma; b = t; };
	if (Hi == 3) { r = p; g = q; b = luma; };
	if (Hi == 4) { r = t; g = p; b = luma; };
	if (Hi == 5) { r = luma; g = p; b = q; };

	return  (uint32_t)std::round(alpha * 255) << 24 | (uint32_t)std::round(b * 255) << 16 | (uint32_t)std::round(g * 255) << 8 | (uint32_t)std::round(r * 255);
}

ColorCorrectionTails::ColorCorrectionTails(double low_thr, double min_luma, double hue, double saturation)
{
	m_lowThr = low_thr;
	m_hue = hue;
	m_saturation = saturation;
	m_minLuma = min_luma;
	Update();
}

ColorCorrectionTails::~ColorCorrectionTails()
{
}
