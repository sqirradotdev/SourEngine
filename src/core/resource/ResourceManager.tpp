#include "ResourceManager.h"

template <class T>
inline std::shared_ptr<T> ResourceManager::MakeResource()
{
    std::shared_ptr<Resource> resource = std::make_shared<T>();
    unsigned int id = m_nextId++;
    m_resources[id] = resource;
    resource->m_id = id;
    return resource;
}
