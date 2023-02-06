#pragma once

#include "pch.h"

struct PatternStep {
	std::vector<int> data;
	int duration = 0;
};

class Pattern {
public:
	std::string id = "";
	std::vector<PatternStep*> steps;

	Pattern(std::string id)
	{
		this->id = id;
	}

	PatternStep* AddStep(std::vector<int> data, int duration)
	{
		PatternStep* step = new PatternStep();
		step->data = data;
		step->duration = duration;

		steps.push_back(step);
		return step;
	}
};