#include "ResourceManager.h"

#include "../LogManager.h"
#include "Resource.h"

ResourceManager::ResourceManager()
{
    m_nextId = 0;
}

ResourceManager::~ResourceManager()
{
    m_resources.clear();
}
