#include "RenderManager.h"

#include <iostream>
#include <exception>
#include <queue>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "../LogManager.h"

void RenderManager::Init()
{
    LOG_INFO("RenderManager initializing...");

    m_currentShaderProgram = 0;
    m_currentTexture = nullptr;

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

    if (!SetupBatched2D())
        throw "Failed to setup 2D rendering.";
}

RenderManager::~RenderManager()
{
    LOG_INFO("RenderManager shutting down...");

    SDL_DestroyWindow(m_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool RenderManager::SetupBatched2D()
{
    m_batched2DState.batchVBO = 0;
    m_batched2DState.batchVBO = 0;
    m_batched2DState.batchEBO = 0;
    m_batched2DState.batchBufferIndex = 0;
    m_batched2DState.batchIndicesBufferIndex = 0;

    m_batched2DState.shaderProgram.vertexShaderSource =

R"(#version 330 core

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

    m_batched2DState.shaderProgram.fragmentShaderSource =

R"(#version 330 core

in vec4 fragVColor;
in vec2 fragUV;

out vec4 fragColor;

uniform sampler2D u_texture;

void main()
{
    fragColor = texture(u_texture, fragUV);
}
)";

    m_batched2DState.shaderProgram.Compile();
    m_batched2DState.shaderProgram.SetUniform("u_texture", 0);

    glGenVertexArrays(1, &m_batched2DState.batchVAO);
    glGenBuffers(1, &m_batched2DState.batchVBO);

    glBindVertexArray(m_batched2DState.batchVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_batched2DState.batchVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_batched2DState.batchBuffer), nullptr, GL_DYNAMIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    // uv
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    glGenBuffers(1, &m_batched2DState.batchEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_batched2DState.batchEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_batched2DState.batchIndicesBuffer), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_batched2DState.projectionMatrix = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);

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

Error RenderManager::UseShader(const ShaderProgram& shader)
{
    if (!shader.m_programCompiled)
    {
        LOG_ERROR("Cannot use shader, not compiled!");
        return FAILED;
    }

    if (m_currentShaderProgram != shader.m_programID)
    {
        m_currentShaderProgram = shader.m_programID;
        glUseProgram(shader.m_programID);
    }

    return OK;
}

Error RenderManager::UseTexture(std::shared_ptr<Texture> texture)
{
    if (!texture->m_initialized)
    {
        LOG_ERROR("Cannot use Texture as it is not initialized!");
        return FAILED;
    }

    if (m_currentTexture != texture)
    {
        m_currentTexture = texture;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->m_textureID);
    }

    return OK;
}

void RenderManager::BeginBatched2D()
{
    m_batched2DState.batchBufferIndex = 0;
    m_batched2DState.batchIndicesBufferIndex = 0;
    UseShader(m_batched2DState.shaderProgram);
}

void RenderManager::DrawBatchedRect(float x, float y, float width, float height, float r, float g, float b, float a)
{
    if (m_batched2DState.batchBufferIndex + 32 > BATCH_BUFFER_SIZE)
    {
        EndBatched2D();
        BeginBatched2D();
    }

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
        m_batched2DState.batchBuffer[m_batched2DState.batchBufferIndex++] = buffer[i];

    GLuint offset = m_batched2DState.batchIndicesBufferIndex / 6 * 4;
    for (int i = 0; i < 6; i++)
        m_batched2DState.batchIndicesBuffer[m_batched2DState.batchIndicesBufferIndex++] = indices[i] + offset;
}

void RenderManager::EndBatched2D()
{
    glBindVertexArray(m_batched2DState.batchVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_batched2DState.batchVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_batched2DState.batchBufferIndex * sizeof(GLfloat), m_batched2DState.batchBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_batched2DState.batchIndicesBufferIndex * sizeof(GLuint), m_batched2DState.batchIndicesBuffer);

    m_batched2DState.shaderProgram.SetUniform("projection", m_batched2DState.projectionMatrix);
    glDrawElements(GL_TRIANGLES, m_batched2DState.batchIndicesBufferIndex, GL_UNSIGNED_INT, 0);
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
