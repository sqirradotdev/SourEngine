#pragma once

#include <string>

#include <glad/glad.h>

class ShaderProgram
{
public:
    /**
     * @brief The source code of the vertex shader.
    */
    std::string vertexShaderSource;

    /**
     * @brief The source code of the fragment shader.
    */
    std::string fragmentShaderSource;

    ShaderProgram();
    ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    ShaderProgram(std::string vertexShaderSource, std::string fragmentShaderSource);
    ~ShaderProgram();

    /**
     * @brief Compile the shader program.
     * @return 0 if successful, 1 otherwise.
     * @note This function will always fail when one of the shader source is empty.
    */
    int Compile();

    /**
     * @brief Get the OpenGL program ID.
     * @return The OpenGL program ID.
    */
    GLuint GetProgramID() const;
private:
    GLuint m_programID;
    bool m_programCompiled;
};
