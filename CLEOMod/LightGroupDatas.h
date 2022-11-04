#pragma once

#include "pch.h"

#include "LightGroup.h"
#include "LightGroupData.h"

class LightGroupDatas {
public:
	static int m_ChangePatternTime;
	static std::map<LightGroup*, LightGroupData*> m_LightGroupDatas;

	static bool HasLightGroupData(LightGroup* lightGroup);
};