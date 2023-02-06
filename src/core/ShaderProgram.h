#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class ShaderProgram
{
private:
    GLuint m_programID;
    bool m_programCompiled;
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
    ShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource);
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

    // function to set uniforms
    void SetUniform(const std::string& name, float value) const;
    void SetUniform(const std::string& name, float v0, float v1) const;
    void SetUniform(const std::string& name, glm::vec2& value) const;
    void SetUniform(const std::string& name, float v0, float v1, float v2) const;
    void SetUniform(const std::string& name, glm::vec3& value) const;
    void SetUniform(const std::string& name, glm::mat4& value) const;
};
