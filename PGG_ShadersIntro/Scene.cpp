
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
	m_cubeDiffuseColour = glm::vec3(1.0f, 0.0f, 0.0f);
	m_cubeSpecularColour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_cubeShininess = 20.0f;
	m_roughness = 0.5f;
	m_animationManager.SetAnimateCube(true);
	m_animationManager.SetAnimateLight(true);
	
	m_shaderManager.LoadShader("PBR", "Shaders/PBR/PBR_vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
	m_shaderManager.LoadShader("Phong", "Shaders/Phong/vertShader.vert", "Shaders/Phong/fragShader.frag");
}


void Scene::Update( float deltaTs )
{
	m_animationManager.Update(deltaTs);
}

void Scene::Draw()
{
	m_shaderManager.UseShader(m_currentShader);
	m_shaderManager.SetUniform(m_currentShader, "viewMat", m_cameraManager.GetViewMatrix());
	m_shaderManager.SetUniform(m_currentShader, "projMat", m_cameraManager.GetProjMatrix());
	m_shaderManager.SetUniform(m_currentShader, "camPos", glm::vec3(glm::inverse(m_cameraManager.GetViewMatrix())[3]));

	if (m_currentShader == "PBR")
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePBR(m_animationManager.GetModelMatrixCube1(),glm::vec3(0.0f, 0.0f, 0.0f), m_cubeDiffuseColour, m_metallic, m_roughness);
		/* Draw Cube 2 PBR - Light Source */
		DrawCubePBR(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), false, 0.1f );
		/* Draw Cube 3 PBR - Floor */
		DrawCubePBR(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), m_metallic, m_roughness );
	}
	else if (m_currentShader == "Phong")
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePhong(m_animationManager.GetModelMatrixCube1(), glm::vec3(0.0f, 0.0f, 0.0f), m_cubeDiffuseColour, m_cubeShininess);
		/* Draw Cube 2 PBR - Light Source */
		DrawCubePhong(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,1.0f,1.0f), m_cubeShininess);
		/* Draw Cube 3 PBR - Floor */
		DrawCubePhong(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), m_cubeShininess);
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
	m_shaderManager.SetUniformVec4("Phong", "worldSpaceLightPos", m_animationManager.GetModelMatrixCube2() * glm::vec4(0, 0, 0, 1));

	m_cubeModel.Draw();
}



void Scene::DrawCubePBR(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_albedo, bool a_metallic,float a_roughness)
{
	m_shaderManager.SetUniform("PBR", "modelMat", a_modelMatrix);
	m_shaderManager.SetUniform("PBR", "albedo", a_albedo);
	m_shaderManager.SetUniform("PBR", "metallic", a_metallic ? 1.0f : 0.0f);
	m_shaderManager.SetUniform("PBR", "roughness", a_roughness);
	m_shaderManager.SetUniform("PBR", "emissiveColour", a_emissiveColour);
	m_shaderManager.SetUniform("PBR", "lightPosition", glm::vec3(m_animationManager.GetModelMatrixCube2() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	m_shaderManager.SetUniform("PBR", "lightColour", glm::vec3(1.0f, 1.0f, 1.0f));

	m_cubeModel.Draw();
}






