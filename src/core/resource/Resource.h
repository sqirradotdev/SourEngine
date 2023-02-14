#pragma once

#include <string>

class ResourceManager;

class Resource
{
protected:
    friend class ResourceManager;

    unsigned int m_id;
    std::string m_path;

    Resource();
public:
    virtual ~Resource() = 0;

    unsigned int GetID();
};

#define RESOURCE_DECLARATION(Class) \
protected: \
    friend class ResourceManager; \
    Class(); \
public: \
    virtual ~Class();
