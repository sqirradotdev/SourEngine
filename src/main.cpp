#define SDL_MAIN_HANDLED

#include <iostream>

#include <SDL.h>
#include <glad/glad.h>

#include "core/LogManager.h"
#include "core/render/RenderManager.h"

int main()
{
    if (LogManager::Init() != 0)
        return 1;

    LOG_INFO("Welcome to SourEngine!");
#ifndef NDEBUG
    LOG_WARN("You are running a debug build, expect crashes and performance degrades :)");
#endif

    if (SDL_Init(SDL_INIT_EVENTS) != 0)
    {
        LOG_CRITICAL("SDL_Init Error: {0}", SDL_GetError());
        return 1;
    }

    if (RenderManager::Init() != 0)
        return 1;

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        RenderManager::GetInstance()->Clear();
        RenderManager::GetInstance()->Begin2D();
        RenderManager::GetInstance()->DrawRect(10, 10, 100, 100);
        RenderManager::GetInstance()->DrawRect(10, 120, 120, 100);
        RenderManager::GetInstance()->DrawRect(10, 230, 140, 100);
        RenderManager::GetInstance()->DrawRect(10, 340, 160, 100);
        RenderManager::GetInstance()->End2D();
        RenderManager::GetInstance()->Present();
    }

    RenderManager::Shutdown();
    SDL_Quit();

    LOG_INFO("Goodbye!");

    return 0;
}
