#include "ResourceManager.h"

template <class T>
inline std::shared_ptr<T> ResourceManager::CreateResource()
{
    std::shared_ptr<T> resource(new T());
    unsigned int id = m_nextId++;
    m_resources[id] = std::static_pointer_cast<Resource>(resource);
    resource->m_id = id;
    return resource;
}
