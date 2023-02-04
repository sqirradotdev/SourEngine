#pragma once

#include <SDL.h>

class RenderManager
{
private:
    static RenderManager* m_instance;

    SDL_Window* m_window;
    SDL_GLContext m_glContext;

    RenderManager();

    int InternalInit();
public:
    ~RenderManager();

    inline static RenderManager* GetInstance() { return m_instance; }

    static int Init();
    static void Shutdown();

    SDL_Window* GetWindow() const;
    SDL_GLContext GetGLContext() const;
};
