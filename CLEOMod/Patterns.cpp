#include "Patterns.h"
#include "Log.h"

std::vector<Pattern*> Patterns::m_Patterns;
int Patterns::m_TimeBetweenPatterns = 5000;

Pattern* Patterns::CreatePattern()
{
	Pattern* pattern = new Pattern();
	m_Patterns.push_back(pattern);

	Log::file << "Patterns: Create pattern" << std::endl;

	return pattern;
}