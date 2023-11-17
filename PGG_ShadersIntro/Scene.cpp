
#include <iostream>
#include <fstream>
#include <string>

#include "Scene.h"



Scene::Scene()
{
	_currentShader = "PBR";
	
	Startup();
}

Scene::~Scene()
{
}

void Scene::Startup()
{
	_shaderManager.LoadShader("PBR", "Shaders/vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
	_shaderManager.LoadShader("Phong", "Shaders/vertShader.vert", "Shaders/Phong/fragShader.frag");
	
	_cube1Angle = 0.0f;
	_cube2Angle = 0.0f;

	_cameraAngleX = 0.0f, _cameraAngleY = 0.0f;
	
	_cubeDiffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
	_cubeSpecularColour = glm::vec3(1.0f, 1.0f, 1.0f);
	_cubeShininess = 1.0f;
	_animateCentreCube = true;
	_animateLight = true;
	
	//_modelMatrixCube1;
	_modelMatrixCube2 = glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(1.0f,0.0f,0.0f)),glm::vec3(0.1f,0.1f,0.1f));
	_modelMatrixCube3 = glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-1.0f,0.0f)),glm::vec3(2.0f,0.1f,2.0f));
	
	// Set up the viewing matrix
	// This represents the camera's orientation and position
	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-3.5f) );
	

	// Set up a projection matrix
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	
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

	AnimationMatrices();
}

void Scene::AnimationMatrices()
{
	// Update the model matrix with the rotation of the object
	_modelMatrixCube1 = glm::rotate( glm::mat4(1.0f), _cube1Angle, glm::vec3(0,1,0) );
	_modelMatrixCube2 = glm::rotate(glm::mat4(1.0f), _cube2Angle, glm::vec3(0, 1, 0));
	_modelMatrixCube2 = glm::translate(_modelMatrixCube2, glm::vec3(1, 0, 0));
	_modelMatrixCube2 = glm::scale(_modelMatrixCube2, glm::vec3(0.1f, 0.1f, 0.1f));
}

void Scene::DrawCubePBR(const glm::mat4& a_modelMatrix, const glm::vec3& a_colour, 
						bool a_isLightSource, float a_metallic, float a_roughness, float a_ao)
{
	_shaderManager.UseShader("PBR");

	GLuint modelMatLocation = _shaderManager.GetUniform("PBR", "modelMat");
	glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, glm::value_ptr(a_modelMatrix));

	GLuint albedoLocation = _shaderManager.GetUniform("PBR", "albedo");
	GLuint metallicLocation = _shaderManager.GetUniform("PBR", "metallic");
	GLuint roughnessLocation = _shaderManager.GetUniform("PBR", "roughness");
	GLuint aoLocation = _shaderManager.GetUniform("PBR", "ao");

	glUniform3fv(albedoLocation, 1, glm::value_ptr(a_colour));

	if (!a_isLightSource)
	{
		glUniform1f(metallicLocation, a_metallic);
		glUniform1f(roughnessLocation, a_roughness);
		glUniform1f(aoLocation, a_ao);
	}
	else
	{
		// For light sources in PBR
		glUniform1f(metallicLocation, 1.0f);
		glUniform1f(roughnessLocation, 0.0f);
		glUniform1f(aoLocation, 1.0f);
	}

	_cubeModel.Draw();
}

void Scene::DrawCubePhong(const glm::mat4& a_modelMatrix, const glm::vec3& a_colour, 
						  bool a_isLightSource, const glm::vec3& a_diffuse, 
						  const glm::vec3& a_specular, float a_shininess, 
						  const glm::vec3& a_worldLightPosition, const glm::vec3& a_emissiveColour)
{
	_shaderManager.UseShader("Phong");

	GLuint modelMatLocation = _shaderManager.GetUniform("Phong", "modelMat");
	glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, glm::value_ptr(a_modelMatrix));

	GLuint diffuseLocation = _shaderManager.GetUniform("Phong", "diffuseColour");
	GLuint specularLocation = _shaderManager.GetUniform("Phong", "specularColour");
	GLuint shininessLocation = _shaderManager.GetUniform("Phong", "shininess");
	GLuint lightPositionLocation = _shaderManager.GetUniform("Phong", "lightPosition");
	GLuint emissiveLocation = _shaderManager.GetUniform("Phong", "emissiveColour");

	glUniform3fv(diffuseLocation, 1, glm::value_ptr(a_diffuse));
	glUniform3fv(specularLocation, 1, glm::value_ptr(a_specular));
	glUniform1f(shininessLocation, a_shininess);
	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(a_worldLightPosition));
	glUniform3fv(emissiveLocation, 1, glm::value_ptr(a_emissiveColour));

	if (a_isLightSource)
	{
		glUniform3fv(emissiveLocation, 1, glm::value_ptr(a_colour));
	}
	else
	{
		glUniform3f(emissiveLocation, 0.0f, 0.0f, 0.0f);
	}

	_cubeModel.Draw();
}



void Scene::Draw()
{
	_shaderManager.UseShader(_currentShader);

	// Fetch uniform locations from ShaderManager for the Phong shader
	GLuint viewMatLocation = _shaderManager.GetUniform(_currentShader, "viewMat");
	GLuint projMatLocation = _shaderManager.GetUniform(_currentShader, "projMat");
	GLuint lightPositionLocation = _shaderManager.GetUniform(_currentShader, "lightPosition");
	GLuint lightColourLocation = _shaderManager.GetUniform(_currentShader, "lightColor");
	GLuint cameraPositionLocation = _shaderManager.GetUniform(_currentShader, "camPos");

	// Set view and projection matrices
	glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, glm::value_ptr(_viewMatrix));
	glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, glm::value_ptr(_projMatrix));

	// Set light position and color for Phong shader
	glm::vec3 lightPosition = glm::vec3(_modelMatrixCube2 * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	glm::vec3 lightColour = glm::vec3(5.0f); // Bright white light
	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
	glUniform3fv(lightColourLocation, 1, glm::value_ptr(lightColour));

	// Set camera position for Phong shader
	glm::vec3 camPos = glm::vec3(glm::inverse(_viewMatrix)[3]);
	glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(camPos));
	

	/////////////////////////////////////// PBR ///////////////////////////////////////
	if (_currentShader == "PBR")
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePBR(_modelMatrixCube1, _cubeDiffuseColour, false, _metallic, _roughness, 0.5f);

		/* Draw Cube 2 PBR - Light Source */
		DrawCubePBR(_modelMatrixCube2, lightColour, true, 0.0f, 0.0f, 0.0f);

		/* Draw Cube 3 PBR - Floor */
		DrawCubePBR(_modelMatrixCube3, glm::vec3(0.3f, 0.3f, 1.0f), false, 0.1f, 0.8f, 0.5f);
	}
	/////////////////////////////////////// PHONG ///////////////////////////////////////
	else if (_currentShader == "Phong")
	{
		/* Draw Cube 1 PHONG - Standard Object */
		DrawCubePhong(_modelMatrixCube1, _cubeDiffuseColour, false, glm::vec3(0.8f, 0.1f, 0.1f), _cubeSpecularColour, _cubeShininess, lightPosition, glm::vec3(0.0f, 0.0f, 0.0f));

		/* Draw Cube 2 PHONG - Light Source */
		DrawCubePhong(_modelMatrixCube2, glm::vec3(1.0f, 1.0f, 1.0f), true, glm::vec3(0.0f, 0.0f, 0.0f), _cubeSpecularColour, _cubeShininess, lightPosition, glm::vec3(1.0f, 1.0f, 1.0f));

		/* Draw Cube 3 PHONG - Floor */
		DrawCubePhong(_modelMatrixCube3, glm::vec3(0.0f, 0.0f, 0.0f), false, glm::vec3(0.3f, 0.3f, 1.0f), _cubeSpecularColour, _cubeShininess, lightPosition, glm::vec3(0.0f, 0.0f, 0.0f));


	}

	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}




