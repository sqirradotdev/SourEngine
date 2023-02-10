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
    if (LogManager::Init() != OK)
        return 1;

    LOG_INFO("Welcome to SourEngine!");
#ifndef NDEBUG
    LOG_WARN("You are running a debug build, expect crashes and performance degrades :)");
#endif

    LOG_INFO("SDL initializing...");
    if (SDL_Init(SDL_INIT_EVENTS) != 0)
    {
        LOG_CRITICAL("SDL_Init Error: {0}", SDL_GetError());
        return 1;
    }

    LOG_INFO("ResourceManager initializing...");
    if (ResourceManager::Init() != OK)
        return 1;

    {
        std::shared_ptr<Image> testImage = ResourceManager::GetInstance()->CreateResource<Image>();
        testImage->LoadFromFile("./res/textures/placeholder.png");
    }

    LOG_INFO("RenderManager initializing...");
    if (RenderManager::Init() != OK)
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

    LOG_INFO("RenderManager shutting down...");
    RenderManager::Shutdown();
    LOG_INFO("ResourceManager shutting down...");
    ResourceManager::Shutdown();
    LOG_INFO("SDL shutting down...");
    SDL_Quit();

    LOG_INFO("Goodbye!");
    LogManager::Shutdown();

    return 0;
}
