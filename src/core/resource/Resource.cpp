#include "Resource.h"

#include "../LogManager.h"

Resource::Resource() : m_path("") {}

Resource::~Resource()
#ifdef NDEBUG
= default;
#else
{
    LOG_DEBUG("Deleted resource id {}", m_id);
}
#endif

unsigned int Resource::GetID() { return m_id; }
