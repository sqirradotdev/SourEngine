#pragma once

#include <SDL.h>
#include <glm/mat4x4.hpp>

#include "ShaderProgram.h"

#define RECT_BATCH_LIMIT 512
#define BATCH_BUFFER_SIZE RECT_BATCH_LIMIT * 3 * 4

/**
 * @brief Low level rendering manager.
*/
class RenderManager
{
private:
    static RenderManager* m_instance;

    SDL_Window* m_window;
    SDL_GLContext m_glContext;

    struct
    {
        GLfloat batchBuffer[BATCH_BUFFER_SIZE];
        GLuint batchIndicesBuffer[RECT_BATCH_LIMIT * 6];
        ShaderProgram shaderProgram;
        glm::mat4 projectionMatrix;
        GLuint batchBufferIndex;
        GLuint batchIndicesBufferIndex;
        GLuint batchVAO;
        GLuint batchVBO;
        GLuint batchEBO;
    } m_renderState2D;

    RenderManager();

    int InternalInit();
    int Setup2D();
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

    void Begin2D();
    void DrawRect(float x, float y, float width, float height);
    void End2D();

    /**
     * @brief Clear the screen.
    */
    void Clear();

    /**
     * @brief Swap the buffers and display the rendered scene.
     * @note This function should be called whenever the scene is ready to be displayed.
    */
    void Present();
};