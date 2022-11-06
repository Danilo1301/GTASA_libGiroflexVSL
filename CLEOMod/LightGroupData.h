#pragma once

#include "pch.h"

#include "LightGroup.h"
#include "Pattern.h"
#include "PatternLoop.h"

class LightGroupData {
public:
	std::vector<Pattern*> patterns;

	PatternLoop* patternLoop = new PatternLoop();
	PatternLoop* stepLoop = new PatternLoop();
	LightGroup* lightGroup;
	int hVehicle;

	LightGroupData(LightGroup* lightGroup, int hVehicle)
	{
		this->lightGroup = lightGroup;
		this->hVehicle = hVehicle;
	}

	Pattern* GetCurrentPattern()
	{
		if (patternLoop->StepIndex > patterns.size() - 1)
		{
			std::cout << "Pattern index " << patternLoop->StepIndex << " not found" << std::endl;
			return NULL;
		}

		return patterns[patternLoop->StepIndex];
	}

	PatternStep* GetCurrenetStep()
	{
		return GetCurrentPattern()->steps[stepLoop->StepIndex];
	}

	bool GetPointIsEnabled(Point* point, int index)
	{
		//auto pattern = GetCurrentPattern();
		auto step = GetCurrenetStep();

		//point->pointPosition == ePointPosition::BY_INDEX

		if (index > step->data.size() - 1) return 0;

		return step->data[index] == 1;
	}
};