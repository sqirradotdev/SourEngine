#include "Image.h"

#include <stb_image.h>

#include "../LogManager.h"

Image::Image() : m_width(0), m_height(0), m_data(nullptr)
{
}

Image::~Image()
{
    if (m_data != nullptr)
    {
        LOG_DEBUG("Deleting image data...");
        stbi_image_free(m_data);
    }
}

Error Image::LoadFromFile(const std::string &path)
{
    if (m_data != nullptr)
        stbi_image_free(m_data);

    int channels;
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &channels, 4);
    if (m_data == nullptr)
    {
        LOG_ERROR("Failed to load image from file: {}", path);
        return FAILED;
    }

    if (channels == 1)
        m_format = BW;
    else if (channels == 3)
        m_format = RGB;
    else if (channels == 4)
        m_format = RGBA;
    else
    {
        LOG_ERROR("Unsupported image format: {}", channels);
        return FAILED;
    }

    m_path = path;

    LOG_DEBUG("Successfully loaded image: {}", path);

    return OK;
}
