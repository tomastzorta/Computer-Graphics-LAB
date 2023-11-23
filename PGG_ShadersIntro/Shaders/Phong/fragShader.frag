#version 430 core
// This is the fragment shader
// The program in this file will be run separately for each fragment (pixel) that is drawn

// These are the per-fragment inputs
// They must match with the outputs of the vertex shader
in vec3 eyeSpaceNormalV;
in vec3 eyeSpaceLightPosV;
in vec3 eyeSpaceVertPosV;

// These variables will be the same for every vertex in the model
// They are mostly material and light properties
// We provide default values in case the program doesn't set them
uniform vec3 lightColour = {1,1,1};
uniform vec3 emissiveColour = {0,0,0};
uniform vec3 ambientColour  = {0.1f,0.1f,0.2f};
uniform vec3 diffuseColour  = {0.8f,0.1f,0.1f};
uniform vec3 specularColour = {1.f,1.f,1.f};
uniform float shininess     = 50.0f;
uniform float alpha         = 1.0f;

// This is the output, it is the fragment's (pixel's) colour
out vec4 fragColour;

// The actual program, which will run on the graphics card
void main()
{
	// This is the direction from the fragment to the light, in eye-space
	vec3 lightDir = normalize( eyeSpaceLightPosV - eyeSpaceVertPosV );
	// Re-normalise the normal just in case
	vec3 normal = normalize( eyeSpaceNormalV );
	vec3 eyeDir = normalize( -eyeSpaceVertPosV );
	vec3 halfVec = normalize(eyeDir + lightDir);

	// This is where you need to write your code!
	vec3 diffuse = diffuseColour * max(0,dot(normal,lightDir));

	// This is where you need to write your code!
	float facing = float(dot(normal, lightDir) > 0);

	vec3 specular = specularColour * lightColour * facing * pow(max(0, dot(normal, halfVec)), shininess);

	// The final output colour is the emissive + ambient + diffuse + specular
	fragColour = vec4( emissiveColour + ambientColour + diffuse + specular, alpha);
}
