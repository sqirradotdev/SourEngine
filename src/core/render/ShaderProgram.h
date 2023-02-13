#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class RenderManager;

class ShaderProgram
{
private:
    friend class RenderManager;

    GLuint m_programID;
    bool m_programCompiled;

    std::unordered_map<std::string, int> m_uniformCache;
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
    ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
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

    GLuint GetUniformLocation(const std::string& name);

    // function to set uniforms
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, float v0, float v1);
    void SetUniform(const std::string& name, glm::vec2& value);
    void SetUniform(const std::string& name, float v0, float v1, float v2);
    void SetUniform(const std::string& name, glm::vec3& value);
    void SetUniform(const std::string& name, glm::mat4& value);
};
