#include "Application.h"

#include <SDL.h>

#include "LogManager.h"
#include "resource/ResourceManager.h"
#include "render/RenderManager.h"

void Application::Init()
{
    LogManager::Instance().Init();

    LOG_INFO("Welcome to SourEngine!");
#ifndef NDEBUG
    LOG_WARN("You are running a debug build, expect crashes and performance degrades :)");
#endif

    ResourceManager::Instance().Init();
    RenderManager::Instance().Init();

    m_running = true;
}

void Application::OnEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            Quit();
    }
}

void Application::OnRender()
{
    RenderManager::Instance().Clear();
    RenderManager::Instance().BeginBatched2D();
    RenderManager::Instance().DrawBatchedRect(700, 10, 200, 200, 1.0f, 0.0f, 0.0f);
    RenderManager::Instance().DrawBatchedRect(120, 50, 300, 150, 1.0f, 1.0f, 0.0f);
    RenderManager::Instance().DrawBatchedRect(300, 300, 200, 250, 1.0f, 0.0f, 1.0f);
    RenderManager::Instance().DrawBatchedRect(600, 340, 160, 100, 0.0f, 1.0f, 1.0f);
    RenderManager::Instance().EndBatched2D();
    RenderManager::Instance().Present();
}

int Application::Run()
{
    Init();

    auto test = ResourceManager::Instance().LoadTexture("./res/textures/testcard.qoi");
    RenderManager::Instance().UseTexture(test);

    while (m_running)
    {
        OnEvent();
        OnRender();
    }

    SDL_Quit();

    return 0;
}

void Application::Quit()
{
    m_running = false;
}
