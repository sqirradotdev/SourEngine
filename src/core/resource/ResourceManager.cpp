#include "ResourceManager.h"

#include "../LogManager.h"
#include "Resource.h"

MANAGER_DEFINITION(ResourceManager)

Error ResourceManager::InternalInit()
{
    m_nextId = 0;

    return OK;
}

ResourceManager::~ResourceManager()
{
    m_resources.clear();
}
