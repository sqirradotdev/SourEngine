#define SDL_MAIN_HANDLED

#include <iostream>

#include <SDL.h>
#include <glad/glad.h>

#include "core/RenderManager.h"
#include "core/ShaderProgram.h"

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

// First out variable defines the color of the fragment
out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

int main()
{
    std::cout << "Hello!" << std::endl;

    if (SDL_Init(SDL_INIT_EVENTS) != 0)
    {
        std::cout << "Cannot initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (RenderManager::Init() != 0)
        return 1;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ShaderProgram shaderProgam;
    shaderProgam.vertexShaderSource = defaultVertexShaderSource;
    shaderProgam.fragmentShaderSource = defaultFragmentShaderSource;
    shaderProgam.Compile();

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        glUseProgram(shaderProgam.GetProgramID());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(RenderManager::GetInstance()->GetWindow());
    }

    RenderManager::Shutdown();
    SDL_Quit();

    std::cout << "Goodbye." << std::endl;

    return 0;
}
