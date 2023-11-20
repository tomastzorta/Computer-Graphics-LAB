
#include <fstream>
#include <string>

#include "Scene.h"



Scene::Scene()
{
	_cube1Angle = 0.0f;
	_cube2Angle = 0.0f;

	_cameraAngleX = 0.0f;
	_cameraAngleY = 0.0f;


	_shaderModelMatLocation = 0;
	_shaderViewMatLocation = 0;
	_shaderProjMatLocation = 0;

	_shaderDiffuseColLocation = 0;
	_shaderEmissiveColLocation = 0;
	_shaderWSLightPosLocation = 0;

	_cubeDiffuseColour = glm::vec3(1.0f, 0.0f, 0.0f);
	_cubeSpecularColour = glm::vec3(1.0f, 1.0f, 1.0f);
	_animateCentreCube = true;
	_animateLight = true;

	//PBR
	_shaderAlbedoLocation = 0;
	_shaderMetallicLocation = 0;
	_shaderRoughnessLocation = 0;
	_shaderAoLocation = 0;
	
	BuildShaders();
	
	//_modelMatrixCube1;
	_modelMatrixCube2 = glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(1.0f,0.0f,0.0f)),glm::vec3(0.1f,0.1f,0.1f));
	_modelMatrixCube3 = glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-1.0f,0.0f)),glm::vec3(2.0f,0.1f,2.0f));
	
	// Set up the viewing matrix
	// This represents the camera's orientation and position
	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-3.5f) );
	

	// Set up a projection matrix
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);


}

Scene::~Scene()
{
}

void Scene::Update( float deltaTs )
{
	if (_animateCentreCube)
	{
		_cube1Angle += deltaTs * 0.5f;
		// Limit the loop to 360 degrees
		// This will prevent numerical inaccuracies
		while( _cube1Angle > (3.14159265358979323846 * 2.0) )
		{
			_cube1Angle -= (3.14159265358979323846 * 2.0);
		}
	}

	if (_animateLight)
	{
		_cube2Angle += deltaTs * 2.0f;
		while (_cube2Angle > (3.14159265358979323846 * 2.0))
		{
			_cube2Angle -= (3.14159265358979323846 * 2.0);
		}
	}
	
	// Update the model matrix with the rotation of the object
	_modelMatrixCube1 = glm::rotate( glm::mat4(1.0f), _cube1Angle, glm::vec3(0,1,0) );
	_modelMatrixCube2 = glm::rotate(glm::mat4(1.0f), _cube2Angle, glm::vec3(0, 1, 0));
	_modelMatrixCube2 = glm::translate(_modelMatrixCube2, glm::vec3(1, 0, 0));
	_modelMatrixCube2 = glm::scale(_modelMatrixCube2, glm::vec3(0.1f, 0.1f, 0.1f));


}

void Scene::DrawCubePhong(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_diffuseColour,
	float a_cubeShininess, bool a_bIsLightSource)
{
	glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(a_modelMatrix));
	glUniform3fv(_shaderEmissiveColLocation, 1, glm::value_ptr(a_emissiveColour));
	glUniform3fv(_shaderDiffuseColLocation, 1, glm::value_ptr(a_diffuseColour));
	glUniform1f(_shaderCubeShininessLocation, a_cubeShininess);

	if (a_bIsLightSource)
	{
		glUniform3fv(_shaderSpecularColLocation, 1, glm::value_ptr(_cubeSpecularColour));
		glUniform4fv(_shaderWSLightPosLocation, 1, glm::value_ptr(a_modelMatrix * glm::vec4(0, 0, 0, 1)));
		glUniform1f(_shaderCubeShininessLocation, 0.1f);
	}
	_cubeModel.Draw();
}

void Scene::Draw()
{
		// Activate the shader program
	glUseProgram( _shaderProgram );

		// We use the small cube's model matrix to transform the light position
		// Send view and projection matrices to OpenGL
		// SHARED BETWEEN PHONG AND PBR
	CameraPositions();

	/////////////////////////// PBR /////////////////////////////////
	// PBRShaderLocations();
	// DrawCubePBR(_modelMatrixCube1, _cubeDiffuseColour, 0.7, 0.2, 0.5, false);
	// DrawCubePBR(_modelMatrixCube2, glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.1f, 0.1f, true);
	// DrawCubePBR(_modelMatrixCube3, glm::vec3(0.3f, 0.3f, 1.0f), 0.2f, 0.8f, 0.5f, false);

	/////////////////////////// PHONG /////////////////////////////////
	PhongShaderLocations();
	DrawCubePhong(_modelMatrixCube1, glm::vec3(0.0f, 0.0f, 0.0f), _cubeDiffuseColour, _cubeShininess, false);
	DrawCubePhong(_modelMatrixCube2, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,1.0f,1.0f), _cubeShininess, true);
	DrawCubePhong(_modelMatrixCube3, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), _cubeShininess, false);

	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}

bool Scene::CheckShaderCompiled( GLint shader )
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

void Scene::PBRShaderLocations()
{
	_shaderAlbedoLocation = glGetUniformLocation(_shaderProgram, "albedo");
	_shaderMetallicLocation = glGetUniformLocation(_shaderProgram, "metallic");
	_shaderRoughnessLocation = glGetUniformLocation(_shaderProgram, "roughness");
	_shaderAoLocation = glGetUniformLocation(_shaderProgram, "ao");
	_shaderLightPosLocation = glGetUniformLocation(_shaderProgram, "lightPosition");
	_shadeLightColLocation = glGetUniformLocation(_shaderProgram, "lightColor");
	_shaderCamPosLocation = glGetUniformLocation(_shaderProgram, "camPos");
}

void Scene::PhongShaderLocations()
{
	_shaderDiffuseColLocation = glGetUniformLocation( _shaderProgram, "diffuseColour" );
	_shaderSpecularColLocation = glGetUniformLocation(_shaderProgram, "specularColour");
	_shaderCubeShininessLocation = glGetUniformLocation(_shaderProgram, "shininess");
	_shaderEmissiveColLocation = glGetUniformLocation( _shaderProgram, "emissiveColour" );
	_shaderWSLightPosLocation = glGetUniformLocation( _shaderProgram, "worldSpaceLightPos" );
}

bool Scene::CompileShaders(std::string vertFilename, std::string fragFilename)
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
	_shaderProgram = glCreateProgram();

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
	glAttachShader(_shaderProgram, vShader);

	// Same for the fragment shader
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderText, NULL);
	glCompileShader(fShader);
	if (!CheckShaderCompiled(fShader))
	{
		std::cerr << "ERROR: failed to compile fragment shader" << std::endl;
		return true;
	}
	glAttachShader(_shaderProgram, fShader);

	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram(_shaderProgram);
	// Check this worked
	GLint linked;
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(_shaderProgram, len, &len, log);
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete [] log;

		return true;
	}
	return false;
}

void Scene::BuildShaders()
{
	
	CompileShaders("Shaders/Phong/vertShader.vert", "Shaders/Phong/fragShader.frag");
	//CompileShaders("Shaders/PBR/PBR_vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
	
	// We will define matrices which we will send to the shader
	// To do this we need to retrieve the locations of the shader's matrix uniform variables
	
	// SHARED BETWEEN PHONG AND PBR
	_shaderModelMatLocation = glGetUniformLocation( _shaderProgram, "modelMat" );
	_shaderViewMatLocation = glGetUniformLocation( _shaderProgram, "viewMat" );
	_shaderProjMatLocation = glGetUniformLocation( _shaderProgram, "projMat" );
}

void Scene::CameraPositions()
{
	glUniformMatrix4fv(_shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(_viewMatrix) );
	glUniformMatrix4fv(_shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(_projMatrix) );
	
	glm::vec3 camPos = glm::vec3(glm::inverse(_viewMatrix)[3]);
	glUniform3fv(_shaderCamPosLocation, 1, glm::value_ptr(camPos));
}

void Scene::DrawCubePBR(glm::mat4& a_modelMatrix, glm::vec3& a_albedo, float a_metallic, float a_roughness,float a_ao, bool a_bIsLightSource)
{
	
	glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(a_modelMatrix));
	glUniform3fv(_shaderAlbedoLocation, 1, glm::value_ptr(a_albedo)); // Variable color
	glUniform1f(_shaderMetallicLocation, a_metallic); // Adjust metallic property
	glUniform1f(_shaderRoughnessLocation, a_roughness); // Adjust roughness
	glUniform1f(_shaderAoLocation, a_ao);

	if (a_bIsLightSource)
	{
		glm::vec3 lightPosition = glm::vec3(a_modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glUniform3fv(_shaderLightPosLocation, 1, glm::value_ptr(lightPosition));
		glm::vec3 lightColour = glm::vec3(1.0f); // shed light colour
		glUniform3fv(_shadeLightColLocation, 1, glm::value_ptr(lightColour));
	}
	
	_cubeModel.Draw();
}
