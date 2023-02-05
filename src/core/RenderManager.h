#pragma once

#include <vector>

#include <SDL.h>

#include "ShaderProgram.h"

/**
 * @brief Low level rendering manager.
*/
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

    /**
     * @brief Get the instance of the RenderManager.
     * @return The instance of the RenderManager.
    */
    inline static RenderManager* GetInstance() { return m_instance; }

    /**
     * @brief Initialize the RenderManager.
     * @return 0 if successful, 1 otherwise.
    */
    static int Init();

    /**
     * @brief Shutdown the RenderManager.
    */
    static void Shutdown();

    /**
     * @brief Get the SDL window handle.
     * @return The SDL window handle.
    */
    SDL_Window* GetWindow() const;

    /**
     * @brief Get the SDL OpenGL context handle.
     * @return The SDL OpenGL context handle.
    */
    SDL_GLContext GetGLContext() const;

    /**
     * @brief Draw the given VAO using the given shader program.
     * @param VAO The VAO to draw.
     * @param shaderProgramID The shader program to use.
     * @param count The number of vertices to draw.
    */
    void Draw(unsigned int VAO, unsigned int shaderProgramID, unsigned int count);
    
    /**
     * @brief Clear the screen.
    */
    void Clear();

    /**
     * @brief Swap the buffers and display the rendered scene.
     * @note This function should be called whenever the scene is ready to be displayed.
    */
    void Flush();

    ShaderProgram& GetDefaultShaderProgram() { return m_defaultShaderProgram; }
};
