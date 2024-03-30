#pragma once

#include "pch.h"

#include <vector>

class PatternLoop {
public:
	std::string Name = "";
	std::vector<int*> Steps;
	int StepIndex = 0;
	int Time = 0;
	int TotalTime = 0;
	bool StepChanged = false;
	bool DontChangeSteps = false;

	void AddStep(int* time) {
		Steps.push_back(time);
	}

	void Clear() {
		Steps.clear();
		Reset();
	}

	void ChangeStep() {
		
	}

	bool HasNoSteps()
	{
		return Steps.size() == 0;
	}

	bool HasStepChanged()
	{
		return StepChanged;
	}

	void SetStep(int index)
	{
		Reset();

		StepIndex = index;
		if (StepIndex >= (int)Steps.size())
		{
			StepIndex = 0;
		}
	}

	void Update(int time) {
		//Log::file << "[PatternLoop " << Name << "] begin Update, dt=" << time << ", steps=" << Steps.size() << std::endl;

		StepChanged = false;

		if (Steps.size() == 0) return;

		Time += time;
		TotalTime += time;

		//Log::file << "1" << std::endl;

		int maxtime = *Steps[StepIndex];

		//Log::file << "2" << std::endl;

		//bool stepChanged = false;

		if (maxtime > 0)
		{
			while (Time >= maxtime)
			{
				//Log::file << "3, " << Time << " / " << maxtime << std::endl;

				Time -= maxtime;

				if (!DontChangeSteps)
				{
					StepIndex++;
					StepChanged = true;

					if (StepIndex >= (int)Steps.size()) {
						StepIndex = 0;
						TotalTime = 0;
					}

					//Log::file << "4" << std::endl;

					maxtime = *Steps[StepIndex];
				}
			}
		}

		if (StepChanged) {
			Time = 0;
			//Log::file << "[PatternLoop " << Name << "] CHANGED step=" << StepIndex << "/" << Steps.size() << ", time " << Time << "" << *Steps[StepIndex] << std::endl;
		}

		//Log::file << "[PatternLoop " << Name << "] end Update" << std::endl;

		//Log::file << "[] step=" << m_StepIndex << " steps=" << m_Steps.size() << " time " << m_Time << std::endl;
	}

	void Reset() {
		StepIndex = 0;
		Time = 0;
		TotalTime = 0;
	}
};