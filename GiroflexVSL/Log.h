#pragma once

#include <fstream>

class Log {
public:
	static std::fstream file;

    static void Open(const char* path);
};