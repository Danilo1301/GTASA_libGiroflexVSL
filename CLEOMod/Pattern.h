#pragma once

#include "pch.h"

struct PatternStep {
	std::vector<int> data;
	int duration = 0;
};

class Pattern {
public:
	std::vector<PatternStep*> steps;

	PatternStep* AddStep(std::vector<int> data, int duration)
	{
		PatternStep* step = new PatternStep();
		step->data = data;
		step->duration = duration;

		steps.push_back(step);
		return step;
	}
};