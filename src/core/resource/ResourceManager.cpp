#include "ResourceManager.h"

#include "../LogManager.h"
#include "Resource.h"

ResourceManager* ResourceManager::m_instance = nullptr;

int ResourceManager::InternalInit()
{
    LOG_INFO("ResourceManager initializing...");

    m_nextId = 0;
    return 0;
}

ResourceManager::~ResourceManager()
{
    LOG_INFO("ResourceManager shutting down...");
    m_resources.clear();
}

int ResourceManager::Init()
{
    if (m_instance != nullptr)
        return 1;

    m_instance = new ResourceManager();
    if (m_instance->InternalInit() != 0)
    {
        delete m_instance;
        return 1;
    }

    return 0;
}

void ResourceManager::Shutdown()
{
    delete m_instance;
}
