#include "ShaderManager.h"

#include <fstream>
#include <iostream>

ShaderManager::ShaderManager()
{
	
	shaderModelMatLocation = 0;
	shaderViewMatLocation = 0;
	shaderProjMatLocation = 0;

	shaderDiffuseColLocation = 0;
	shaderEmissiveColLocation = 0;
	shaderWsLightPosLocation = 0;
	
		//PBR
	shaderAlbedoLocation = 0;
	shaderMetallicLocation = 0;
	shaderRoughnessLocation = 0;
	shaderAoLocation = 0;

}

bool ShaderManager::CompileShaders(std::string vertFilename, std::string fragFilename)
{
	std::ifstream vertFile(vertFilename);
	char* vShaderText = NULL;

	if (vertFile.is_open())
	{
		// Find out how many characters are in the file
		vertFile.seekg(0, vertFile.end);
		int length = (int)vertFile.tellg();
		vertFile.seekg(0, vertFile.beg);

		// Create our buffer
		vShaderText = new char[length];

		// Transfer data from file to buffer
		vertFile.read(vShaderText, length);

		// Check it reached the end of the file
		if (!vertFile.eof())
		{
			vertFile.close();
			std::cerr << "WARNING: could not read vertex shader from file: " << vertFilename << std::endl;
			return true;
		}

		// Find out how many characters were actually read
		length = (int)vertFile.gcount();

		// Needs to be NULL-terminated
		vShaderText[length - 1] = 0;

		vertFile.close();
	}
	else
	{
		std::cerr << "WARNING: could not open vertex shader from file: " << vertFilename << std::endl;
		return true;
	}


	std::ifstream fragFile(fragFilename);
	char* fShaderText = NULL;

	if (fragFile.is_open())
	{
		// Find out how many characters are in the file
		fragFile.seekg(0, fragFile.end);
		int length = (int)fragFile.tellg();
		fragFile.seekg(0, fragFile.beg);

		// Create our buffer
		fShaderText = new char[length];

		// Transfer data from file to buffer
		fragFile.read(fShaderText, length);

		// Check it reached the end of the file
		if (!fragFile.eof())
		{
			fragFile.close();
			std::cerr << "WARNING: could not read fragment shader from file: " << fragFilename << std::endl;
			return true;
		}

		// Find out how many characters were actually read
		length = (int)fragFile.gcount();

		// Needs to be NULL-terminated
		fShaderText[length - 1] = 0;

		fragFile.close();
	}
	else
	{
		std::cerr << "WARNING: could not open fragment shader from file: " << fragFilename << std::endl;
		return true;
	}

	// The 'program' stores the shaders
	shaderProgram = glCreateProgram();

	// Create the vertex shader
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	// Give GL the source for it
	glShaderSource(vShader, 1, &vShaderText, NULL);
	// Compile the shader
	glCompileShader(vShader);
	// Check it compiled and give useful output if it didn't work!
	if (!CheckShaderCompiled(vShader))
	{
		std::cerr << "ERROR: failed to compile vertex shader" << std::endl;
		return true;
	}
	// This links the shader to the program
	glAttachShader(shaderProgram, vShader);

	// Same for the fragment shader
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderText, NULL);
	glCompileShader(fShader);
	if (!CheckShaderCompiled(fShader))
	{
		std::cerr << "ERROR: failed to compile fragment shader" << std::endl;
		return true;
	}
	glAttachShader(shaderProgram, fShader);

	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram(shaderProgram);
	// Check this worked
	GLint linked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(shaderProgram, len, &len, log);
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete [] log;

		return true;
	}
	return false;
}

GLint ShaderManager::GetUniformLocation(const std::string& a_name) const
{
	return __glewGetUniformLocation(shaderProgram, a_name.c_str());
}

void ShaderManager::SwitchShader(const std::string& a_shaderType)
{
	currentShaderType = a_shaderType;

	if (currentShaderType == "PBR")
	{
		CompileShaders("Shaders/PBR/PBR_vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
		PBRShaderLocations();
	}
	else if (currentShaderType == "PHONG")
	{
		CompileShaders("Shaders/Phong/vertShader.vert", "Shaders/Phong/fragShader.frag");
		PhongShaderLocations();
	}
}

bool ShaderManager::CheckShaderCompiled( GLint shader )
{
	GLint compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled )
	{
		GLsizei len;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

		// OpenGL will store an error message as a string that we can retrieve and print
		GLchar* log = new GLchar[len+1];
		glGetShaderInfoLog( shader, len, &len, log );
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete [] log;

		return false;
	}
	return true;
}

void ShaderManager::PBRShaderLocations()
{
	shaderAlbedoLocation = GetUniformLocation("albedo");
	shaderMetallicLocation = GetUniformLocation("metallic");
	shaderRoughnessLocation = GetUniformLocation("roughness");
	shaderAoLocation = GetUniformLocation("ao");
	shaderLightPosLocation = GetUniformLocation("lightPosition");
	shadeLightColLocation = GetUniformLocation("lightColor");
	shaderCamPosLocation = GetUniformLocation("camPos");
}

void ShaderManager::PhongShaderLocations()
{
	shaderDiffuseColLocation = GetUniformLocation("diffuseColour");
	shaderSpecularColLocation = GetUniformLocation("specularColour");
	shaderCubeShininessLocation = GetUniformLocation("shininess");
	shaderEmissiveColLocation = GetUniformLocation("emissiveColour");
	shaderWsLightPosLocation = GetUniformLocation("worldSpaceLightPos");
}

void ShaderManager::CameraShaderLocations()
{
	// SHARED BETWEEN PHONG AND PBR
	shaderModelMatLocation = GetUniformLocation("modelMat");
	shaderViewMatLocation = GetUniformLocation("viewMat");
	shaderProjMatLocation = GetUniformLocation("projMat");
}
