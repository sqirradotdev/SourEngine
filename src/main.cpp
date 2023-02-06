#define SDL_MAIN_HANDLED

#include <iostream>

#include <SDL.h>
#include <glad/glad.h>

#include "core/LogManager.h"
#include "core/RenderManager.h"
#include "core/ShaderProgram.h"

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

    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f,  0.5f, 0.0f
    // };

    // unsigned int VBO, VAO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);

    // glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

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
        //RenderManager::GetInstance()->Draw(VAO, RenderManager::GetInstance()->GetDefaultShaderProgram().GetProgramID(), 3);
        RenderManager::GetInstance()->DrawRect(10, 10, 100, 100);
        RenderManager::GetInstance()->DrawRect(10, 120, 100, 100);
        RenderManager::GetInstance()->DrawRect(10, 230, 100, 100);
        RenderManager::GetInstance()->DrawRect(10, 340, 100, 100);
        RenderManager::GetInstance()->Present();
    }

    RenderManager::Shutdown();
    SDL_Quit();

    LOG_INFO("Goodbye!");

    return 0;
}
