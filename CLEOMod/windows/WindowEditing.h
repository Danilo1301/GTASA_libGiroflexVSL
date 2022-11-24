#pragma once

#include "../pch.h"
#include "../menu/Menu.h"
#include "../LightGroup.h"

class WindowEditing {
public:
	static bool FreezeLights;
	static bool ShowCurrentEditingLightGroup;
	static LightGroup* LightGroupToShow;

	static void Create(Window* parent, LightGroup* lightGroup);
};