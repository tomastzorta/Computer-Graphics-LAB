
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
	_shaderManager.LoadShader("PBR", "Shaders/PBR/PBR_vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
	_shaderManager.LoadShader("Phong", "Shaders/Phong/vertShader.vert", "Shaders/Phong/fragShader.frag");
	
	_cube1Angle = 0.0f;
	_cube2Angle = 0.0f;

	_cameraAngleX = 0.0f, _cameraAngleY = 0.0f;
	
	_cubeDiffuseColour = glm::vec3(1.0f, 0.0f, 0.0f);
	_cubeSpecularColour = glm::vec3(1.0f, 1.0f, 1.0f);
	_cubeShininess = 1.0f;
	_animateCentreCube = true;
	_animateLight = true;

	_metallic = 0.7f;
	_roughness = 0.2f;
	
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

void Scene::DrawCubePhong(const glm::mat4& a_modelMatrix, 
						  const glm::vec3& a_diffuse, 
						  const glm::vec3& a_specular, 
						  float a_shininess, 
						  const glm::vec3& a_emissive, 
						  bool a_isLightSource)
{
	_shaderManager.UseShader("Phong");

	GLuint modelMatLocation = _shaderManager.GetUniform("Phong", "modelMat");
	GLuint diffuseLocation = _shaderManager.GetUniform("Phong", "diffuseColour");
	GLuint specularLocation = _shaderManager.GetUniform("Phong", "specularColour");
	GLuint shininessLocation = _shaderManager.GetUniform("Phong", "shininess");
	GLuint emissiveLocation = _shaderManager.GetUniform("Phong", "emissiveColour");

	glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, glm::value_ptr(a_modelMatrix));
	glUniform3fv(diffuseLocation, 1, glm::value_ptr(a_diffuse));
	glUniform3fv(specularLocation, 1, glm::value_ptr(a_specular));
	glUniform1f(shininessLocation, a_shininess);
	glUniform3fv(emissiveLocation, 1, glm::value_ptr(a_emissive));

	if (a_isLightSource)
	{
		glUniform3fv(emissiveLocation, 1, glm::value_ptr(a_emissive));
		glUniform1f(shininessLocation, a_shininess);
	}
	else
	{
		glUniform3f(emissiveLocation, 0.0f, 0.0f, 0.0f);
		glUniform1f(shininessLocation, 0.1f);
	}

	_cubeModel.Draw();
}



void Scene::Draw()
{
    glm::vec3 lightPosition = glm::vec3(_modelMatrixCube2 * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    /////////////////////////////////////// PBR ///////////////////////////////////////
    if (_currentShader == "PBR")
    {
        _shaderManager.UseShader("PBR");

        GLuint viewMatLocationPBR = _shaderManager.GetUniform("PBR", "viewMat");
        GLuint projMatLocationPBR = _shaderManager.GetUniform("PBR", "projMat");
        GLuint lightPositionLocationPBR = _shaderManager.GetUniform("PBR", "lightPosition");
        GLuint lightColourLocationPBR = _shaderManager.GetUniform("PBR", "lightColor");
        GLuint cameraPositionLocationPBR = _shaderManager.GetUniform("PBR", "camPos");

    	glm::vec3 camPos = glm::vec3(glm::inverse(_viewMatrix)[3]);
        glUniform3fv(cameraPositionLocationPBR, 1, glm::value_ptr(camPos));
        glm::vec3 lightColourPBR = glm::vec3(1.0f); // Adjust as needed
        glUniform3fv(lightColourLocationPBR, 1, glm::value_ptr(lightColourPBR));
        glUniform3fv(lightPositionLocationPBR, 1, glm::value_ptr(lightPosition));
        glUniformMatrix4fv(viewMatLocationPBR, 1, GL_FALSE, glm::value_ptr(_viewMatrix));
        glUniformMatrix4fv(projMatLocationPBR, 1, GL_FALSE, glm::value_ptr(_projMatrix));
    	
    	/* Draw Cube 1 PBR - Standard Object */
    	DrawCubePBR(_modelMatrixCube1, _cubeDiffuseColour, false, _metallic, _roughness, 0.5f);

    	/* Draw Cube 2 PBR - Light Source */
    	DrawCubePBR(_modelMatrixCube2, lightColourPBR, true, 0.0f, 0.0f, 0.0f);

    	/* Draw Cube 3 PBR - Floor */
    	DrawCubePBR(_modelMatrixCube3, glm::vec3(0.3f, 0.3f, 1.0f), false, 0.1f, 0.8f, 0.5f);
    }
    /////////////////////////////////////// PHONG ///////////////////////////////////////
    else if (_currentShader == "Phong")
    {
        _shaderManager.UseShader("Phong");

        GLuint viewMatLocationPhong = _shaderManager.GetUniform("Phong", "viewMat");
        GLuint projMatLocationPhong = _shaderManager.GetUniform("Phong", "projMat");
        GLuint lightPositionLocationPhong = _shaderManager.GetUniform("Phong", "worldSpaceLightPos");

        glUniform3fv(lightPositionLocationPhong, 1, glm::value_ptr(lightPosition));
        glUniformMatrix4fv(viewMatLocationPhong, 1, GL_FALSE, glm::value_ptr(_viewMatrix));
        glUniformMatrix4fv(projMatLocationPhong, 1, GL_FALSE, glm::value_ptr(_projMatrix));
    	
    	/* Draw Cube 1 PHONG - Standard Object */
    	DrawCubePhong(_modelMatrixCube1, _cubeDiffuseColour, glm::vec3(0.0f, 0.0f, 0.0f), 0.1f, glm::vec3(0.0f, 0.0f, 0.0f), false);

    	/* Draw Cube 2 PHONG - Light Source */
    	DrawCubePhong(_modelMatrixCube2, glm::vec3(0.f), _cubeSpecularColour, _cubeShininess, glm::vec3(1.0f, 1.0f, 1.0f), true);

    	/* Draw Cube 3 PHONG - Floor */
    	DrawCubePhong(_modelMatrixCube3, glm::vec3(0.3f, 0.3f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.1f, glm::vec3(0.0f, 0.0f, 0.0f), false);
    }

    glUseProgram(0);
}





