#include "RenderManager.h"

#include <iostream>
#include <glad/glad.h>

const char* defaultVertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char* defaultFragmentShaderSource = R"(
#version 330 core

out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
)";

RenderManager* RenderManager::m_instance = nullptr;

RenderManager::RenderManager() {}

int RenderManager::InternalInit()
{
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

        std::cout << "RenderManager init failed..." << std::endl;
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

void RenderManager::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderManager::Render()
{
    SDL_GL_SwapWindow(m_window);
}
