#include "RenderManager.h"

#include <iostream>
#include <exception>
#include <queue>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "../LogManager.h"

RenderManager::RenderManager()
{
    LOG_INFO("RenderManager initializing...");

    m_window = nullptr;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
        throw "Failed to initialize SDL2 video subsystem.";

    m_window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    if (m_window == nullptr)
        throw "Failed to create SDL2 window.";

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        throw "Failed to initialize OpenGL context.";

    if (!Setup2D())
        throw "Failed to setup 2D rendering.";
}

RenderManager::~RenderManager()
{
    LOG_INFO("RenderManager shutting down...");

    SDL_DestroyWindow(m_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool RenderManager::Setup2D()
{
    m_rectBatch.batchVBO = 0;
    m_rectBatch.batchVBO = 0;
    m_rectBatch.batchEBO = 0;
    m_rectBatch.batchBufferIndex = 0;
    m_rectBatch.batchIndicesBufferIndex = 0;

    m_rectBatch.shaderProgram.vertexShaderSource =

R"(
#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec4 vertVColor;
layout (location = 2) in vec2 vertUV;

out vec4 fragVColor;
out vec2 fragUV;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(pos.x, pos.y, 0.0f, 1.0);
    fragVColor = vertVColor;
    fragUV = vertUV;
}
)";

    m_rectBatch.shaderProgram.fragmentShaderSource =

R"(
#version 330 core

in vec4 fragVColor;
in vec2 fragUV;

out vec4 fragColor;

uniform sampler2D texture;

void main()
{
    fragColor = fragVColor;
}
)";

    m_rectBatch.shaderProgram.Compile();

    glGenVertexArrays(1, &m_rectBatch.batchVAO);
    glGenBuffers(1, &m_rectBatch.batchVBO);

    glBindVertexArray(m_rectBatch.batchVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_rectBatch.batchVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_rectBatch.batchBuffer), nullptr, GL_DYNAMIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    // uv
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    glGenBuffers(1, &m_rectBatch.batchEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rectBatch.batchEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_rectBatch.batchIndicesBuffer), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_rectBatch.projectionMatrix = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);

    return true;
}

SDL_Window *RenderManager::GetWindow() const { return m_window; }

SDL_GLContext RenderManager::GetGLContext() const { return SDL_GLContext(); }

// TODO: make this useful again
void RenderManager::Draw(GLuint VAO, GLuint shaderProgramID, GLuint count)
{
    glUseProgram(shaderProgramID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, count);
}

void RenderManager::Begin2D()
{
    m_rectBatch.batchBufferIndex = 0;
    m_rectBatch.batchIndicesBufferIndex = 0;
}

void RenderManager::DrawRect(float x, float y, float width, float height, std::shared_ptr<Texture> texture, float r, float g, float b, float a)
{
    // position (xy), color (rgb), uv
    GLfloat buffer[] = {
        x        , y         , r, g, b, a, 0.0f, 0.0f,
        x + width, y         , r, g, b, a, 1.0f, 0.0f,
        x + width, y + height, r, g, b, a, 1.0f, 1.0f,
        x        , y + height, r, g, b, a, 0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    for (int i = 0; i < 32; i++)
        m_rectBatch.batchBuffer[m_rectBatch.batchBufferIndex++] = buffer[i];

    GLuint offset = m_rectBatch.batchIndicesBufferIndex / 6 * 4;
    for (int i = 0; i < 6; i++)
        m_rectBatch.batchIndicesBuffer[m_rectBatch.batchIndicesBufferIndex++] = indices[i] + offset;
}

void RenderManager::End2D()
{
    glBindVertexArray(m_rectBatch.batchVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_rectBatch.batchVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_rectBatch.batchBufferIndex * sizeof(GLfloat), m_rectBatch.batchBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_rectBatch.batchIndicesBufferIndex * sizeof(GLuint), m_rectBatch.batchIndicesBuffer);

    glUseProgram(m_rectBatch.shaderProgram.GetProgramID());
    m_rectBatch.shaderProgram.SetUniform("projection", m_rectBatch.projectionMatrix);
    glDrawElements(GL_TRIANGLES, m_rectBatch.batchIndicesBufferIndex, GL_UNSIGNED_INT, 0);
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
