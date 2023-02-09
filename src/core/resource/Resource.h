#pragma once

#include <string>

class ResourceManager;

class Resource
{
protected:
    friend class ResourceManager;

    unsigned int m_id;
    std::string m_path;
public:
    Resource();
    virtual ~Resource();

    unsigned int GetID();
};
