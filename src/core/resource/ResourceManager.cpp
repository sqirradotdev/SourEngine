#include "ResourceManager.h"

#include "../LogManager.h"
#include "Resource.h"
#include "Texture.h"

void ResourceManager::Init()
{
    LOG_INFO("ResourceManager initializing...");

    m_nextId = 0;
}

ResourceManager::~ResourceManager()
{
    LOG_INFO("ResourceManager shutting down...");

    m_resources.clear();
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string &path)
{
    std::shared_ptr<Texture> texture = CreateResource<Texture>();
    if (texture->LoadFromFile(path) != OK)
        return std::shared_ptr<Texture>();

    return texture;
}
