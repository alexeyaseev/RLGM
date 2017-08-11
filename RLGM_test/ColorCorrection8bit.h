#pragma once
#include <cmath>
#include <cstdint>

class ColorCorrection8bit
{

private:
	static const int MAX_VALUE = 255;

	double m_lowThr = 0.0;
	double m_highThr = 1.0;
	double m_gamma = 0.5;
	double m_hue = 0.0;
	double m_saturation = 0.0;
	double m_minLuma = 0;
	double m_maxGamma = 1;
	double m_minGamma = 1;
	double m_alphaThr = 0;

	void Update();
	double LumaCorrection(double value);
	double LumaGammaCorrection(double value);
	void ClampTo01(double& value);

public:
	uint32_t colorsRGBA[ColorCorrection8bit::MAX_VALUE + 1];

	double GetLowThr();
	void   SetLowThr(double value);
	double GetHighThr();
	void   SetHighThr(double value);
	double GetGamma();
	double GetGammaPower();
	void   SetGamma(double value);
	double GetHue();
	void   SetHue(double value);
	double GetSaturation();
	void   SetSaturation(double value);
	double GetAlphaThr();
	void   SetAlphaThr(double value);

	static uint32_t HSVtoRGBA(double hue, double sat, double luma, double alpha);
	
	ColorCorrection8bit(double low_thr, double high_thr, double gamma, double hue, double saturation, double min_luma, double max_gamma, double alpha_thr);
	~ColorCorrection8bit();
};


