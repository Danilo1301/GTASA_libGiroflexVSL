#pragma once

#include "../pch.h"

#include "../menu/Menu.h"

#include "../LightGroup.h"

class WindowLightGroups {
public:
	static void Create(Window* parent);

	static void CreateEditLightGroup(Window* parent, LightGroup* lightGroup);
	static void CreateEditPoints(Window* parent, LightGroup* lightGroup);
	static void CreateEditPoint(Window* parent, LightGroup* lightGroup, Point* point);
};