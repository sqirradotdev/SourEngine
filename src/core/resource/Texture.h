#pragma once

#include <string>
#include <glad/glad.h>

#include "Resource.h"
#include "../Error.h"

class ResourceManager;

class Texture : public Resource
{
RESOURCE_DECLARATION(Texture)
public:
    enum Format
    {
        BW,
        RGB,
        RGBA
    };
private:
    friend class ResourceManager;

    unsigned int m_width;
    unsigned int m_height;
    Format m_format;
    GLuint m_textureHandle;

    bool m_initialized;
public:
    Error LoadFromFile(const std::string& path);
};
