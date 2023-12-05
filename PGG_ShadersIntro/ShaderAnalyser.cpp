#include "ShaderAnalyser.h"
#include <iomanip>
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <fstream>

double ShaderAnalyser::MeasureShaderCompileTime(const GLenum a_shaderType, const std::string& a_shaderSource)
{
    const auto timerStart = std::chrono::high_resolution_clock::now();

    const GLuint shaderToCompile = ShaderManager::CompileShader(a_shaderSource, a_shaderType);

    const auto timerEnd = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> timerDuration = timerEnd - timerStart;

    glDeleteShader(shaderToCompile);

    return timerDuration.count(); // return time in seconds
}

void ShaderAnalyser::CompareShaders()
{
    std::ofstream analysisFile("ShaderAnalysis.txt");
    
    std::string phongVertexShaderFile = ShaderManager::ReadShaderFile("Shaders/Phong/vertShader.vert");
    std::string phongFragmentShaderFile = ShaderManager::ReadShaderFile("Shaders/Phong/fragShader.frag");
    std::string pbrVertexShaderFile = ShaderManager::ReadShaderFile("Shaders/PBR/PBR_vertShader.vert");
    std::string pbrFragmentShaderFile = ShaderManager::ReadShaderFile("Shaders/PBR/PBR_fragShader.frag");
    std::string disneyVertexShaderFile = ShaderManager::ReadShaderFile("Shaders/Disney_PBR/DPBR_vertShader.vert");
    std::string disneyFragmentShaderFile = ShaderManager::ReadShaderFile("Shaders/Disney_PBR/DPBR_fragShader.frag");

    double PhongVertexCompileTime = MeasureShaderCompileTime(GL_VERTEX_SHADER, phongVertexShaderFile);
    double PhongFragmentCompileTime = MeasureShaderCompileTime(GL_FRAGMENT_SHADER, phongFragmentShaderFile);

    double PBRVertexCompileTime = MeasureShaderCompileTime(GL_VERTEX_SHADER, pbrVertexShaderFile);
    double PBRFragmentCompileTime = MeasureShaderCompileTime(GL_FRAGMENT_SHADER, pbrFragmentShaderFile);

    double DisneyVertexCompileTime = MeasureShaderCompileTime(GL_VERTEX_SHADER, disneyVertexShaderFile);
    double DisneyFragmentCompileTime = MeasureShaderCompileTime(GL_FRAGMENT_SHADER, disneyFragmentShaderFile);

    analysisFile << std::fixed << std::setprecision(6);
    analysisFile << "Shader Compile Times" << std::endl;
    analysisFile << "Phong Vertex Shader took " << PhongVertexCompileTime << " seconds to compile" << std::endl;
    analysisFile << "Phong Fragment Shader took " << PhongFragmentCompileTime << " seconds to compile" << std::endl;
    analysisFile << "PBR Vertex Shader took " << PBRVertexCompileTime << " seconds to compile" << std::endl;
    analysisFile << "PBR Fragment Shader took " << PBRFragmentCompileTime << " seconds to compile" << std::endl;
    analysisFile << "Disney Vertex Shader took " << DisneyVertexCompileTime << " seconds to compile" << std::endl;
    analysisFile << "Disney Fragment Shader took " << DisneyFragmentCompileTime << " seconds to compile" << std::endl;
    analysisFile << std::endl;
    
    analysisFile.close();
}

