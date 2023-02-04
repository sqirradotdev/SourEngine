#include "RenderManager.h"

#include <iostream>
#include <glad/glad.h>

RenderManager* RenderManager::m_instance = nullptr;

RenderManager::RenderManager() {}

int RenderManager::InternalInit()
{
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
        return 1;

    m_window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (m_window == nullptr)
        return 1;

    SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        return 1;

    std::cout << "RenderManager initialized" << std::endl;

    return 0;
}

RenderManager::~RenderManager()
{
    SDL_DestroyWindow(m_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    std::cout << "RenderManager destroyed" << std::endl;
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
