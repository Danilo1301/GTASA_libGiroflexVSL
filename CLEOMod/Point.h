#pragma once

#include "pch.h"

enum ePointPosition
{
	LEFT,
	CENTER,
	RIGHT,
	BY_INDEX
};

class Point {
public:
	ePointPosition pointPosition = ePointPosition::BY_INDEX;
	CVector offset = { 0, 0, 0 };
	CRGBA color = { 255, 0, 0 };
};