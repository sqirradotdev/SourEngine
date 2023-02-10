#pragma once

#include <memory>
#include <unordered_map>

#include "../Manager.h"
#include "../LogManager.h"
#include "Resource.h"

class ResourceManager
{
MANAGER_DECLARATION(ResourceManager)
private:
    friend class Resource;

    unsigned int m_nextId;
    std::unordered_map<unsigned int, std::shared_ptr<Resource>> m_resources;
public:
    template <class T>
    std::shared_ptr<T> CreateResource();
};

#include "ResourceManager.tpp"
