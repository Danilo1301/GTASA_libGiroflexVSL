#pragma once

#include "../pch.h"
#include "../menu/Menu.h"

class WindowSoundPanelSettings {
public:
	static Window* m_Window;
	static bool m_RadioEnabled;

	static void Create();
	static void Remove();
};