#include "Resource.h"

#include "../LogManager.h"

Resource::Resource() : m_path("blah")
{
    LOG_INFO("aodsjaosjas");
}

Resource::~Resource()
{
    LOG_INFO("gosgosgos");
}

unsigned int Resource::GetID() { return m_id; }
