#pragma once

#include "pch.h"

#include <vector>

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

	void RemoveStep(PatternStep* step)
	{
		auto it = std::find(steps.begin(), steps.end(), step);
		if (it == steps.end()) return;
		steps.erase(it);
		delete step;
	}

	void RemoveAllSteps()
	{
		while (steps.size() > 0)
			RemoveStep(steps[0]);
	}

	Json::Value ToJSON()
	{
		Json::Value value = Json::objectValue;

		value["steps"] = Json::arrayValue;

		for (auto step : steps)
		{
			Json::Value stepValue = Json::objectValue;

			stepValue["duration"] = step->duration;

			stepValue["data"] = Json::arrayValue;

			for (auto d : step->data)
			{
				stepValue["data"].append(d);
			}

			value["steps"].append( stepValue );
		}

		return value;
	}

	void FromJSON(Json::Value value)
	{
		for (int step_i = 0; step_i < (int)value["steps"].size(); step_i++)
		{
			Json::Value step = value["steps"][step_i];

			int duration = step["duration"].asInt();

			std::vector<int> data;
			for (int val_i = 0; val_i < (int)step["data"].size(); val_i++)
			{
				data.push_back(step["data"][val_i].asInt());
			}

			AddStep(data, duration);
		}
	}
};