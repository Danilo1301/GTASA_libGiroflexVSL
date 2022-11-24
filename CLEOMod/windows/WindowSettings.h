#pragma once

#include "../pch.h"
#include "../menu/Menu.h"

class WindowSettings {
public:
	static bool EditScreenModeEnabled;

	static void Create(Window* parent);
	static void ToggleEditScreenPos(bool enabled);

	static void Update();
	static void Draw();
};