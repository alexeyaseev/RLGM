#include "ColorCorrection8bit.h"

double ColorCorrection8bit::GetLowThr()
{
	return m_lowThr;
}
void ColorCorrection8bit::SetLowThr(double value)
{
	ClampTo01(value);
	m_lowThr = value;
	Update();
}
double ColorCorrection8bit::GetHighThr()
{
	return m_highThr;
}
void ColorCorrection8bit::SetHighThr(double value)
{
	if (value < m_lowThr) value = m_lowThr;
	ClampTo01(value);
	m_highThr = value;
	Update();
}
double ColorCorrection8bit::GetGamma()
{
	return m_gamma;
}
void ColorCorrection8bit::SetGamma(double value)
{
	ClampTo01(value);
	m_gamma = value;
	Update();
}
double ColorCorrection8bit::GetGammaPower() {
	double res = 1.0 + (m_maxGamma - 1.0) * (m_gamma - 0.5) / 0.5; // gamma > 0.5
	if (m_gamma < 0.5) res = 1.0 + (1.0 - m_minGamma) * (m_gamma - 0.5) / 0.5;
	return res;
}
double ColorCorrection8bit::GetHue() {
	return m_hue;
}
void   ColorCorrection8bit::SetHue(double value) {
	ClampTo01(value);
	m_hue = value;
	Update();
}
double ColorCorrection8bit::GetSaturation() {
	return m_saturation;
}
void   ColorCorrection8bit::SetSaturation(double value) {
	ClampTo01(value);
	m_saturation = value;
	Update();
}

double ColorCorrection8bit::GetAlphaThr() {
	return m_alphaThr;
}
void   ColorCorrection8bit::SetAlphaThr(double value) {
	ClampTo01(value);
	m_alphaThr = value;
	Update();
}

void ColorCorrection8bit::ClampTo01(double& value) {
	if (value > 1.0) value = 1.0;
	if (value < 0.0) value = 0.0;
}

double ColorCorrection8bit::LumaCorrection(double value)
{
	if (value <= m_lowThr) return 0.0;
	if (value >= m_highThr) return 1.0;
	if (m_highThr == m_lowThr) return (1 + m_minLuma) / 2;
	double a = (1 - m_minLuma) / (m_highThr - m_lowThr);
	double b = 1 - a * m_highThr;
	return a * value + b;
}

double ColorCorrection8bit::LumaGammaCorrection(double value) {
	return std::pow(value, GetGammaPower());
}

void ColorCorrection8bit::Update()
{
	for (int i = 0; i < 256; i++)
	{
		double luma = i / 255.0;

		luma = LumaGammaCorrection(luma);
		luma = LumaCorrection(luma);
		
		//alpha correction version 1
		//double alpha = luma;
		
		//alpha correction version 3
		double alpha = 0;
		double par = 0.85;
		double a = 2 * par - 4 * m_alphaThr * par;
		double b = 4 * m_alphaThr * par - 2 * par + 1;
		double c = 0;
		alpha = a * luma * luma + b * luma + c;
		ClampTo01(alpha);

		//alpha correction version 2
		/*double alpha = 0;
		double par = 0.85;
		double f = (0.5 - m_alphaThr) * 2;
		double a, b, c;
		if (m_alphaThr > 0.5)
		{
			a = 2 * f - 4 * par - 4 * f*par + 2;
			b = 4 * par - f + 4 * f*par - 1;
			c = -f;
		}
		else if (m_alphaThr > 0)
		{
			a = -(2 * (2 * par - 1)) / ((f - 1)*(f - 1));
			b = -(3 * f - 4 * par - 4 * f*par + 1) / ((f - 1)*(f - 1));
			c = (f*(f - 4 * par + 1)) / (f * f - 2 * f + 1);
		}
		alpha = a * luma * luma + b * luma + c;
		ClampTo01(alpha);*/



		colorsRGBA[i] = ColorCorrection8bit::HSVtoRGBA(m_hue, m_saturation, luma, alpha);
	}
}

uint32_t ColorCorrection8bit::HSVtoRGBA(double hue, double sat, double luma, double alpha)
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

ColorCorrection8bit::ColorCorrection8bit(double low_thr, double high_thr, double gamma, double hue, double saturation, double min_luma, double max_gamma, double alpha_thr)
{
	m_lowThr = low_thr;
	m_highThr = high_thr;
	m_gamma = gamma;
	m_hue = hue;
	m_saturation = saturation;
	m_minLuma = min_luma;
	m_maxGamma = max_gamma;
	m_minGamma = 1 / m_maxGamma;
	m_alphaThr = alpha_thr;
	Update();
}

ColorCorrection8bit::~ColorCorrection8bit() 
{
}
