#pragma once

#include <vector>

#include <SDL.h>

#include "ShaderProgram.h"

class RenderManager
{
private:
    static RenderManager* m_instance;

    SDL_Window* m_window;
    SDL_GLContext m_glContext;

    ShaderProgram m_defaultShaderProgram;

    RenderManager();

    int InternalInit();
public:
    ~RenderManager();

    inline static RenderManager* GetInstance() { return m_instance; }

    static int Init();
    static void Shutdown();

    SDL_Window* GetWindow() const;
    SDL_GLContext GetGLContext() const;

    void Draw(unsigned int VAO, unsigned int shaderProgramID, unsigned int count);
    void Clear();

    void Render();

    ShaderProgram& GetDefaultShaderProgram() { return m_defaultShaderProgram; }
};
