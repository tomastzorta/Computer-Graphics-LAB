
#include <fstream>
#include <string>

#include "Scene.h"

#include <iostream>


Scene::Scene()
{
	m_currentShader = "Phong";

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
		DrawCubePBR(m_animationManager.GetModelMatrixCube1(),glm::vec3(0.05f, 0.05f, 0.05f), m_cubeModel.GetCubeColour(), m_cubeModel.GetCubeMetallic(), m_cubeModel.GetCubeRoughness());
		/* Draw Cube 2 PBR - Light Source */
		DrawCubePBR(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), false, 0.1f );
		/* Draw Cube 3 PBR - Floor */
		DrawCubePBR(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.3f, 0.3f, 1.0f), m_cubeModel.GetCubeMetallic(), m_cubeModel.GetCubeRoughness() );
	}
	else if (m_currentShader == "Phong")
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePhong(m_animationManager.GetModelMatrixCube1(), glm::vec3(0.0f, 0.0f, 0.0f), m_cubeModel.GetCubeColour(), m_cubeModel.GetCubeShininess());
		/* Draw Cube 2 PBR - Light Source */
		DrawCubePhong(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,1.0f,1.0f), m_cubeModel.GetCubeShininess());
		/* Draw Cube 3 PBR - Floor */
		DrawCubePhong(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), m_cubeModel.GetCubeShininess());
	}
	else if (m_currentShader == "Disney")
	{
		/* Draw Cube 1 Disney - Metallic Cube */
		DrawCubeDisney(m_animationManager.GetModelMatrixCube1(), glm::vec3(0.05f, 0.05f, 0.05f), m_cubeModel.GetCubeColour(), m_cubeModel.GetCubeMetallic(), m_cubeModel.GetCubeSubsurface(), m_cubeModel.GetCubeRoughness(), 0.8f, m_cubeModel.GetCubeSpecularTint(), m_cubeModel.GetCubeAnisotropic(), m_cubeModel.GetCubeSheen(), m_cubeModel.GetCubeSheenTint(), m_cubeModel.GetCubeClearcoat(), m_cubeModel.GetCubeClearcoatGloss());

		/* Draw Cube 2 Disney - Light Source */
		DrawCubeDisney(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), false, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

		/* Draw Cube 3 Disney - Wooden Floor */
		DrawCubeDisney(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.2f, 0.1f), false, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	}
	
    glUseProgram(0);
}

void Scene::DrawCubePhong(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_diffuseColour,
	float a_cubeShininess )
{
	// Set uniforms
	m_shaderManager.SetUniform("Phong", "modelMat", a_modelMatrix);
	m_shaderManager.SetUniform("Phong", "diffuseColour", a_diffuseColour);
	m_shaderManager.SetUniform("Phong", "shininess", a_cubeShininess);
	m_shaderManager.SetUniform("Phong", "emissiveColour", a_emissiveColour);
	m_shaderManager.SetUniform("Phong", "specularColour", m_cubeModel.GetLightColour());
	m_shaderManager.SetUniform("Phong", "worldSpaceLightPos", m_animationManager.GetModelMatrixCube2() * glm::vec4(0, 0, 0, 1));
	m_shaderManager.SetUniform("Phong", "ambientColour", m_cubeModel.GetCubeAmbientColour());

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
	m_shaderManager.SetUniform("PBR", "lightColour", m_cubeModel.GetLightColour());
	m_shaderManager.SetUniform("PBR", "ao", m_cubeModel.GetCubeAO());

	m_cubeModel.Draw();
}

void Scene::DrawCubeDisney(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_baseColour, bool a_metallic, float a_subsurface, float a_roughness, float a_specular, float a_specularTint, float a_anisotropic, float a_sheen, float a_sheenTint, float a_clearCoat, float a_clearcoatGloss)
{
	m_shaderManager.SetUniform("Disney", "modelMat", a_modelMatrix);
	m_shaderManager.SetUniform("Disney", "baseColour", a_baseColour);
	m_shaderManager.SetUniform("Disney", "emissiveColour", a_emissiveColour);
	m_shaderManager.SetUniform("Disney", "metallic", a_metallic ? 1.0f : 0.0f);
	m_shaderManager.SetUniform("Disney", "subsurface", a_subsurface);
	m_shaderManager.SetUniform("Disney", "specular", a_specular);
	m_shaderManager.SetUniform("Disney", "roughness", a_roughness);
	m_shaderManager.SetUniform("Disney", "specularTint", a_specularTint);
	m_shaderManager.SetUniform("Disney", "anisotropic", a_anisotropic);
	m_shaderManager.SetUniform("Disney", "sheen", a_sheen);
	m_shaderManager.SetUniform("Disney", "sheenTint", a_sheenTint);
	m_shaderManager.SetUniform("Disney", "clearCoat", a_clearCoat);
	m_shaderManager.SetUniform("Disney", "clearCoatGloss", a_clearcoatGloss);
	m_shaderManager.SetUniform("Disney", "lightPosition", glm::vec3(m_animationManager.GetModelMatrixCube2() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	m_shaderManager.SetUniform("Disney", "lightColour", m_cubeModel.GetLightColour());

	m_cubeModel.Draw();
}

void Scene::SetBrushedMetalCube()
{
	if (m_currentShader == "PBR")
	{
		m_cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f));
		m_cubeModel.SetCubeAO(1.0f);
		m_cubeModel.SetCubeMetallic(true); // High
		m_cubeModel.SetCubeRoughness(0.1f); // Moderate to high
	}
	else if (m_currentShader == "Phong")
	{
		m_cubeModel.SetCubeAmbientColour(glm::vec3(0.2f, 0.2f, 0.2f));
		m_cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f));
		m_cubeModel.SetCubeSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));
		m_cubeModel.SetCubeShininess(100.0f);
	}
	else if (m_currentShader == "Disney")
	{
		m_cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f)); // Grey or silver color
		m_cubeModel.SetCubeMetallic(true); // High
		m_cubeModel.SetCubeSubsurface(0.0f); // Low
		m_cubeModel.SetCubeRoughness(0.1f); // Moderate to high
		m_cubeModel.SetCubeSpecular(1.0f); // High
		m_cubeModel.SetCubeSpecularTint(0.0f); // Low
		m_cubeModel.SetCubeAnisotropic(1.0f); // High
		m_cubeModel.SetCubeSheen(0.0f); // Low
		m_cubeModel.SetCubeSheenTint(0.0f); // Low
		m_cubeModel.SetCubeClearcoat(1.0f); // can also be 0.0
		m_cubeModel.SetCubeClearcoatGloss(1.0f); // can also be 0.0
	}
}

void Scene::SetPlasticCube()
{
	if (m_currentShader == "PBR")
	{
		m_cubeModel.SetCubeColour(glm::vec3(1.0f, 0.0f, 0.0f));
		m_cubeModel.SetCubeMetallic(false); // High
		m_cubeModel.SetCubeAO(1.0f);
		m_cubeModel.SetCubeRoughness(0.5f); // Moderate to high
	}
	else if (m_currentShader == "Phong")
	{
		m_cubeModel.SetCubeAmbientColour(glm::vec3(0.1f, 0.1f, 0.1f));
		m_cubeModel.SetCubeColour(glm::vec3(1.0f, 0.0f, 0.0f));
		m_cubeModel.SetCubeSpecularColour(glm::vec3(0.5f, 0.5f, 0.5f));
		m_cubeModel.SetCubeShininess(30.0f);
	}
	else if (m_currentShader == "Disney")
	{
		m_cubeModel.SetCubeColour(glm::vec3(1.0f, 0.0f, 0.0f)); // Grey or silver color
		m_cubeModel.SetCubeMetallic(false); // Non Metallic
		m_cubeModel.SetCubeSubsurface(0.0f); // Low
		m_cubeModel.SetCubeSpecular(0.5f); // Moderate
		m_cubeModel.SetCubeRoughness(0.5f); // Moderate to high
		m_cubeModel.SetCubeSpecularTint(0.1f); // Low
		m_cubeModel.SetCubeAnisotropic(0.0f); // High
		m_cubeModel.SetCubeSheen(0.2f); // Low
		m_cubeModel.SetCubeSheenTint(0.0f); // Low
		m_cubeModel.SetCubeClearcoat(0.0f); // high
		m_cubeModel.SetCubeClearcoatGloss(0.0f); // high
	}
}

void Scene::SetVelvetFabricCube()
{
	if (m_currentShader == "PBR")
	{
		m_cubeModel.SetCubeColour(glm::vec3(0.7f, 0.2f, 0.3f));
		m_cubeModel.SetCubeMetallic(false); // High
		m_cubeModel.SetCubeAO(0.8f);
		m_cubeModel.SetCubeRoughness(0.8f); // Moderate to high
	}
	else if (m_currentShader == "Phong")
	{
		m_cubeModel.SetCubeAmbientColour(glm::vec3(0.05f, 0.05f, 0.1f));
		m_cubeModel.SetCubeColour(glm::vec3(0.7f, 0.2f, 0.3f));
		m_cubeModel.SetCubeSpecularColour(glm::vec3(0.1f, 0.1f, 0.1f));
		m_cubeModel.SetCubeShininess(5.0f);
	}
	else if (m_currentShader == "Disney")
	{
		m_cubeModel.SetCubeColour(glm::vec3(0.7f, 0.2f, 0.3f)); // Red
		m_cubeModel.SetCubeMetallic(false); // Not Metallic
		m_cubeModel.SetCubeSubsurface(0.4f); //moderate
		m_cubeModel.SetCubeSpecular(0.2f); // low
		m_cubeModel.SetCubeRoughness(1.0f); //high
		m_cubeModel.SetCubeSpecularTint(0.1f); // moderate
		m_cubeModel.SetCubeAnisotropic(0.0f); // low
		m_cubeModel.SetCubeSheen(1.0f); // Low
		m_cubeModel.SetCubeSheenTint(1.0f); // Low
		m_cubeModel.SetCubeClearcoat(0.0f); // low
		m_cubeModel.SetCubeClearcoatGloss(0.0f); // low
	}
}

void Scene::DisneyRubberCube()
{
	m_cubeModel.SetCubeColour(glm::vec3(0.05f, 0.05f, 0.05f));
	m_cubeModel.SetCubeMetallic(false);
	m_cubeModel.SetCubeSubsurface(0.0f); 
	m_cubeModel.SetCubeRoughness(0.7f); 
	m_cubeModel.SetCubeSpecular(0.5f); 
	m_cubeModel.SetCubeSpecularTint(0.0f); 
	m_cubeModel.SetCubeAnisotropic(0.0f); 
	m_cubeModel.SetCubeSheen(0.0f); 
	m_cubeModel.SetCubeSheenTint(0.0f); 
	m_cubeModel.SetCubeClearcoat(0.0f);
	m_cubeModel.SetCubeClearcoatGloss(0.0f);
}

void Scene::DisneyGlassCube()
{
	m_cubeModel.SetCubeColour(glm::vec3(0.7f, 0.9f, 1.0f)); //Light Blue Glass
	m_cubeModel.SetCubeMetallic(false); // Low
	m_cubeModel.SetCubeSubsurface(0.2f); // Low
	m_cubeModel.SetCubeRoughness(0.1f); // Moderate to high
	m_cubeModel.SetCubeSpecular(1.0f); // High
	m_cubeModel.SetCubeSpecularTint(0.2f); // Low
	m_cubeModel.SetCubeAnisotropic(0.0f); // High
	m_cubeModel.SetCubeSheen(0.0f); // Low
	m_cubeModel.SetCubeSheenTint(0.0f); // Low
	m_cubeModel.SetCubeClearcoat(1.0f); // can also be 0.0
	m_cubeModel.SetCubeClearcoatGloss(1.0f); // can also be 0.0
}

void Scene::DisneyCopperCube()
{
	m_cubeModel.SetCubeColour(glm::vec3(0.6f, 0.1f, 0.1f)); //Polished Copper
	m_cubeModel.SetCubeMetallic(true); // Low
	m_cubeModel.SetCubeSubsurface(0.0f); // Low
	m_cubeModel.SetCubeRoughness(0.4f); // Moderate to high
	m_cubeModel.SetCubeSpecular(1.0f); // High
	m_cubeModel.SetCubeSpecularTint(0.8f); // Low
	m_cubeModel.SetCubeAnisotropic(0.2f); // High
	m_cubeModel.SetCubeSheen(0.0f); // Low
	m_cubeModel.SetCubeSheenTint(0.0f); // Low
	m_cubeModel.SetCubeClearcoat(0.3f); // can also be 0.0
	m_cubeModel.SetCubeClearcoatGloss(1.0f); // can also be 0.0
}






