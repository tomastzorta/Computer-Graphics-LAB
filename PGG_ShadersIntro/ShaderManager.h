#pragma once
#include <string>
#include "glew.h"


class ShaderManager
{
public:
    
    GLuint shaderProgram;
    bool CheckShaderCompiled( GLint shader );
    ShaderManager();
    bool CompileShaders(std::string vertFilename, std::string fragFilename);

    GLint GetUniformLocation(const std::string& a_name) const;

    void SwitchShader(const std::string& a_shaderType);
    
    void PBRShaderLocations();
    void PhongShaderLocations();
    void CameraShaderLocations();

    //Camera
    int shaderModelMatLocation;
    int shaderViewMatLocation;
    int shaderProjMatLocation;

    //Phong
    int shaderDiffuseColLocation, shaderEmissiveColLocation, shaderSpecularColLocation, shaderCubeShininessLocation;
    int shaderWsLightPosLocation;

    //PBR
    int shaderAlbedoLocation;
    int shaderMetallicLocation;
    int shaderRoughnessLocation;
    int shaderAoLocation;
    int shaderLightPosLocation;
    int shadeLightColLocation;
    int shaderCamPosLocation;

    std::string currentShaderType;
};
