#pragma once

#include <string>
#include <glad/glad.h>

#include "Resource.h"
#include "../Error.h"

class RenderManager;

class Texture : public Resource
{
RESOURCE_DECLARATION(Texture)
public:
    enum Format
    {
        RGB,
        RGBA
    };
private:
    friend class RenderManager;

    unsigned int m_width;
    unsigned int m_height;
    Format m_format;
    GLuint m_textureID;

    bool m_initialized;
public:
    Error LoadFromFile(const std::string& path);
};
