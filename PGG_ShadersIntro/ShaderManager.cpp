#include "ShaderManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <GLM/detail/type_mat4x4.hpp>
#include <GLM/detail/type_vec3.hpp>
#include <GLM/gtc/type_ptr.inl>

ShaderManager::ShaderManager()
{
    for (const auto& shader : m_shaderPrograms)
    {
        glDeleteProgram(shader.second);
    }
}

void ShaderManager::LoadShader(const std::string& a_shaderType, const std::string& a_vertexPath,
    const std::string& a_fragmentPath)
{
    const std::string vertex_shader_file = ReadShaderFile(a_vertexPath);
    const std::string fragment_shader_file = ReadShaderFile(a_fragmentPath);

    const GLuint vertex_shader = CompileShader(vertex_shader_file, GL_VERTEX_SHADER);
    const GLuint fragment_shader = CompileShader(fragment_shader_file, GL_FRAGMENT_SHADER);

    const GLuint shader_program = LinkProgram(vertex_shader, fragment_shader);

    m_shaderPrograms[a_shaderType] = shader_program;

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void ShaderManager::UseShader(const std::string& a_shaderType) const
{
    glUseProgram(m_shaderPrograms.at(a_shaderType));
    
}

/*void ShaderManager::SetUniform(const std::string& a_shaderType, const std::string& a_uniformName,
    const int a_value) const
{
    // Get the location of the uniform
    const int uniform_location = glGetUniformLocation(m_shaderPrograms.at(a_shaderType), a_uniformName.c_str());
    // Set the uniform
    glUniform1i(uniform_location, a_value);
}*/

void ShaderManager::SetUniform(const std::string& a_shaderType, const std::string& a_uniformName,
    const float a_value) const
{
    //get the location of the uniform
    const int uniform_location = glGetUniformLocation(m_shaderPrograms.at(a_shaderType), a_uniformName.c_str());
    //set the uniform
    glUniform1f(uniform_location, a_value);
}

void ShaderManager::SetUniform(const std::string& a_shaderType, const std::string& a_uniformName,
    const glm::vec3 a_value) const
{
    //get the location of the uniform
    GLuint shader_program = m_shaderPrograms.at(a_shaderType);
    const int uniform_location = glGetUniformLocation(shader_program, a_uniformName.c_str());
    //set the uniform
    glUniform3fv(uniform_location, 1, glm::value_ptr(a_value));
}

void ShaderManager::SetUniform(const std::string& a_shaderType, const std::string& a_uniformName,
    const glm::mat4 a_value) const
{
    //get the location of the uniform
    const int uniform_location = glGetUniformLocation(m_shaderPrograms.at(a_shaderType), a_uniformName.c_str());
    //set the uniform
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(a_value));
}

GLuint ShaderManager::CompileShader(const std::string& a_shaderSource, const GLenum a_shaderType)
{
    const GLuint shader_program = glCreateShader(a_shaderType);
    const char* source_c_str = a_shaderSource.c_str();
    glShaderSource(shader_program, 1, &source_c_str, nullptr);
    glCompileShader(shader_program);

    // Check for errors
    GLint successful;
    glGetShaderiv(shader_program, GL_COMPILE_STATUS, &successful);
    if (!successful) {
        char info_log[512];
        glGetShaderInfoLog(shader_program, 512, nullptr, info_log);
        std::cerr << "Error shading compilation failed\n" << info_log << std::endl;
    }

    return shader_program;
}

GLuint ShaderManager::LinkProgram(const GLuint a_vertexShader, const GLuint a_fragmentShader)
{
    const GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, a_vertexShader);
    glAttachShader(shader_program, a_fragmentShader);
    glLinkProgram(shader_program);

    // Check for linking errors
    GLint successful;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &successful);
    if (!successful) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
        std::cerr << "Error Shading Program Linking failed\n" << info_log << std::endl;
    }

    return shader_program;
}

std::string ShaderManager::ReadShaderFile(const std::string& a_filePath)
{
    std::ifstream shader_file(a_filePath);
    std::stringstream shader_stream;

    shader_stream << shader_file.rdbuf();
    shader_file.close();

    return shader_stream.str();
}



