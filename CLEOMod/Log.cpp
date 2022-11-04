#include "Log.h"

std::fstream Log::file;
std::fstream Log::opcodes;

void Log::TestOpen(std::string path)
{
	std::fstream testFile;
	testFile.open(path.c_str(), std::fstream::out | std::fstream::trunc);
	testFile << "Tested" << std::endl;
	testFile.close();
}

void Log::OpenAtFolder(std::string path)
{
	char buffer[512];

	sprintf(buffer, "%s/giroflex.log", path.c_str());
	file.open(buffer, std::fstream::out | std::fstream::trunc);

	sprintf(buffer, "%s/giroflex_opcodes.log", path.c_str());
	opcodes.open(buffer, std::fstream::out | std::fstream::trunc);
}