#pragma once

#include "../pch.h"

#include "../menu/Menu.h"

class WindowMain {
public:
	static Window* m_Window;
	static int m_ModelId;

	static void Create(int modelId);
	static void Remove();
};