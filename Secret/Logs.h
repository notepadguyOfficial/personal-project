#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <mutex>

enum class LOG_LEVEL {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logs
{
public:
    static Logs& GetInstance();
    void SetLogFile(const std::string& filename);
    void SetLogLevel(LOG_LEVEL level);
    void Print(const std::string& message, LOG_LEVEL level);
    void Debug(const std::string& message);
    void Info(const std::string& message);
    void Warning(const std::string& message);
    void Error(const std::string& message);
    void Initialized();

private:
    std::ofstream filestream_;
    std::mutex mutex_;
#if _DEBUG
    LOG_LEVEL LogLevel_ = LOG_LEVEL::DEBUG;
#else
    LOG_LEVEL LogLevel_ = LOG_LEVEL::INFO;
#endif
    Logs() = default;
    std::string GetCurrentTime();
    std::string FormatFileName();
    std::string LogLevelToString(LOG_LEVEL level);
    std::string FormatMessage(const std::string& message, LOG_LEVEL level);
};

extern Logs& Log;