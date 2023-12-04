#include "ShaderAnalyser.h"
#include <iomanip>
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <fstream>

double ShaderAnalyser::MeasureShaderCompileTime(const std::string& a_shaderType, const std::string& a_vertexPath, const std::string& a_fragmentPath) const
{
    
    auto start = std::chrono::high_resolution_clock::now();

    m_shaderManager.LoadShader(a_shaderType, a_vertexPath, a_fragmentPath);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    return elapsed.count();  // return time in seconds
}

double ShaderAnalyser::MeasureShaderLinkingTime(const std::string& a_shaderType) const
{
    auto startTime = std::chrono::high_resolution_clock::now();

    m_shaderManager.UseShader(a_shaderType);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;

    return duration.count();
}

size_t GetShaderMemoryUsage() 
{
    //found on microsoft documentation
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));
    const SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    return virtualMemUsedByMe;
}

void ShaderAnalyser::CompareShaders() const
{

    std::ofstream analysisFile("ShaderAnalysis.txt");
    
    size_t initialMemory = GetShaderMemoryUsage();
    double PhongTime = MeasureShaderCompileTime("Phong", "Shaders/Phong/vertShader.vert", "Shaders/Phong/fragShader.frag");
    double PhongLinkTime = MeasureShaderLinkingTime("Phong");
    size_t PhongMemory = (GetShaderMemoryUsage() - initialMemory) / 1024;

    double PBRTime = MeasureShaderCompileTime("PBR", "Shaders/PBR/PBR_vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
    double PBRLinkTime = MeasureShaderLinkingTime("PBR");
    size_t PBRMemory = (GetShaderMemoryUsage() - initialMemory - PhongMemory * 1024) / 1024;

    double DisneyTime = MeasureShaderCompileTime("Disney", "Shaders/Disney_PBR/DPBR_vertShader.vert", "Shaders/Disney_PBR/DPBR_fragShader.frag");
    double DisneyLinkTime = MeasureShaderLinkingTime("Disney");
    size_t DisneyMemory = (GetShaderMemoryUsage() - initialMemory - PhongMemory * 1024 - PBRMemory * 1024) / 1024;
    
    analysisFile << std::fixed << std::setprecision(6);
    analysisFile << "Shader Compile Times" << std::endl;
    analysisFile << "Phong Shader took " << PhongTime << " seconds to compile" << std::endl;
    analysisFile << "PBR Shader took " << PBRTime << " seconds to compile" << std::endl;
    analysisFile << "Disney Shader took " << DisneyTime << " seconds to compile" << std::endl;
    analysisFile << std::endl;
    analysisFile << "Shader Linking Times" << std::endl;
    analysisFile << "Phong Shader took " << PhongLinkTime << " seconds to link" << std::endl;
    analysisFile << "PBR Shader took " << PBRLinkTime << " seconds to link" << std::endl;
    analysisFile << "Disney Shader took " << DisneyLinkTime << " seconds to link" << std::endl;
    analysisFile << std::endl;
    analysisFile << "Memory Usage" << std::endl;
    analysisFile << "Phong Shader used " << PhongMemory << " kilobytes of memory" << std::endl;
    analysisFile << "PBR Shader used " << PBRMemory << " kilobytes of memory" << std::endl;
    analysisFile << "Disney Shader used " << DisneyMemory << " kilobytes of memory" << std::endl;
    analysisFile << std::endl;
    
    
}

