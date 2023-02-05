#include "LogManager.h"

#include <spdlog/sinks/stdout_color_sinks.h>

LogManager* LogManager::m_instance = nullptr;

LogManager::LogManager() {}

int LogManager::InternalInit()
{
    m_consoleLogger = spdlog::stdout_color_mt("SourEngine");
    m_consoleLogger->set_level(spdlog::level::trace);

    spdlog::set_pattern("%^[%T] [%L] %v%$");

    return 0;
}

LogManager::~LogManager() {}

int LogManager::Init()
{
    // Skip if already initialized
    if (m_instance != nullptr)
        return 1;

    m_instance = new LogManager();
    if (m_instance->InternalInit() != 0)
    {
        delete m_instance;
        return 1;
    }

    return 0;
}

std::shared_ptr<spdlog::logger> LogManager::GetConsoleLogger() { return m_instance->m_consoleLogger; }
