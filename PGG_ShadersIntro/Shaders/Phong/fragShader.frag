#version 430 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// Phong material properties
uniform vec3 emissiveColour = vec3(0.0, 0.0, 0.0); // Emissive color
uniform vec3 ambientColour = vec3(0.1, 0.1, 0.1);
uniform vec3 diffuseColour = vec3(0.8, 0.1, 0.1);
uniform vec3 specularColour = vec3(1.0, 1.0, 1.0);
uniform float shininess = 50.0;
uniform float alpha = 1.0;

// Light properties
uniform vec3 lightPosition; // Assuming cube 1's position
uniform vec3 lightColor;    // Assuming cube 1's color

uniform vec3 camPos;  // Camera position

void main()
{
	vec3 lightDir = normalize(lightPosition - WorldPos);
	vec3 viewDir = normalize(camPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, Normal);

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularColour * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * diffuseColour + emissiveColour; // Add emissive color
	FragColor = vec4(result, alpha);
}
