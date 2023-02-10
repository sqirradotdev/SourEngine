#pragma once

#ifdef NDEBUG
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#else
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <memory>
#include <spdlog/spdlog.h>

#include "Manager.h"

class LogManager : public Manager<LogManager>
{
MANAGER_DECLARATION(LogManager)
private:
    std::shared_ptr<spdlog::logger> m_consoleLogger;
public:
    std::shared_ptr<spdlog::logger> GetConsoleLogger();
};

#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
