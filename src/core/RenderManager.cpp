#include "RenderManager.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "LogManager.h"

static const char* defaultVertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

static const char* defaultFragmentShaderSource = R"(
#version 330 core

out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
)";

RenderManager* RenderManager::m_instance = nullptr;

RenderManager::RenderManager()
    : m_window(nullptr), m_batchBufferIndex(0), m_batchVAO(0), m_batchVBO(0) {}

int RenderManager::InternalInit()
{
    LOG_INFO("Initializing RenderManager...");

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
        return 1;

    m_window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (m_window == nullptr)
        return 1;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        return 1;

    m_defaultShaderProgram = ShaderProgram(defaultVertexShaderSource, defaultFragmentShaderSource);
    if (m_defaultShaderProgram.Compile() != 0)
        return 1;

    return 0;
}

int RenderManager::Setup2D()
{
    glGenVertexArrays(1, &m_batchVAO);
    glGenBuffers(1, &m_batchVBO);

    glBindVertexArray(m_batchVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_batchVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_batchBuffer), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //ebo
    glGenBuffers(1, &m_batchEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_batchEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_batchIndices), m_batchIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_orthoProjectionMatrix = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

    return 0;
}

RenderManager::~RenderManager()
{
    SDL_DestroyWindow(m_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

int RenderManager::Init()
{
    // Skip if already initialized
    if (m_instance != nullptr)
        return 1;

    m_instance = new RenderManager();
    if (m_instance->InternalInit() != 0)
    {
        delete m_instance;

        LOG_CRITICAL("Failed to initialize RenderManager!");
        return 1;
    }

    return 0;
}

void RenderManager::Shutdown()
{
    delete m_instance;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

SDL_Window *RenderManager::GetWindow() const { return m_window; }

SDL_GLContext RenderManager::GetGLContext() const { return SDL_GLContext(); }

void RenderManager::Draw(unsigned int VAO, unsigned int shaderProgramID, unsigned int count)
{
    glUseProgram(shaderProgramID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, count);
}

void RenderManager::DrawRect(float x, float y, float width, float height)
{
    float vertices[] = {
        x, y, 0.0f,
        x + width, y, 0.0f,
        x + width, y + height, 0.0f,
        x, y + height, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    for (int i = 0; i < 12; i++)
        m_batchBuffer[m_batchBufferIndex++] = vertices[i];

    unsigned int offset = m_batchBufferIndex;
    for (int i = 0; i < 6; i++)
        m_batchIndices[m_batchBufferIndex++] = indices[i] + offset;
}

void RenderManager::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderManager::Present()
{
    glBindVertexArray(m_batchVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_batchVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_batchBufferIndex * sizeof(float), m_batchBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_batchIndicesBufferIndex * sizeof(unsigned int), m_batchIndices);

    glUseProgram(m_defaultShaderProgram.GetProgramID());
    m_defaultShaderProgram.SetUniform("mvp", m_orthoProjectionMatrix);
    glDrawElements(GL_TRIANGLES, m_batchBufferIndex, GL_UNSIGNED_INT, 0);

    m_batchBufferIndex = 0;
    m_batchIndicesBufferIndex = 0;

    SDL_GL_SwapWindow(m_window);
}
