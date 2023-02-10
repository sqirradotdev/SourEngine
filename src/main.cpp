#define SDL_MAIN_HANDLED

#include <iostream>
#include <memory>
#include <SDL.h>
#include <glad/glad.h>

#include "core/LogManager.h"
#include "core/resource/ResourceManager.h"
#include "core/resource/Resource.h"
#include "core/resource/Image.h"
#include "core/render/RenderManager.h"

int main()
{
    LogManager::Instance();

    LOG_INFO("Welcome to SourEngine!");
#ifndef NDEBUG
    LOG_WARN("You are running a debug build, expect crashes and performance degrades :)");
#endif

    LOG_INFO("ResourceManager initializing...");
    ResourceManager::Instance();

    LOG_INFO("RenderManager initializing...");
    RenderManager::Instance();

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
        RenderManager::Instance().Begin2D();
        RenderManager::Instance().DrawRect(10, 10, 100, 100);
        RenderManager::Instance().DrawRect(10, 120, 120, 100);
        RenderManager::Instance().DrawRect(10, 230, 140, 100);
        RenderManager::Instance().DrawRect(10, 340, 160, 100);
        RenderManager::Instance().End2D();
        RenderManager::Instance().Present();
    }

    LOG_INFO("Cleaning up...");

    SDL_Quit();

    return 0;
}
