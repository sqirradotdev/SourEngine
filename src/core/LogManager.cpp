#include "LogManager.h"

#include <spdlog/sinks/stdout_color_sinks.h>

MANAGER_DEFINITION(LogManager)

Error LogManager::InternalInit()
{
#ifdef NDEBUG
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::trace);
#endif
    m_consoleLogger = spdlog::stdout_color_mt("SourEngine");

    spdlog::set_pattern("%^[%T] [%L] %v%$");

    return OK;
}

LogManager::~LogManager() {}

std::shared_ptr<spdlog::logger> LogManager::GetConsoleLogger() { return m_consoleLogger; }
