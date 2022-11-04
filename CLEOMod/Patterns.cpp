#include "Patterns.h"
#include "Log.h"

std::vector<Pattern*> Patterns::m_Patterns;

Pattern* Patterns::CreatePattern()
{
	Pattern* pattern = new Pattern();
	m_Patterns.push_back(pattern);

	std::cout << "Create pattern" << std::endl;
	Log::file << "Create pattern" << std::endl;

	return pattern;
}