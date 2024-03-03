#pragma once

#include "pch.h"
#include "ModelInfo.h"

class ModelInfos {
public:
	static std::map<int, ModelInfo*> m_ModelInfos;

	static ModelInfo* CreateModelInfo(int modelId);
	static bool HasModelInfo(int modelId);
	static ModelInfo* GetModelInfo(int modelId);
	static void RemoveModelInfo(int modelId);
};