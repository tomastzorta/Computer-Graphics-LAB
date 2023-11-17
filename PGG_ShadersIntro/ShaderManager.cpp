#include "ShaderManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

ShaderManager::ShaderManager()
{
    for (const auto& shader : m_shaderPrograms)
    {
        glDeleteProgram(shader.second);
    }
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::LoadShader(const std::string& a_shaderType, const std::string& a_vertexPath,
    const std::string& a_fragmentPath)
{
    const std::string vertexShaderFile = ReadShaderFile(a_vertexPath);
    const std::string fragmentShaderFile = ReadShaderFile(a_fragmentPath);

    const GLuint vertexShader = CompileShader(vertexShaderFile, GL_VERTEX_SHADER);
    const GLuint fragmentShader = CompileShader(fragmentShaderFile, GL_FRAGMENT_SHADER);

    const GLuint shaderProgram = LinkProgram(vertexShader, fragmentShader);

    m_shaderPrograms[a_shaderType] = shaderProgram;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderManager::UseShader(const std::string& a_shaderType) const
{
    glUseProgram(m_shaderPrograms.at(a_shaderType));
    
}

GLuint ShaderManager::GetUniform(const std::string& a_shaderType, const std::string& a_uniformName) const
{
    return glGetUniformLocation(m_shaderPrograms.at(a_shaderType), a_uniformName.c_str());
}

void ShaderManager::SharedUniformLocations(const std::string& a_shaderType)
{
    const GLuint shaderProgram = m_shaderPrograms[a_shaderType];
    m_uniformLocations[a_shaderType]["modelMat"] = glGetUniformLocation(shaderProgram, "modelMat");
    m_uniformLocations[a_shaderType]["viewMat"] = glGetUniformLocation(shaderProgram, "viewMat");
}

GLuint ShaderManager::CompileShader(const std::string& a_shaderSource, const GLenum a_shaderType)
{
    const GLuint shaderProgram = glCreateShader(a_shaderType);
    const char* sourceCStr = a_shaderSource.c_str();
    glShaderSource(shaderProgram, 1, &sourceCStr, nullptr);
    glCompileShader(shaderProgram);

    // Check for errors
    GLint successful;
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &successful);
    if (!successful) {
        char infoLog[512];
        glGetShaderInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Error shading compilation failed\n" << infoLog << std::endl;
    }

    return shaderProgram;
}

GLuint ShaderManager::LinkProgram(const GLuint a_vertexShader, const GLuint a_fragmentShader)
{
    const GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, a_vertexShader);
    glAttachShader(shaderProgram, a_fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    GLint successful;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successful);
    if (!successful) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Error Shading Program Linking failed\n" << infoLog << std::endl;
    }

    return shaderProgram;
}

std::string ShaderManager::ReadShaderFile(const std::string& a_filePath)
{
    std::ifstream shaderFile(a_filePath);
    std::stringstream shaderStream;

    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}



