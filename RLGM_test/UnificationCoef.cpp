#include "UnificationCoef.h"



UnificationCoef::UnificationCoef(int size, double start_value, double stop_value)
{
	m_coefs.resize(size);
	ClampTo01(start_value);
	ClampTo01(stop_value);
	m_startValue = start_value ;
	m_stopValue = stop_value;
	Update();
}

UnificationCoef::~UnificationCoef()
{
}

void UnificationCoef::ClampTo01(double& value) {
	if (value > 1.0) value = 1.0;
	if (value < 0.0) value = 0.0;
}

double UnificationCoef::GetStartValue() {
	return m_startValue;
}
void UnificationCoef::SetStartValue(double value) {
	ClampTo01(value);
	m_startValue = value;
	Update();
}
double UnificationCoef::GetStopValue() {
	return m_stopValue;
}
void UnificationCoef::SetStopValue(double value) {
	ClampTo01(value);
	m_stopValue = value;
	Update();
}
void UnificationCoef::Update() {
	double a = (m_stopValue - m_startValue) / (m_coefs.size() - 1.0);
	for (int i = 0; i < m_coefs.size(); i++)
		m_coefs[i] = (uint16_t)round(MAX_VALUE * (a * i + m_startValue));
}

