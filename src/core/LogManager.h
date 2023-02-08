#pragma once

#include <memory>

#include <spdlog/spdlog.h>

class LogManager
{
private:
    static LogManager* m_instance;

    std::shared_ptr<spdlog::logger> m_consoleLogger;

    LogManager() = default;

    int InternalInit();
public:
    ~LogManager();

    inline static LogManager* GetInstance() { return m_instance; }

    static int Init();
    static void Shutdown();

    std::shared_ptr<spdlog::logger> GetConsoleLogger();
};

#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
