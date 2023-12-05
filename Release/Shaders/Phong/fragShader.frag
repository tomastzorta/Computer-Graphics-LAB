#version 430 core

in vec3 eyeSpaceNormalV;
in vec3 eyeSpaceLightPosV;
in vec3 eyeSpaceVertPosV;

uniform vec3 lightColour = {1,1,1};
uniform vec3 emissiveColour = {0,0,0};
uniform vec3 ambientColour  = {0.1f,0.1f,0.2f};
uniform vec3 diffuseColour  = {0.8f,0.1f,0.1f};
uniform vec3 specularColour = {1.f,1.f,1.f};
uniform float shininess     = 50.0f;
uniform float alpha         = 1.0f;

out vec4 fragColour;

void main()
{
	vec3 lightDir = normalize( eyeSpaceLightPosV - eyeSpaceVertPosV );

	vec3 normal = normalize( eyeSpaceNormalV );
	vec3 eyeDir = normalize( -eyeSpaceVertPosV );
	vec3 halfVec = normalize(eyeDir + lightDir);
	
	vec3 diffuse = diffuseColour * max(0,dot(normal,lightDir));
	
	float facing = float(dot(normal, lightDir) > 0);

	vec3 specular = specularColour * lightColour * facing * pow(max(0, dot(normal, halfVec)), shininess);
	
	fragColour = vec4( emissiveColour + ambientColour + diffuse + specular, alpha);
}
