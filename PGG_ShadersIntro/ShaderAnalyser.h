#pragma once
#include "ShaderManager.h"
#include <chrono>
#include "Cube.h"

class ShaderAnalyser
{
public:
    ShaderAnalyser(ShaderManager& shaderManager) : m_shaderManager(shaderManager)
    {
        
    }
    
    double MeasureShaderCompileTime(const std::string& a_shaderType, const std::string& a_vertexPath, const std::string& a_fragmentPath) const;
    double MeasureShaderLinkingTime(const std::string& a_shaderType) const;

    double MeasureGPUTime(const std::string& a_shaderType);
    void CompareShaders() const;

private:
    ShaderManager& m_shaderManager;
};
