#include "Log.h"

std::fstream Log::file;

void Log::Open(const char* path)
{
    file.open(path, std::fstream::out | std::fstream::trunc);
}