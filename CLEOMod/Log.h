#pragma once

#include <fstream>

class Log {
public:
	static std::fstream file;
	static std::fstream opcodes;

	static void TestOpen(std::string path);
	static void OpenAtFolder(std::string path);
};
