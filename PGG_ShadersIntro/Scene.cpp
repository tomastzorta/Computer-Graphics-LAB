
#include <fstream>
#include <string>

#include "Scene.h"

#include <iostream>


Scene::Scene()
{
	m_currentShader = "Phong";
	
	Startup();
}

Scene::~Scene()
{
}

void Scene::Startup()
{
	m_cube1Angle = 0.0f;
	m_cube2Angle = 0.0f;

	m_cameraAngleX = 0.0f, m_cameraAngleY = 0.0f;
	
	m_cubeDiffuseColour = glm::vec3(1.0f, 0.0f, 0.0f);
	m_cubeSpecularColour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_cubeShininess = 1.0f;
	m_animateCentreCube = true;
	m_animateLight = true;
	
	m_roughness = 0.2f;
	
	//_modelMatrixCube1;
	m_modelMatrixCube2 = glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(1.0f,0.0f,0.0f)),glm::vec3(0.1f,0.1f,0.1f));
	m_modelMatrixCube3 = glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-1.0f,0.0f)),glm::vec3(2.0f,0.1f,2.0f));
	
	// Set up the viewing matrix
	// This represents the camera's orientation and position
	m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-3.5f) );
	

	// Set up a projection matrix
	m_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	m_shaderManager.LoadShader("PBR", "Shaders/PBR/PBR_vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
	m_shaderManager.LoadShader("Phong", "Shaders/Phong/vertShader.vert", "Shaders/Phong/fragShader.frag");

	
}


void Scene::Update( float deltaTs )
{
	if (m_animateCentreCube)
	{
		m_cube1Angle += deltaTs * 0.5f;
		// Limit the loop to 360 degrees
		// This will prevent numerical inaccuracies
		while( m_cube1Angle > (3.14159265358979323846 * 2.0) )
		{
			m_cube1Angle -= (3.14159265358979323846 * 2.0);
		}
	}

	if (m_animateLight)
	{
		m_cube2Angle += deltaTs * 2.0f;
		while (m_cube2Angle > (3.14159265358979323846 * 2.0))
		{
			m_cube2Angle -= (3.14159265358979323846 * 2.0);
		}
	}

	AnimationMatrices();
}

void Scene::AnimationMatrices()
{
	// Update the model matrix with the rotation of the object
	m_modelMatrixCube1 = glm::rotate( glm::mat4(1.0f), m_cube1Angle, glm::vec3(0,1,0) );
	m_modelMatrixCube2 = glm::rotate(glm::mat4(1.0f), m_cube2Angle, glm::vec3(0, 1, 0));
	m_modelMatrixCube2 = glm::translate(m_modelMatrixCube2, glm::vec3(1, 0, 0));
	m_modelMatrixCube2 = glm::scale(m_modelMatrixCube2, glm::vec3(0.1f, 0.1f, 0.1f));
}

void Scene::Draw()
{
	m_shaderManager.UseShader(m_currentShader);
	m_shaderManager.SetUniform(m_currentShader, "viewMat", m_viewMatrix);
	m_shaderManager.SetUniform(m_currentShader, "projMat", m_projMatrix);
	m_shaderManager.SetUniform(m_currentShader, "camPos", glm::vec3(glm::inverse(m_viewMatrix)[3]));

	if (m_currentShader == "PBR")
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePBR(m_modelMatrixCube1,glm::vec3(0.0f, 0.0f, 0.0f), m_cubeDiffuseColour, m_metallic, m_roughness);
		/* Draw Cube 2 PBR - Light Source */
		DrawCubePBR(m_modelMatrixCube2, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), false, 0.1f );
		/* Draw Cube 3 PBR - Floor */
		DrawCubePBR(m_modelMatrixCube3, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), m_metallic, m_roughness );
	}
	else if (m_currentShader == "Phong")
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePhong(m_modelMatrixCube1, glm::vec3(0.0f, 0.0f, 0.0f), m_cubeDiffuseColour, m_cubeShininess);
		/* Draw Cube 2 PBR - Light Source */
		DrawCubePhong(m_modelMatrixCube2, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,1.0f,1.0f), m_cubeShininess);
		/* Draw Cube 3 PBR - Floor */
		DrawCubePhong(m_modelMatrixCube3, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), m_cubeShininess);
	}
	
    glUseProgram(0);
}

void Scene::DrawCubePhong(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_diffuseColour,
	float a_cubeShininess)
{
	// Set uniforms
	m_shaderManager.SetUniform("Phong", "modelMat", a_modelMatrix);
	m_shaderManager.SetUniform("Phong", "diffuseColour", a_diffuseColour);
	m_shaderManager.SetUniform("Phong", "shininess", a_cubeShininess);
	m_shaderManager.SetUniform("Phong", "emissiveColour", a_emissiveColour);
	m_shaderManager.SetUniform("Phong", "specularColour", m_cubeSpecularColour);
	m_shaderManager.SetUniformVec4("Phong", "worldSpaceLightPos", m_modelMatrixCube2 * glm::vec4(0, 0, 0, 1));

	m_cubeModel.Draw();
}



void Scene::DrawCubePBR(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_albedo, bool a_metallic,float a_roughness)
{
	m_shaderManager.SetUniform("PBR", "modelMat", a_modelMatrix);
	m_shaderManager.SetUniform("PBR", "albedo", a_albedo);
	m_shaderManager.SetUniform("PBR", "metallic", a_metallic ? 1.0f : 0.0f);
	m_shaderManager.SetUniform("PBR", "roughness", a_roughness);
	m_shaderManager.SetUniform("PBR", "emissiveColour", a_emissiveColour);
	m_shaderManager.SetUniform("PBR", "lightPosition", glm::vec3(m_modelMatrixCube2 * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	m_shaderManager.SetUniform("PBR", "lightColour", glm::vec3(1.0f, 1.0f, 1.0f));

	m_cubeModel.Draw();
}






