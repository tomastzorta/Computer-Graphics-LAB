#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    TexCoords = aTexCoords;
    vec4 worldPosition = modelMat * vec4(aPos, 1.0);
    WorldPos = worldPosition.xyz;
    Normal = mat3(transpose(inverse(modelMat))) * aNormal;
    gl_Position = projMat * viewMat * worldPosition;
}
