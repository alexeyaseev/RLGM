#include <cstdint>
#include <vector>

#pragma once
class UnificationCoef
{
	const uint16_t MAX_VALUE = UINT16_MAX;
private:

	double m_startValue = MAX_VALUE;
	double m_stopValue = 1;

public:
	double GetStartValue();
	void   SetStartValue(double value);
	double GetStopValue();
	void   SetStopValue(double value);

	std::vector<uint16_t> m_coefs;
	
	void Update();
	void ClampTo01(double& value);

	UnificationCoef(int size, double start_value, double stop_value);
	~UnificationCoef();
};

