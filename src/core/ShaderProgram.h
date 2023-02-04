#pragma once

#include <string>

#include <glad/glad.h>

class ShaderProgram
{
public:
    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    ShaderProgram();
    ~ShaderProgram();

    int Compile();

    GLuint GetProgramID() const;
private:
    GLuint m_programID;
    bool m_programCompiled;
};
