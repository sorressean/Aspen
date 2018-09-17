/**
* Logging for Aspen
* Contains: classes and methods used for logs.
*/
#ifndef ASPEN_LOG_H
#include <memory>
#include <string>

#include "mud.h"

/**
*LogData is declared in log.cpp and prevents us from having to force everyone
* who includes log.h to include all kinds of boost headers as well.
*/
struct LogData;

enum class SeverityLevel
{
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

class Log
{
    static Log* _instance;
    std::unique_ptr<LogData> _logData;

    Log() = default;
    ~Log() = default;
    Log(const Log& what) = delete;
    Log& operator =(const Log what) = delete;
public:
    /**
    * Called to initialize the logger singleton instance.
    * Note: This does not call Setup, nor does
    * Setup call this. They need to be called at the creation of the application.
    */
    static void Initialize();
    /**
    * Frees up the instance pointer.
    */
    static void Release();
    /**
    * Retrieves a pointer to the instance.
    */
    static Log* Instance();
    /**
    * Sets up logging based on configuration values.
    */
    void Setup();
    /**
    * Writes the specified message to the log.
    */
    void Write(const std::string& message);
    /**
    * Writes the specified message with a severity level to the log.
    */
    void Write(SeverityLevel level, const std::string& message);
};

//utility functions to make logging easier.
inline void WriteLog(SeverityLevel level, const std::string& message)
{
    auto instance = Log::Instance();
    if (!instance)
        return;

    instance->Write(level, message);
}
inline void WriteLog(const std::string& message)
{
    WriteLog(SeverityLevel::Info, message);
}
#endif
