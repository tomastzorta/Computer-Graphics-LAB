#include "ShaderManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Scene.h"

ShaderManager::ShaderManager()
{
    for (const auto& shader : m_shaderPrograms)
    {
        glDeleteProgram(shader.second);
    }
}

void ShaderManager::LoadShader(const std::string& a_shaderType, const std::string& a_vertexPath,const std::string& a_fragmentPath)
{
    const std::string vertexShaderFile = ReadShaderFile(a_vertexPath);
    const std::string fragmentShaderFile = ReadShaderFile(a_fragmentPath);

    const GLuint vertexShader = CompileShader(vertexShaderFile, GL_VERTEX_SHADER);
    const GLuint fragmentShader = CompileShader(fragmentShaderFile, GL_FRAGMENT_SHADER);

    const GLuint shaderProgram = LinkShaderProgram(vertexShader, fragmentShader);

    m_shaderPrograms[a_shaderType] = shaderProgram;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderManager::UseShader(const std::string& a_shaderType) const
{
    glUseProgram(m_shaderPrograms.at(a_shaderType)); // Use the shader program
}

void ShaderManager::SetUniform(const std::string& a_shaderType, const std::string& a_uniformName, const int a_value) const
{
    // Get the location of the uniform
    const GLuint shaderProgram = m_shaderPrograms.at(a_shaderType);
    const int uniformLocation = glGetUniformLocation(shaderProgram, a_uniformName.c_str());
    // Set the uniform
    glUniform1i(uniformLocation, a_value);
}

void ShaderManager::SetUniform(const std::string& a_shaderType, const std::string& a_uniformName, const float a_value) const
{
    //get the location of the uniform
    const GLuint shaderProgram = m_shaderPrograms.at(a_shaderType);
    const int uniform_location = glGetUniformLocation(shaderProgram, a_uniformName.c_str());
    //set the uniform
    glUniform1f(uniform_location, a_value);
}

void ShaderManager::SetUniform(const std::string& a_shaderType, const std::string& a_uniformName,const glm::vec3& a_value) const
{
    //get the location of the uniform
    const GLuint shaderProgram = m_shaderPrograms.at(a_shaderType);
    const int uniformLocation = glGetUniformLocation(shaderProgram, a_uniformName.c_str());
    //set the uniform
    glUniform3fv(uniformLocation, 1, glm::value_ptr(a_value));
}

void ShaderManager::SetUniform(const std::string& a_shaderType, const std::string& a_uniformName,const glm::mat4& a_value) const
{
    //get the location of the uniform
    const int uniformLocation = glGetUniformLocation(m_shaderPrograms.at(a_shaderType), a_uniformName.c_str());
    //set the uniform
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(a_value));
}

GLuint ShaderManager::CompileShader(const std::string& a_shaderSource, const GLenum a_shaderType)
{
    const GLuint shaderProgram = glCreateShader(a_shaderType); // Create a shader
    const char* sourceCStr = a_shaderSource.c_str(); // Get the shader source as a C string
    glShaderSource(shaderProgram, 1, &sourceCStr, nullptr); // Set the shader source
    glCompileShader(shaderProgram); // Compile the shader

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

GLuint ShaderManager::LinkShaderProgram(const GLuint a_vertexShader, const GLuint a_fragmentShader)
{
    const GLuint shaderProgram = glCreateProgram(); // Create a shader program
    glAttachShader(shaderProgram, a_vertexShader); // Attach the vertex shader
    glAttachShader(shaderProgram, a_fragmentShader); // Attach the fragment shader
    glLinkProgram(shaderProgram); // Link the shaders together

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
    std::ifstream shaderFile(a_filePath); // Open the shader file
    std::stringstream shaderStream; // Create a string stream

    shaderStream << shaderFile.rdbuf(); // Read the shader file into the string stream
    shaderFile.close(); // Close the file

    return shaderStream.str(); // Return the string stream as a string
}



