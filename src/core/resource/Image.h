#pragma once

#include "Resource.h"
#include "../util/Error.h"

class ResourceManager;

class Image : public Resource
{
public:
    enum Format
    {
        BW,
        RGB,
        RGBA
    };
private:
    friend class ResourceManager;

    int m_width, m_height;
    Format m_format;
    unsigned char* m_data;

    Image();
public:
    virtual ~Image();

    Error LoadFromFile(const std::string& path);
};
