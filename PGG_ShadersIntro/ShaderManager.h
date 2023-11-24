#pragma once

#include <string>
#include <unordered_map>
#include <GLM/detail/type_mat.hpp>
#include <GLM/detail/type_vec.hpp>

#include "glew.h"

class ShaderManager
{
public:
    ShaderManager();

    void LoadShader(const std::string& a_shaderType, const std::string& a_vertexPath, const std::string& a_fragmentPath);
    void UseShader(const std::string& a_shaderType) const;
    void SetUniform(const std::string& a_shaderType, const std::string& a_uniformName, int a_value) const;
    
    void SetUniform(const std::string& a_shaderType, const std::string& a_uniformName, const float a_value) const;
    void SetUniform(const ::std::string& a_shaderType, const ::std::string& a_uniformName, const glm::vec3& a_value) const;
    void SetUniform(const std::string& a_shaderType, const std::string& a_uniformName, const glm::mat4& a_value) const;
    void SetUniform(const std::string& a_shaderType, const std::string& a_uniformName, const glm::vec4& a_value) const;
    

private:
    std::unordered_map<std::string, GLuint> m_shaderPrograms;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> m_uniformLocations;
    static GLuint CompileShader(const std::string& a_shaderSource, GLenum a_shaderType);
    static GLuint LinkShaderProgram(GLuint a_vertexShader, GLuint a_fragmentShader);
    static std::string ReadShaderFile(const std::string& a_filePath);
};
