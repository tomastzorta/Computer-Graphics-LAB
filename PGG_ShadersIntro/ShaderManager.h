#pragma once

#include <string>
#include <unordered_map>

#include "glew.h"

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    void LoadShader(const std::string& a_shaderType, const std::string& a_vertexPath, const std::string& a_fragmentPath);
    void UseShader(const std::string& a_shaderType) const;
    
    GLuint GetUniform(const std::string& a_shaderType, const std::string& a_uniformName) const;
    void SharedUniformLocations(const std::string& a_shaderType);

private:
    std::unordered_map<std::string, GLuint> m_shaderPrograms;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> m_uniformLocations;
    static GLuint CompileShader(const std::string& a_shaderSource, GLenum a_shaderType);
    static GLuint LinkProgram(GLuint a_vertexShader, GLuint a_fragmentShader);
    static std::string ReadShaderFile(const std::string& a_filePath);
};
