#pragma once

#include <string>

#include "ResourceManager.h"

class Resource
{
private:
    friend class ResourceManager;

    unsigned int m_id;
    std::string m_path;
public:
    Resource();
    virtual ~Resource();

    unsigned int GetID();
};
