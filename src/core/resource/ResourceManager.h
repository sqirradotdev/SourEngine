#pragma once

#include <memory>
#include <unordered_map>
#include <utility>

#include "../Manager.h"
#include "../LogManager.h"
#include "Resource.h"

class Texture;

class ResourceManager
{
MANAGER_DECLARATION(ResourceManager)
private:
    friend class Resource;

    unsigned int m_nextId;
    std::unordered_map<unsigned int, std::shared_ptr<Resource>> m_resources;
public:
    template <typename T, typename... Ts>
    std::shared_ptr<T> CreateResource(Ts... args);

    std::shared_ptr<Texture> LoadTexture(const std::string& path);
};

#include "ResourceManager.tpp"
