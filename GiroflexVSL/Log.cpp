#include "Log.h"

std::fstream Log::logfile;
std::fstream Log::updateFile;

LogFile logFile;

LogFile Log::Level(LOG_LEVEL level)
{
    logFile.level = level;

    auto date = FormatDate();

    if(level == LOG_LEVEL::LOG_BOTH || level == LOG_LEVEL::LOG_NORMAL)
    {
        logfile << date << " ";
    }

     if(level == LOG_LEVEL::LOG_BOTH || level == LOG_LEVEL::LOG_UPDATE)
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

const char* Log::FormatDate()
{
    std::time_t currentTime = std::time(nullptr);

    std::tm* localTime = std::localtime(&currentTime);
    
    char buffer[256];
    
    strftime(buffer, sizeof(buffer), "[%d/%m %H:%M:%S]", localTime);

    return buffer;
}