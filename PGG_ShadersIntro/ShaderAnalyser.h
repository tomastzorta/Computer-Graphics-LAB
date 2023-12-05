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

    static double MeasureShaderCompileTime(const GLenum a_shaderType, const std::string& a_shaderSource);

    static void CompareShaders();

private:
    ShaderManager& m_shaderManager;
};
