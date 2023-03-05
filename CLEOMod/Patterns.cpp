#include "Patterns.h"
#include "Log.h"

std::vector<Pattern*> Patterns::m_Patterns;
int Patterns::m_TimeBetweenPatterns = 7000;

Pattern* Patterns::CreatePattern(std::string id)
{
	Pattern* pattern = new Pattern(id);
	m_Patterns.push_back(pattern);

	Log::file << "Patterns: Create pattern" << std::endl;

	return pattern;
}