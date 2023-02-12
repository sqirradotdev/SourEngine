#define SDL_MAIN_HANDLED

#include <iostream>
#include <memory>
#include <SDL.h>
#include <glad/glad.h>

#include "core/LogManager.h"
#include "core/resource/ResourceManager.h"
#include "core/resource/Resource.h"
#include "core/resource/Texture.h"
#include "core/render/RenderManager.h"

int main()
{
    LogManager::Instance();

    LOG_INFO("Welcome to SourEngine!");
#ifndef NDEBUG
    LOG_WARN("You are running a debug build, expect crashes and performance degrades :)");
#endif

    ResourceManager::Instance();
    RenderManager::Instance();

    {
        auto test = ResourceManager::Instance().CreateResource<Texture>();
    }

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
        RenderManager::Instance().DrawRect(700, 10, 200, 200, nullptr, 1.0f, 0.0f, 0.0f);
        RenderManager::Instance().DrawRect(120, 50, 300, 150, nullptr, 1.0f, 1.0f, 0.0f);
        RenderManager::Instance().DrawRect(300, 300, 200, 250, nullptr, 1.0f, 0.0f, 1.0f);
        RenderManager::Instance().DrawRect(600, 340, 160, 100, nullptr, 0.0f, 1.0f, 1.0f);
        RenderManager::Instance().End2D();
        RenderManager::Instance().Present();
    }

    LOG_INFO("Cleaning up...");

    SDL_Quit();

    return 0;
}
