#pragma once

#include <memory>
#include <unordered_map>

#include "../LogManager.h"

class Resource;

class ResourceManager
{
private:
    friend class Resource;

    static ResourceManager* m_instance;

    unsigned int m_nextId;
    std::unordered_map<unsigned int, std::shared_ptr<Resource>> m_resources;

    ResourceManager() = default;

    int InternalInit();
public:
    ~ResourceManager();

    inline static ResourceManager* GetInstance() { return m_instance; }

    static int Init();
    static void Shutdown();

    template <class T>
    std::shared_ptr<T> MakeResource();
};

#include "ResourceManager.tpp"
