#include "ResourceManager.h"

template <typename T, typename... Ts>
inline std::shared_ptr<T> ResourceManager::CreateResource(Ts... args)
{
    std::shared_ptr<T> resource(new T(args...));
    unsigned int id = m_nextId++;
    resource->m_id = id;
    m_resources[id] = std::static_pointer_cast<Resource>(resource);
    LOG_DEBUG("Created resource id {}", id);
    return resource;
}
