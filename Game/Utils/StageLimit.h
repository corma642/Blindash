#pragma once

namespace StageLimit
{
	inline float GetStageLimit(const int stageNumber)
	{
		switch (stageNumber)
		{
		case 1: return 10.0f;
		case 2: return 20.0f;
		case 3: return 35.0f;
		case 4: return 50.0f;
		}
		return 0.0f;
	}
}