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
}

void Application::Event()
{
}

void Application::Render()
{
}

int Application::Run()
{
    Init();

    auto test = ResourceManager::Instance().LoadTexture("./res/textures/placeholder.png");
    RenderManager::Instance().UseTexture(test);

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        RenderManager::Instance().Clear();
        RenderManager::Instance().BeginBatched2D();
        RenderManager::Instance().DrawBatchedRect(700, 10, 200, 200, 1.0f, 0.0f, 0.0f);
        RenderManager::Instance().DrawBatchedRect(120, 50, 300, 150, 1.0f, 1.0f, 0.0f);
        RenderManager::Instance().DrawBatchedRect(300, 300, 200, 250, 1.0f, 0.0f, 1.0f);
        RenderManager::Instance().DrawBatchedRect(600, 340, 160, 100, 0.0f, 1.0f, 1.0f);
        RenderManager::Instance().EndBatched2D();
        RenderManager::Instance().Present();
    }

    SDL_Quit();

    return 0;
}
