#pragma once

#include <string>

/// <summary>
/// Write the given message inside a .txt file, the file is at the root of the project. 
/// <para> You can use the LOG macro to not have to put the 'p_filePath' and the 'p_line' parameters. </para> </summary>
class RuntimeLogger
{

public:

    static constexpr const char* LOG_FILE_NAME = "RuntimeWarnings.log";
    //static constexpr const char* LOG_FILE_PATH = "Logs"; // Put the file inside the given folder (starts from the .sln folder)
    
public:

    static void Log(const std::string& p_message, const char* p_filePath, const int p_line);
    static void ClearLogs();

private:
    
    static std::string FormatMessage(const std::string& p_message, const char* p_filePath, const int p_line);
    static void AppendToFile(const std::string& p_fileName, const std::string& p_message);
    static std::string GetTimeStamp();
    
};

/// <summary>
/// Write the given message insind a .txt file, the file is at the root of the project. </summary>
#define LOG(p_message) RuntimeLogger::Log(p_message, __FILE__, __LINE__)