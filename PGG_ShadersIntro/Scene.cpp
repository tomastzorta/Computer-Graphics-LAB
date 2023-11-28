
#include <fstream>
#include <string>

#include "Scene.h"

#include <iostream>


Scene::Scene()
{
	m_currentShader = "Disney";

	m_shaderManager.LoadShader("PBR", "Shaders/PBR/PBR_vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
	m_shaderManager.LoadShader("Phong", "Shaders/Phong/vertShader.vert", "Shaders/Phong/fragShader.frag");
	m_shaderManager.LoadShader("Disney", "Shaders/PBR/PBR_vertShader.vert", "Shaders/Disney_PBR/DPBR_fragShader.frag");
}

Scene::~Scene()
{
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
		DrawCubePBR(m_animationManager.GetModelMatrixCube1(),glm::vec3(0.05f, 0.05f, 0.05f), m_cubeModel.GetCubeDiffuseColour(), m_cubeModel.GetCubeMetallic(), m_cubeModel.GetCubeRoughness());
		/* Draw Cube 2 PBR - Light Source */
		DrawCubePBR(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), false, 0.1f );
		/* Draw Cube 3 PBR - Floor */
		DrawCubePBR(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.3f, 0.3f, 1.0f), m_cubeModel.GetCubeMetallic(), m_cubeModel.GetCubeRoughness() );
	}
	else if (m_currentShader == "Phong")
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePhong(m_animationManager.GetModelMatrixCube1(), glm::vec3(0.0f, 0.0f, 0.0f), m_cubeModel.GetCubeDiffuseColour(), m_cubeModel.GetCubeShininess());
		/* Draw Cube 2 PBR - Light Source */
		DrawCubePhong(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,1.0f,1.0f), m_cubeModel.GetCubeShininess());
		/* Draw Cube 3 PBR - Floor */
		DrawCubePhong(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), m_cubeModel.GetCubeShininess());
	}
	else if (m_currentShader == "Disney")
	{
		/* Draw Cube 1 Disney - Metallic Cube */
		DrawCubeDisney(m_animationManager.GetModelMatrixCube1(), glm::vec3(0.8f, 0.8f, 0.8f), m_cubeModel.GetCubeMetallic(), 0.0f, 0.2f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.8f);

		/* Draw Cube 2 Disney - Light Source */
		DrawCubeDisney(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), false, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		/* Draw Cube 3 Disney - Wooden Floor */
		DrawCubeDisney(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.4f, 0.2f, 0.1f), false, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
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
	m_shaderManager.SetUniform("Phong", "specularColour", m_cubeModel.GetCubeSpecularColour());
	m_shaderManager.SetUniform("Phong", "worldSpaceLightPos", m_animationManager.GetModelMatrixCube2() * glm::vec4(0, 0, 0, 1));

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
	m_shaderManager.SetUniform("PBR", "lightColour", m_cubeModel.GetCubeSpecularColour());

	m_cubeModel.Draw();
}

void Scene::DrawCubeDisney(glm::mat4& a_modelMatrix, glm::vec3& a_baseColour, bool a_metallic, float a_subsurface, float a_roughness, float a_specular, float a_specularTint, float a_anisotropic, float a_sheen, float a_sheenTint, float a_clearcoat, float a_clearcoatGloss)
{
	m_shaderManager.SetUniform("Disney", "modelMat", a_modelMatrix);
	m_shaderManager.SetUniform("Disney", "baseColour", a_baseColour);
	m_shaderManager.SetUniform("Disney", "metallic", a_metallic ? 1.0f : 0.0f);
	m_shaderManager.SetUniform("Disney", "subsurface", a_subsurface);
	m_shaderManager.SetUniform("Disney", "specular", a_specular);
	m_shaderManager.SetUniform("Disney", "roughness", a_roughness);
	m_shaderManager.SetUniform("Disney", "specularTint", a_specularTint);
	m_shaderManager.SetUniform("Disney", "anisotropic", a_anisotropic);
	m_shaderManager.SetUniform("Disney", "sheen", a_sheen);
	m_shaderManager.SetUniform("Disney", "sheenTint", a_sheenTint);
	m_shaderManager.SetUniform("Disney", "clearcoat", a_clearcoat);
	m_shaderManager.SetUniform("Disney", "clearcoatGloss", a_clearcoatGloss);
	m_shaderManager.SetUniform("Disney", "lightPosition", glm::vec3(m_animationManager.GetModelMatrixCube2() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	m_shaderManager.SetUniform("Disney", "lightColour", m_cubeModel.GetCubeSpecularColour());

	m_cubeModel.Draw();
}






