
#include <fstream>
#include <string>

#include "Scene.h"

Scene::Scene()
{
	_cube1Angle = 0.0f;
	_cube2Angle = 0.0f;

	_cameraAngleX = 0.0f;
	_cameraAngleY = 0.0f;

	_cubeDiffuseColour = glm::vec3(1.0f, 0.0f, 0.0f);
	_cubeSpecularColour = glm::vec3(1.0f, 1.0f, 1.0f);
	_animateCentreCube = true;
	_animateLight = true;

	
	m_shaderManager.SwitchShader("PHONG");
	
	
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
	
	glUniformMatrix4fv(m_shaderManager.shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(a_modelMatrix));
	glUniform3fv(m_shaderManager.shaderEmissiveColLocation, 1, glm::value_ptr(a_emissiveColour));
	glUniform3fv(m_shaderManager.shaderDiffuseColLocation, 1, glm::value_ptr(a_diffuseColour));
	glUniform1f(m_shaderManager.shaderCubeShininessLocation, a_cubeShininess);

	if (a_bIsLightSource)
	{
		glUniform3fv(m_shaderManager.shaderSpecularColLocation, 1, glm::value_ptr(_cubeSpecularColour));
		glUniform4fv(m_shaderManager.shaderWsLightPosLocation, 1, glm::value_ptr(a_modelMatrix * glm::vec4(0, 0, 0, 1)));
		glUniform1f(m_shaderManager.shaderCubeShininessLocation, 0.1f);
	}
	_cubeModel.Draw();
}

void Scene::Draw()
{
		// Activate the shader program
	glUseProgram( m_shaderManager.shaderProgram );

		// We use the small cube's model matrix to transform the light position
		// Send view and projection matrices to OpenGL
		// SHARED BETWEEN PHONG AND PBR
	CameraPositions();

		if (m_shaderManager.currentShaderType == "PBR")
		{
			/////////////////////////// PBR /////////////////////////////////
			DrawCubePBR(_modelMatrixCube1, _cubeDiffuseColour, 0.7, 0.2, 0.5, false);
			DrawCubePBR(_modelMatrixCube2, glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.1f, 0.1f, true);
			DrawCubePBR(_modelMatrixCube3, glm::vec3(0.3f, 0.3f, 1.0f), 0.2f, 0.8f, 0.5f, false);
		}
		else if (m_shaderManager.currentShaderType == "PHONG")
		{
			/////////////////////////// PHONG /////////////////////////////////
			DrawCubePhong(_modelMatrixCube1, glm::vec3(0.0f, 0.0f, 0.0f), _cubeDiffuseColour, _cubeShininess, false);
			DrawCubePhong(_modelMatrixCube2, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,1.0f,1.0f), _cubeShininess, true);
			DrawCubePhong(_modelMatrixCube3, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), _cubeShininess, false);
		}

	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}



void Scene::CameraPositions()
{
	m_shaderManager.CameraShaderLocations();
	
	glUniformMatrix4fv(m_shaderManager.shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(_viewMatrix) );
	glUniformMatrix4fv(m_shaderManager.shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(_projMatrix) );
	
	glm::vec3 camPos = glm::vec3(glm::inverse(_viewMatrix)[3]);
	glUniform3fv(m_shaderManager.shaderCamPosLocation, 1, glm::value_ptr(camPos));
}

void Scene::DrawCubePBR(glm::mat4& a_modelMatrix, glm::vec3& a_albedo, float a_metallic, float a_roughness,float a_ao, bool a_bIsLightSource)
{
	
	glUniformMatrix4fv(m_shaderManager.shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(a_modelMatrix));
	glUniform3fv(m_shaderManager.shaderAlbedoLocation, 1, glm::value_ptr(a_albedo)); // Variable color
	glUniform1f(m_shaderManager.shaderMetallicLocation, a_metallic); // Adjust metallic property
	glUniform1f(m_shaderManager.shaderRoughnessLocation, a_roughness); // Adjust roughness
	glUniform1f(m_shaderManager.shaderAoLocation, a_ao);

	if (a_bIsLightSource)
	{
		glm::vec3 lightPosition = glm::vec3(a_modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glUniform3fv(m_shaderManager.shaderLightPosLocation, 1, glm::value_ptr(lightPosition));
		glm::vec3 lightColour = _cubeSpecularColour; // shed light colour
		glUniform3fv(m_shaderManager.shadeLightColLocation, 1, glm::value_ptr(lightColour));
	}
	
	_cubeModel.Draw();
}
