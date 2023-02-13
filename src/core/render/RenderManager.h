#pragma once

#include <memory>
#include <SDL.h>
#include <glm/mat4x4.hpp>

#include "../Manager.h"
#include "../Error.h"
#include "ShaderProgram.h"
#include "../resource/Texture.h"

#define RECT_BATCH_LIMIT 512
#define BATCH_BUFFER_SIZE RECT_BATCH_LIMIT * 4 * 4

/**
 * @brief Low level rendering manager.
*/
class RenderManager
{
MANAGER_DECLARATION(RenderManager)
private:
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
    } m_batched2DState;

    GLuint m_currentShaderProgram;
    std::shared_ptr<Texture> m_currentTexture;

    bool SetupBatched2D();
public:
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
    void Draw(GLuint VAO, GLuint shaderProgramID, GLuint count);

    Error UseShader(const ShaderProgram& shader);
    Error UseTexture(std::shared_ptr<Texture> texture);

    void BeginBatched2D();
    void DrawBatchedRect(float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
    void EndBatched2D();

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
