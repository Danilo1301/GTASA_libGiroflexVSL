#pragma once

#include "pch.h"

#include "Pattern.h"

class Patterns {
public:
	static std::vector<Pattern*> m_Patterns;

	static Pattern* CreatePattern();
};