#include "stdafx.h"

#include "Help.h"

namespace RLGM 
{
	//Перевод номера линейки в радианы
	double LineToRadians(int line, int lineCount)
	{
		if (lineCount != 0)
			return (RLGM::M_PI / 2 - RLGM::M_PI * 2 * (line-0.5) / lineCount);
		else
			return 0;
	}
};