#include "Resource.h"

#include "../LogManager.h"

Resource::Resource() : m_path("") {}

Resource::~Resource() = default;

unsigned int Resource::GetID() { return m_id; }
