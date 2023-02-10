#include "RenderManager.h"

#include <iostream>
#include <queue>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "../LogManager.h"

MANAGER_DEFINITION(RenderManager)

Error RenderManager::InternalInit()
{
    m_window = nullptr;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
        return FAILED;

    m_window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    if (m_window == nullptr)
        return FAILED;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        return FAILED;

    if (!Setup2D())
        return FAILED;

    return OK;
}

bool RenderManager::Setup2D()
{
    m_renderState2D.batchVBO = 0;
    m_renderState2D.batchVBO = 0;
    m_renderState2D.batchEBO = 0;
    m_renderState2D.batchBufferIndex = 0;
    m_renderState2D.batchIndicesBufferIndex = 0;

    m_renderState2D.shaderProgram.vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
    )";
    m_renderState2D.shaderProgram.fragmentShaderSource = R"(
#version 330 core

out vec4 fragColor;

uniform sampler2D texture;

void main()
{
    fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
    )";
    m_renderState2D.shaderProgram.Compile();

    glGenVertexArrays(1, &m_renderState2D.batchVAO);
    glGenBuffers(1, &m_renderState2D.batchVBO);

    glBindVertexArray(m_renderState2D.batchVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_renderState2D.batchVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_renderState2D.batchBuffer), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glGenBuffers(1, &m_renderState2D.batchEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderState2D.batchEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_renderState2D.batchIndicesBuffer), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_renderState2D.projectionMatrix = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);

    return true;
}

RenderManager::~RenderManager()
{
    SDL_DestroyWindow(m_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

SDL_Window *RenderManager::GetWindow() const { return m_window; }

SDL_GLContext RenderManager::GetGLContext() const { return SDL_GLContext(); }

// TODO: make this useful again
void RenderManager::Draw(unsigned int VAO, unsigned int shaderProgramID, unsigned int count)
{
    glUseProgram(shaderProgramID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, count);
}

void RenderManager::Begin2D()
{
    m_renderState2D.batchBufferIndex = 0;
    m_renderState2D.batchIndicesBufferIndex = 0;
}

void RenderManager::DrawRect(float x, float y, float width, float height)
{
    float vertices[] = {
        x        , y         , 0.0f,
        x + width, y         , 0.0f,
        x + width, y + height, 0.0f,
        x        , y + height, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    for (int i = 0; i < 12; i++)
        m_renderState2D.batchBuffer[m_renderState2D.batchBufferIndex++] = vertices[i];

    unsigned int offset = m_renderState2D.batchIndicesBufferIndex / 6 * 4;
    for (int i = 0; i < 6; i++)
        m_renderState2D.batchIndicesBuffer[m_renderState2D.batchIndicesBufferIndex++] = indices[i] + offset;
}

void RenderManager::End2D()
{
    glBindVertexArray(m_renderState2D.batchVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderState2D.batchVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_renderState2D.batchBufferIndex * sizeof(GLfloat), m_renderState2D.batchBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_renderState2D.batchIndicesBufferIndex * sizeof(GLuint), m_renderState2D.batchIndicesBuffer);

    glUseProgram(m_renderState2D.shaderProgram.GetProgramID());
    m_renderState2D.shaderProgram.SetUniform("projection", m_renderState2D.projectionMatrix);
    glDrawElements(GL_TRIANGLES, m_renderState2D.batchIndicesBufferIndex, GL_UNSIGNED_INT, 0);
}

void RenderManager::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderManager::Present()
{
    SDL_GL_SwapWindow(m_window);
}
