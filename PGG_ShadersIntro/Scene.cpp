
#include <fstream>
#include <string>

#include "Scene.h"

#include <iostream>


Scene::Scene()
{
	_currentShader = "Phong";
	
	Startup();
}

Scene::~Scene()
{
}

void Scene::Startup()
{
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

	_shaderManager.LoadShader("PBR", "Shaders/PBR/PBR_vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
	_shaderManager.LoadShader("Phong", "Shaders/Phong/vertShader.vert", "Shaders/Phong/fragShader.frag");

	
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

void Scene::Draw()
{
	_shaderManager.UseShader(_currentShader);
	_shaderManager.SetUniform(_currentShader, "viewMat", _viewMatrix);
	_shaderManager.SetUniform(_currentShader, "projMat", _projMatrix);
	_shaderManager.SetUniform(_currentShader, "camPos", glm::vec3(glm::inverse(_viewMatrix)[3]));

	if (_currentShader == "PBR")
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePBR(_modelMatrixCube1,glm::vec3(0.0f, 0.0f, 0.0f), _cubeDiffuseColour, true, 0.2, false);
		/* Draw Cube 2 PBR - Light Source */
		DrawCubePBR(_modelMatrixCube2, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), false, 0.1f, true);
		/* Draw Cube 3 PBR - Floor */
		DrawCubePBR(_modelMatrixCube3, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), false, 0.8f, false);
	}
	else if (_currentShader == "Phong")
	{
		DrawCubePhong(_modelMatrixCube1, glm::vec3(0.0f, 0.0f, 0.0f), _cubeDiffuseColour, _cubeShininess);
		DrawCubePhong(_modelMatrixCube2, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,1.0f,1.0f), _cubeShininess);
		DrawCubePhong(_modelMatrixCube3, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), _cubeShininess);
	}
	
    glUseProgram(0);
}

void Scene::DrawCubePhong(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_diffuseColour,
	float a_cubeShininess)
{
	// Set uniforms
	_shaderManager.SetUniform("Phong", "modelMat", a_modelMatrix);
	_shaderManager.SetUniform("Phong", "diffuseColour", a_diffuseColour);
	_shaderManager.SetUniform("Phong", "shininess", a_cubeShininess);
	_shaderManager.SetUniform("Phong", "emissiveColour", a_emissiveColour);
	_shaderManager.SetUniform("Phong", "specularColour", _cubeSpecularColour);
	glm::vec4 lightPos = _modelMatrixCube2 * glm::vec4(0, 0, 0, 1);
	_shaderManager.SetUniformVec4("Phong", "worldSpaceLightPos", lightPos);

	_cubeModel.Draw();
}



void Scene::DrawCubePBR(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_albedo, bool a_metallic,float a_roughness, bool a_bIsLightSource)
{
	
	_shaderManager.SetUniform("PBR", "modelMat", a_modelMatrix);
	_shaderManager.SetUniform("PBR", "albedo", a_albedo);
	_shaderManager.SetUniform("PBR", "metallic", a_metallic);
	_shaderManager.SetUniform("PBR", "roughness", a_roughness);
	_shaderManager.SetUniform("PBR", "emissiveColour", a_emissiveColour);

	if (a_bIsLightSource)
	{
		_shaderManager.SetUniform("PBR", "lightPosition", glm::vec3(_modelMatrixCube2 * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		_shaderManager.SetUniform("PBR", "lightColour", glm::vec3(1.0f, 1.0f, 1.0f));
	}

	_cubeModel.Draw();
}






