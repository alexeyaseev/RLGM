#pragma once
#include <cmath>
#include <cstdint>

class ColorCorrectionTails
{
private:
	static const int MAX_VALUE = 255;

	double m_lowThr = 1.0;
	double m_hue = 0.0;
	double m_saturation = 0.0;
	double m_minLuma = 0;

	void Update();
	double LumaCorrection(double value);
	void ClampTo01(double& value);

public:
	uint32_t colorsRGBA[MAX_VALUE + 1];

	double GetLowThr();
	void   SetLowThr(double value);
	double GetMinLuma();
	void   SetMinLuma(double value);
	double GetHue();
	void   SetHue(double value);
	double GetSaturation();
	void   SetSaturation(double value);

	static uint32_t HSVtoRGBA(double hue, double sat, double luma, double alpha);

	ColorCorrectionTails(double low_thr, double min_luma, double hue, double saturation);
	~ColorCorrectionTails();
};


