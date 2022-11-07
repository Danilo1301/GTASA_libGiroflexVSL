#pragma once

#include "pch.h"
#include "Menu.h"
#include "LightGroup.h"

class WindowLightGroups {
public:
	static void Create(Window* parent);

	static void CreateEditLightGroup(Window* parent, LightGroup* lightGroup);
};