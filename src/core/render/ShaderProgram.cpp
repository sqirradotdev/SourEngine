#include "ShaderProgram.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "../LogManager.h"

ShaderProgram::ShaderProgram() : m_programID(0), m_programCompiled(false) {}

ShaderProgram::ShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource)
    : vertexShaderSource(vertexShaderSource), fragmentShaderSource(fragmentShaderSource) {}

ShaderProgram::ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
    : vertexShaderSource(vertexShaderSource), fragmentShaderSource(fragmentShaderSource) {}

ShaderProgram::~ShaderProgram()
{
    if (m_programCompiled)
        glDeleteProgram(m_programID);
}

int ShaderProgram::Compile()
{
    if (vertexShaderSource.empty() || fragmentShaderSource.empty())
        return 1;

    const char* tmpPtr;
    int success;
    char infoLog[1024];

    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    tmpPtr = vertexShaderSource.c_str();
    glShaderSource(vertexShaderID, 1, &tmpPtr, nullptr);
    glCompileShader(vertexShaderID);
    
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderID, 1024, nullptr, infoLog);
        LOG_ERROR("Vertex shader compile failed!\n{0}", infoLog);
        glDeleteShader(vertexShaderID);
        return 1;
    }

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    tmpPtr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShaderID, 1, &tmpPtr, nullptr);
    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderID, 1024, nullptr, infoLog);
        LOG_ERROR("Fragment shader compile failed!\n{0}", infoLog);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        return 1;
    }

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShaderID);
    glAttachShader(m_programID, fragmentShaderID);
    glLinkProgram(m_programID);

    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_programID, 1024, nullptr, infoLog);
        LOG_ERROR("Program link failed!\n{0}", infoLog);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        glDeleteProgram(m_programID);
        m_programID = 0;
        return 1;
    }

    m_programCompiled = true;
    m_uniformCache.clear();

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return 0;
}

GLuint ShaderProgram::GetProgramID() const { return m_programID; }

GLuint ShaderProgram::GetUniformLocation(const std::string& name)
{
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end())
        return it->second;

    GLuint location = glGetUniformLocation(m_programID, name.c_str());
    m_uniformCache[name] = location;
    return location;
}

void ShaderProgram::SetUniform(const std::string &name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string &name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string& name, float v0, float v1)
{
    glUniform2f(GetUniformLocation(name), v0, v1);
}

void ShaderProgram::SetUniform(const std::string& name, glm::vec2& value)
{
    SetUniform(name, value.x, value.y);
}

void ShaderProgram::SetUniform(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void ShaderProgram::SetUniform(const std::string& name, glm::vec3& value)
{
    SetUniform(name, value.x, value.y, value.z);
}

void ShaderProgram::SetUniform(const std::string& name, glm::mat4& value)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}
