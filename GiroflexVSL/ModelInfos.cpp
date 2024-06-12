#include "ModelInfos.h"

#include "Log.h"

std::map<int, ModelInfo*> ModelInfos::m_ModelInfos;

ModelInfo* ModelInfos::CreateModelInfo(int modelId)
{
	ModelInfo* modelInfo = new ModelInfo(modelId);
	m_ModelInfos[modelId] = modelInfo;

	Log::Level(LOG_LEVEL::LOG_BOTH) << "ModelInfos: Create model info (id: " << modelId << ")" << std::endl;

	return modelInfo;
}

bool ModelInfos::HasModelInfo(int modelId)
{
	return m_ModelInfos.find(modelId) != m_ModelInfos.end();
}

ModelInfo* ModelInfos::GetModelInfo(int modelId)
{
	if (!HasModelInfo(modelId)) return NULL;
	return m_ModelInfos.at(modelId);
}

void ModelInfos::RemoveModelInfo(int modelId)
{
	if (!HasModelInfo(modelId)) return;

	delete m_ModelInfos[modelId];

	m_ModelInfos.erase(modelId);
}