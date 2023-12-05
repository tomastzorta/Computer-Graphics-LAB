
#include <fstream>
#include <string>

#include "Scene.h"

#include <iostream>
#include <GLM/gtc/matrix_transform.hpp>
#include "nvtx3/nvtx3.hpp"


Scene::Scene() : m_shaderAnalyser(m_shaderManager)
{
	m_currentShader = "Disney";
	m_isAnalyserActive = false;
	
	ShaderAnalyser::CompareShaders();
	
	nvtxRangePushA("Phong Load Shader");
	m_shaderManager.LoadShader("Phong", "Shaders/Phong/vertShader.vert", "Shaders/Phong/fragShader.frag");
	nvtxRangePop();
	nvtxRangePushA("PBR Load Shader");
	m_shaderManager.LoadShader("PBR", "Shaders/PBR/PBR_vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
	nvtxRangePop();
	nvtxRangePushA("Disney Load Shader");
	m_shaderManager.LoadShader("Disney", "Shaders/Disney_PBR/DPBR_vertShader.vert", "Shaders/Disney_PBR/DPBR_fragShader.frag");
	nvtxRangePop();
}

Scene::~Scene()
{
}


void Scene::Update( float deltaTs )
{
	m_animationManager.Update(deltaTs);
}

void Scene::CameraUniforms()
{
	// Camera and Projection Matrices
	m_shaderManager.UseShader(m_currentShader);
	m_shaderManager.SetUniform(m_currentShader, "viewMat", m_cameraManager.GetViewMatrix());
	m_shaderManager.SetUniform(m_currentShader, "projMat", m_cameraManager.GetProjMatrix());
	m_shaderManager.SetUniform(m_currentShader, "camPos", glm::vec3(glm::inverse(m_cameraManager.GetViewMatrix())[3]));
}

void Scene::DrawPhong()
{

	CameraUniforms();
	
	if (m_isAnalyserActive)
	{
		nvtxRangePushA("CPU Killer");
		for (int i = 0; i < 10000; i++)
		{
			
			glm::mat4 modelMatrix = glm::translate(m_animationManager.GetModelMatrixCube1(), glm::vec3(0.0f, 0.0f, -100.0f + i * 0.2f)); // Translate from -100 forward
			DrawCubePhong(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f), m_cubeModel.GetCubeColour(), m_cubeModel.GetCubeShininess());
		}
		nvtxRangePop();
	}
	else
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePhong(m_animationManager.GetModelMatrixCube1(), glm::vec3(0.0f, 0.0f, 0.0f), m_cubeModel.GetCubeColour(), m_cubeModel.GetCubeShininess());
	}
		
	/* Draw Cube 2 PBR - Light Source */
	DrawCubePhong(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,1.0f,1.0f), m_cubeModel.GetCubeShininess());
		
	/* Draw Cube 3 PBR - Floor */
	DrawCubePhong(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), m_cubeModel.GetCubeShininess());
	
    glUseProgram(0);
}

void Scene::DrawPBR()
{
	CameraUniforms();
	
	if (m_isAnalyserActive)
	{
		for (int i = 0; i < 10000; i++)
		{
			glm::mat4 modelMatrix = glm::translate(m_animationManager.GetModelMatrixCube1(),glm::vec3(0.0f, 0.0f, -100.0f + i * 0.2f));
			DrawCubePBR(modelMatrix, glm::vec3(0.05f, 0.05f, 0.05f), m_cubeModel.GetCubeColour(),m_cubeModel.GetCubeMetallic(), m_cubeModel.GetCubeRoughness());
		}
	}
	else
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePBR(m_animationManager.GetModelMatrixCube1(), glm::vec3(0.05f, 0.05f, 0.05f),m_cubeModel.GetCubeColour(), m_cubeModel.GetCubeMetallic(), m_cubeModel.GetCubeRoughness());
	}

	/* Draw Cube 2 PBR - Light Source */
	DrawCubePBR(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),false, 0.1f);

	/* Draw Cube 3 PBR - Floor */
	DrawCubePBR(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.3f, 0.3f, 1.0f),m_cubeModel.GetCubeMetallic(), m_cubeModel.GetCubeRoughness());

	glUseProgram(0);
}

void Scene::DrawDisney()
{
	CameraUniforms();
	
	if (m_isAnalyserActive)
	{
		for (int i = 0; i < 10000; i++)
		{
			glm::mat4 modelMatrix = glm::translate(m_animationManager.GetModelMatrixCube1(), glm::vec3(0.0f, 0.0f, -100.0f + i * 0.2f)); // Translate from -100 forward
			DrawCubeDisney(modelMatrix, glm::vec3(0.05f, 0.05f, 0.05f), m_cubeModel.GetCubeColour(), m_cubeModel.GetCubeMetallic(), m_cubeModel.GetCubeSubsurface(), m_cubeModel.GetCubeRoughness(), 0.8f, m_cubeModel.GetCubeSpecularTint(), m_cubeModel.GetCubeAnisotropic(), m_cubeModel.GetCubeSheen(), m_cubeModel.GetCubeSheenTint(), m_cubeModel.GetCubeClearcoat(), m_cubeModel.GetCubeClearcoatGloss());
		}
	}
	else
	{
		/* Draw Cube 1 Disney - Metallic Cube */
		DrawCubeDisney(m_animationManager.GetModelMatrixCube1(), glm::vec3(0.05f, 0.05f, 0.05f), m_cubeModel.GetCubeColour(), m_cubeModel.GetCubeMetallic(), m_cubeModel.GetCubeSubsurface(), m_cubeModel.GetCubeRoughness(), 0.8f, m_cubeModel.GetCubeSpecularTint(), m_cubeModel.GetCubeAnisotropic(), m_cubeModel.GetCubeSheen(), m_cubeModel.GetCubeSheenTint(), m_cubeModel.GetCubeClearcoat(), m_cubeModel.GetCubeClearcoatGloss());
	}
		
	/* Draw Cube 2 Disney - Light Source */
	DrawCubeDisney(m_animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), false, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		
	/* Draw Cube 3 Disney - Wooden Floor */
	DrawCubeDisney(m_animationManager.GetModelMatrixCube3(), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.2f, 0.1f), false, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(0);
}

void Scene::DrawCubePhong(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_diffuseColour,float a_cubeShininess )
{
	// Set uniforms to function parameters
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
	// Set uniforms to function parameters
	m_shaderManager.SetUniform("PBR", "modelMat", a_modelMatrix);
	m_shaderManager.SetUniform("PBR", "albedo", a_albedo);
	m_shaderManager.SetUniform("PBR", "metallic", a_metallic ? 1.0f : 0.0f);
	m_shaderManager.SetUniform("PBR", "roughness", a_roughness);
	m_shaderManager.SetUniform("PBR", "emissiveColour", a_emissiveColour);
	m_shaderManager.SetUniform("PBR", "lightPosition", glm::vec3(m_animationManager.GetModelMatrixCube2() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	m_shaderManager.SetUniform("PBR", "lightColour", m_cubeModel.GetLightColour());

	m_cubeModel.Draw();
}

void Scene::DrawCubeDisney(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_baseColour, bool a_metallic, float a_subsurface, float a_roughness, float a_specular, float a_specularTint, float a_anisotropic, float a_sheen, float a_sheenTint, float a_clearCoat, float a_clearcoatGloss)
{
	// Set uniforms to function parameters
	m_shaderManager.SetUniform("Disney", "modelMat", a_modelMatrix); //model matrix
	m_shaderManager.SetUniform("Disney", "baseColour", a_baseColour); //base colour
	m_shaderManager.SetUniform("Disney", "emissiveColour", a_emissiveColour); //emissive colour
	m_shaderManager.SetUniform("Disney", "metallic", a_metallic ? 1.0f : 0.0f); //bool metallic
	m_shaderManager.SetUniform("Disney", "subsurface", a_subsurface); //the effect of light entering an object  and scattering beneath its surface before exiting (wax, skin, marble)
	m_shaderManager.SetUniform("Disney", "specular", a_specular); // used to simulate surfaces that have a clear coat or layer of varnish (car paint, nail polish)
	m_shaderManager.SetUniform("Disney", "roughness", a_roughness); // used to simulate rough surfaces (wood, concrete, stone)
	m_shaderManager.SetUniform("Disney", "specularTint", a_specularTint); // this allows the specular colour to inherit some of the base colour, to simulate materials like coloured metals.
	m_shaderManager.SetUniform("Disney", "anisotropic", a_anisotropic); // used to simulate brushed metal (steel, aluminium, gold)
	m_shaderManager.SetUniform("Disney", "sheen", a_sheen); // sheen is used to simulate fabrics, where light tends to scatter in all directions to create soft highlight (silk, velvet, satin)
	m_shaderManager.SetUniform("Disney", "sheenTint", a_sheenTint); // this allows the sheen colour to inherit some of the base colour, to simulate materials like coloured fabrics.
	m_shaderManager.SetUniform("Disney", "clearCoat", a_clearCoat); // simulating a clear coat typically seen on car paint
	m_shaderManager.SetUniform("Disney", "clearCoatGloss", a_clearcoatGloss); // the glossiness of the clear coat layer, a high value results in a smooth, shiny clear coat where a low value results in a rough matte clear coat.
	m_shaderManager.SetUniform("Disney", "lightPosition", glm::vec3(m_animationManager.GetModelMatrixCube2() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))); //light position
	m_shaderManager.SetUniform("Disney", "lightColour", m_cubeModel.GetLightColour()); //light colour

	m_cubeModel.Draw();
}

void Scene::SetMetalCube() // Brushed Metal Cube Preset
{
	if (m_currentShader == "PBR")
	{
		m_cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f)); // Grey or silver color
		m_cubeModel.SetCubeMetallic(true); // is metallic
		m_cubeModel.SetCubeRoughness(0.1f); // Moderate to high
	}
	else if (m_currentShader == "Phong")
	{
		m_cubeModel.SetCubeAmbientColour(glm::vec3(0.2f, 0.2f, 0.2f));
		m_cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f)); // Grey or silver color
		m_cubeModel.SetCubeSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f)); // White
		m_cubeModel.SetCubeShininess(100.0f); // High
	}
	else if (m_currentShader == "Disney")
	{
		m_cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f)); // Grey or silver color
		m_cubeModel.SetCubeMetallic(true); // is metallic
		m_cubeModel.SetCubeSubsurface(0.0f); // Low
		m_cubeModel.SetCubeRoughness(0.1f); // Moderate to high
		m_cubeModel.SetCubeSpecular(1.0f); // High
		m_cubeModel.SetCubeSpecularTint(0.0f); // Low
		m_cubeModel.SetCubeAnisotropic(0.0f); // High
		m_cubeModel.SetCubeSheen(0.0f); // Low
		m_cubeModel.SetCubeSheenTint(0.0f); // Low
		m_cubeModel.SetCubeClearcoat(1.0f); // can also be 0.0
		m_cubeModel.SetCubeClearcoatGloss(1.0f); // can also be 0.0
	}
}

void Scene::SetPlasticCube() // Plastic Cube Preset
{
	if (m_currentShader == "PBR")
	{
		m_cubeModel.SetCubeColour(glm::vec3(1.0f, 0.0f, 0.0f)); // Red6
		m_cubeModel.SetCubeMetallic(false); // High
		m_cubeModel.SetCubeRoughness(0.5f); // Moderate to high
	}
	else if (m_currentShader == "Phong")
	{
		m_cubeModel.SetCubeAmbientColour(glm::vec3(0.1f, 0.1f, 0.1f));
		m_cubeModel.SetCubeColour(glm::vec3(1.0f, 0.0f, 0.0f)); // Red
		m_cubeModel.SetCubeSpecularColour(glm::vec3(0.5f, 0.5f, 0.5f));
		m_cubeModel.SetCubeShininess(30.0f);
	}
	else if (m_currentShader == "Disney")
	{
		m_cubeModel.SetCubeColour(glm::vec3(1.0f, 0.0f, 0.0f)); // Red
		m_cubeModel.SetCubeMetallic(false); // Non Metallic
		m_cubeModel.SetCubeSubsurface(0.0f); // Low
		m_cubeModel.SetCubeSpecular(0.5f); // Moderate
		m_cubeModel.SetCubeRoughness(0.5f); // Moderate to high
		m_cubeModel.SetCubeSpecularTint(0.1f); // Low
		m_cubeModel.SetCubeAnisotropic(0.0f); // Low
		m_cubeModel.SetCubeSheen(0.2f); // Low
		m_cubeModel.SetCubeSheenTint(0.0f); // Low
		m_cubeModel.SetCubeClearcoat(0.0f); // Low
		m_cubeModel.SetCubeClearcoatGloss(0.0f); // Low
	}
}

void Scene::SetVelvetFabricCube() // Velvet Fabric Cube Preset
{
	if (m_currentShader == "PBR")
	{
		m_cubeModel.SetCubeColour(glm::vec3(0.7f, 0.2f, 0.3f));
		m_cubeModel.SetCubeMetallic(false); // High
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

void Scene::DisneyBrushedCube()
{
	m_cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f)); // Grey or silver color
	m_cubeModel.SetCubeMetallic(true); // is metallic
	m_cubeModel.SetCubeSubsurface(0.0f); // Low
	m_cubeModel.SetCubeRoughness(0.4f); // Moderate to high
	m_cubeModel.SetCubeSpecular(1.0f); // High
	m_cubeModel.SetCubeSpecularTint(0.0f); // Low
	m_cubeModel.SetCubeAnisotropic(1.0f); // High
	m_cubeModel.SetCubeSheen(0.0f); // Low
	m_cubeModel.SetCubeSheenTint(0.0f); // Low
	m_cubeModel.SetCubeClearcoat(1.0f); // can also be 0.0
	m_cubeModel.SetCubeClearcoatGloss(1.0f); // can also be 0.0
}

void Scene::DisneyCopperCube()
{
	m_cubeModel.SetCubeColour(glm::vec3(0.95f, 0.64f, 0.54f)); //Polished Copper
	m_cubeModel.SetCubeMetallic(true); // is metallic
	m_cubeModel.SetCubeSubsurface(0.0f); // Low
	m_cubeModel.SetCubeRoughness(0.1f); // Low for a polished surface
	m_cubeModel.SetCubeSpecular(1.0f); // High
	m_cubeModel.SetCubeSpecularTint(0.9f); // High
	m_cubeModel.SetCubeAnisotropic(0.8f); // High for a polished metal
	m_cubeModel.SetCubeSheen(0.0f); // Low
	m_cubeModel.SetCubeSheenTint(0.0f); // Low
	m_cubeModel.SetCubeClearcoat(0.5f); // Moderate
	m_cubeModel.SetCubeClearcoatGloss(1.0f); // High
}






