#include "LightGroupDatas.h"

int LightGroupDatas::m_ChangePatternTime = 3000;
std::map<LightGroup*, LightGroupData*> LightGroupDatas::m_LightGroupDatas;

bool LightGroupDatas::HasLightGroupData(LightGroup* lightGroup)
{
	return m_LightGroupDatas.find(lightGroup) != m_LightGroupDatas.end();
}