#version 430 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormalIn;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec4 worldSpaceLightPos = {1,0.0,1,1};

out vec3 eyeSpaceNormalV;
out vec3 eyeSpaceLightPosV;
out vec3 eyeSpaceVertPosV;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vPosition;
    
    eyeSpaceVertPosV = vec3(viewMat * modelMat * vPosition);
    eyeSpaceLightPosV = vec3(viewMat * worldSpaceLightPos);
    eyeSpaceNormalV = mat3(viewMat * modelMat) * vNormalIn;
}
