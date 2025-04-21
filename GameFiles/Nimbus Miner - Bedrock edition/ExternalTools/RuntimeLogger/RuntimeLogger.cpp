#include "RuntimeLogger.h"

#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

void RuntimeLogger::Log(const std::string& p_message, const char* p_filePath, const int p_line)
{
    std::string fullMessage = FormatMessage(p_message, p_filePath, p_line);
    
    AppendToFile(LOG_FILE_NAME, fullMessage);
}

void RuntimeLogger::ClearLogs()
{
    std::fstream textFile;

    textFile.open(LOG_FILE_NAME, std::fstream::app);

    textFile.clear();
}

std::string RuntimeLogger::FormatMessage(const std::string& p_message, const char* p_filePath, const int p_line)
{
    std::ostringstream oss;
    oss << "[" << GetTimeStamp() << "] ";
    oss << p_filePath << "(" << p_line << ") :\n" << p_message;

    return oss.str();
}

void RuntimeLogger::AppendToFile(const std::string& p_fileName, const std::string& p_message)
{
    std::fstream textFile;

    textFile.open(p_fileName, std::fstream::app);

    if (textFile.is_open())
    {
        textFile << p_message << "\n\n";
        textFile.close();
    }
}

std::string RuntimeLogger::GetTimeStamp()
{
    std::time_t now = std::time(nullptr);
    std::tm localTime;

#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}