#include "Log.h"

std::fstream Log::logfile;
std::fstream Log::updateFile;
bool Log::deepLogEnabled = false;

LogFile logFile;

LogFile Log::Level(LOG_LEVEL level)
{
    logFile.level = level;

    auto time = FormatDate("%H:%M:%S");
    std::string date = "[" + std::string(time) + "]";

    if(level == LOG_LEVEL::LOG_BOTH || level == LOG_LEVEL::LOG_NORMAL)
    {
        logfile << date << " ";
    }

    bool logToUpdate = level == LOG_LEVEL::LOG_BOTH || level == LOG_LEVEL::LOG_UPDATE;
    if(level == LOG_LEVEL::LOG_DEEP_UPDATE && deepLogEnabled) logToUpdate = true;

    if(logToUpdate)
    {
        updateFile << date << " ";
    }

    return logFile;
}

void Log::Open(std::string folderPath, std::string fileName)
{
    logfile.open((folderPath + "/" + fileName + ".log"), std::fstream::out | std::fstream::trunc);
    updateFile.open((folderPath + "/" + fileName + "_update.log"), std::fstream::out | std::fstream::trunc);
}

/*
%d/%m
%d/%m/%Y
%H:%M:%S
*/
const char* Log::FormatDate(const char* format)
{
    std::time_t currentTime = std::time(nullptr);

    std::tm* localTime = std::localtime(&currentTime);
    
    char buffer[256];
    
    strftime(buffer, sizeof(buffer), format, localTime);

    return buffer;
}