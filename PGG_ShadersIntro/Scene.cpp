
#include <fstream>
#include <string>

#include "Scene.h"

#include <iostream>
#include <GLM/gtc/matrix_transform.hpp>
#include "nvtx3/nvtx3.hpp"


Scene::Scene() : shaderAnalyser(shaderManager)
{
	m_currentShader = "Disney"; // Set the current shader to Disney
	m_isAnalyserActive = false; // Set the analyser to false
	
	ShaderAnalyser::CompareShaders(); // Compare the shaders
	
	nvtxRangePushA("Phong Load Shader"); // Push the shader loading to the NVTX profiler
	shaderManager.LoadShader("Phong", "Shaders/Phong/vertShader.vert", "Shaders/Phong/fragShader.frag");
	nvtxRangePop();
	nvtxRangePushA("PBR Load Shader");
	shaderManager.LoadShader("PBR", "Shaders/PBR/PBR_vertShader.vert", "Shaders/PBR/PBR_fragShader.frag");
	nvtxRangePop();
	nvtxRangePushA("Disney Load Shader");
	shaderManager.LoadShader("Disney", "Shaders/Disney_PBR/DPBR_vertShader.vert", "Shaders/Disney_PBR/DPBR_fragShader.frag");
	nvtxRangePop();
}

Scene::~Scene()
{
}


void Scene::Update(const float a_deltaTs )
{
	// Update the animation manager
	animationManager.Update(a_deltaTs);
}

void Scene::CameraUniforms()
{
	// Camera and Projection Matrices
	shaderManager.UseShader(m_currentShader);
	shaderManager.SetUniform(m_currentShader, "viewMat", cameraManager.GetViewMatrix());
	shaderManager.SetUniform(m_currentShader, "projMat", cameraManager.GetProjMatrix());
	shaderManager.SetUniform(m_currentShader, "camPos", glm::vec3(glm::inverse(cameraManager.GetViewMatrix())[3]));
}

void Scene::DrawPhong()
{

	CameraUniforms();
	
	if (m_isAnalyserActive)
	{
		nvtxRangePushA("CPU Killer");
		for (int i = 0; i < 10000; i++) // Draw 10000 cubes
		{
			
			glm::mat4 modelMatrix = glm::translate(animationManager.GetModelMatrixCube1(), glm::vec3(0.0f, 0.0f, -100.0f + i * 0.2f)); // Translate from -100 forward
			DrawCubePhong(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f), cubeModel.GetCubeColour(), cubeModel.GetCubeShininess());
		}
		nvtxRangePop();
	}
	else
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePhong(animationManager.GetModelMatrixCube1(), glm::vec3(0.0f, 0.0f, 0.0f), cubeModel.GetCubeColour(), cubeModel.GetCubeShininess());
	}
		
	/* Draw Cube 2 PBR - Light Source */
	DrawCubePhong(animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,1.0f,1.0f), cubeModel.GetCubeShininess());
		
	/* Draw Cube 3 PBR - Floor */
	DrawCubePhong(animationManager.GetModelMatrixCube3(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), cubeModel.GetCubeShininess());
	
    glUseProgram(0);
}

void Scene::DrawPBR()
{
	CameraUniforms();
	
	if (m_isAnalyserActive)
	{
		for (int i = 0; i < 10000; i++)
		{
			glm::mat4 modelMatrix = glm::translate(animationManager.GetModelMatrixCube1(),glm::vec3(0.0f, 0.0f, -100.0f + i * 0.2f));
			DrawCubePBR(modelMatrix, glm::vec3(0.05f, 0.05f, 0.05f), cubeModel.GetCubeColour(),cubeModel.GetCubeMetallic(), cubeModel.GetCubeRoughness());
		}
	}
	else
	{
		/* Draw Cube 1 PBR - Standard Object */
		DrawCubePBR(animationManager.GetModelMatrixCube1(), glm::vec3(0.05f, 0.05f, 0.05f),cubeModel.GetCubeColour(), cubeModel.GetCubeMetallic(), cubeModel.GetCubeRoughness());
	}

	/* Draw Cube 2 PBR - Light Source */
	DrawCubePBR(animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),false, 0.1f);

	/* Draw Cube 3 PBR - Floor */
	DrawCubePBR(animationManager.GetModelMatrixCube3(), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.3f, 0.3f, 1.0f),cubeModel.GetCubeMetallic(), cubeModel.GetCubeRoughness());

	glUseProgram(0);
}

void Scene::DrawDisney()
{
	CameraUniforms();
	
	if (m_isAnalyserActive)
	{
		for (int i = 0; i < 10000; i++)
		{
			glm::mat4 modelMatrix = glm::translate(animationManager.GetModelMatrixCube1(), glm::vec3(0.0f, 0.0f, -100.0f + i * 0.2f)); // Translate from -100 forward
			DrawCubeDisney(modelMatrix, glm::vec3(0.05f, 0.05f, 0.05f), cubeModel.GetCubeColour(), cubeModel.GetCubeMetallic(), cubeModel.GetCubeSubsurface(), cubeModel.GetCubeRoughness(), 0.8f, cubeModel.GetCubeSpecularTint(), cubeModel.GetCubeAnisotropic(), cubeModel.GetCubeSheen(), cubeModel.GetCubeSheenTint(), cubeModel.GetCubeClearCoat(), cubeModel.GetCubeClearCoatGloss());
		}
	}
	else
	{
		/* Draw Cube 1 Disney - Metallic Cube */
		DrawCubeDisney(animationManager.GetModelMatrixCube1(), glm::vec3(0.05f, 0.05f, 0.05f), cubeModel.GetCubeColour(), cubeModel.GetCubeMetallic(), cubeModel.GetCubeSubsurface(), cubeModel.GetCubeRoughness(), 0.8f, cubeModel.GetCubeSpecularTint(), cubeModel.GetCubeAnisotropic(), cubeModel.GetCubeSheen(), cubeModel.GetCubeSheenTint(), cubeModel.GetCubeClearCoat(), cubeModel.GetCubeClearCoatGloss());
	}
		
	/* Draw Cube 2 Disney - Light Source */
	DrawCubeDisney(animationManager.GetModelMatrixCube2(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), false, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		
	/* Draw Cube 3 Disney - Wooden Floor */
	DrawCubeDisney(animationManager.GetModelMatrixCube3(), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.2f, 0.1f), false, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(0);
}

void Scene::DrawCubePhong(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_diffuseColour,float a_cubeShininess )
{
	// Set uniforms to function parameters
	shaderManager.SetUniform("Phong", "modelMat", a_modelMatrix); //model matrix
	shaderManager.SetUniform("Phong", "diffuseColour", a_diffuseColour); //diffuse colour
	shaderManager.SetUniform("Phong", "shininess", a_cubeShininess); //shininess
	shaderManager.SetUniform("Phong", "emissiveColour", a_emissiveColour); //emissive colour
	shaderManager.SetUniform("Phong", "specularColour", cubeModel.GetLightColour()); //specular colour
	shaderManager.SetUniform("Phong", "worldSpaceLightPos", animationManager.GetModelMatrixCube2() * glm::vec4(0, 0, 0, 1)); //light position
	shaderManager.SetUniform("Phong", "ambientColour", cubeModel.GetCubeAmbientColour()); //ambient colour

	cubeModel.Draw();
}

void Scene::DrawCubePBR(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_albedo, bool a_metallic,float a_roughness)
{
	// Set uniforms to function parameters
	shaderManager.SetUniform("PBR", "modelMat", a_modelMatrix); //model matrix
	shaderManager.SetUniform("PBR", "albedo", a_albedo); //albedo
	shaderManager.SetUniform("PBR", "metallic", a_metallic ? 1.0f : 0.0f); //bool metallic
	shaderManager.SetUniform("PBR", "roughness", a_roughness); //roughness
	shaderManager.SetUniform("PBR", "emissiveColour", a_emissiveColour); //emissive colour
	shaderManager.SetUniform("PBR", "lightPosition", glm::vec3(animationManager.GetModelMatrixCube2() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))); //light position
	shaderManager.SetUniform("PBR", "lightColour", cubeModel.GetLightColour()); //light colour

	cubeModel.Draw();
}

void Scene::DrawCubeDisney(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_baseColour, bool a_metallic, float a_subsurface, float a_roughness, float a_specular, float a_specularTint, float a_anisotropic, float a_sheen, float a_sheenTint, float a_clearCoat, float a_clearcoatGloss)
{
	// Set uniforms to function parameters
	shaderManager.SetUniform("Disney", "modelMat", a_modelMatrix); //model matrix
	shaderManager.SetUniform("Disney", "baseColour", a_baseColour); //base colour
	shaderManager.SetUniform("Disney", "emissiveColour", a_emissiveColour); //emissive colour
	shaderManager.SetUniform("Disney", "metallic", a_metallic ? 1.0f : 0.0f); //bool metallic
	shaderManager.SetUniform("Disney", "subsurface", a_subsurface); //the effect of light entering an object  and scattering beneath its surface before exiting (wax, skin, marble)
	shaderManager.SetUniform("Disney", "specular", a_specular); // used to simulate surfaces that have a clear coat or layer of varnish (car paint, nail polish)
	shaderManager.SetUniform("Disney", "roughness", a_roughness); // used to simulate rough surfaces (wood, concrete, stone)
	shaderManager.SetUniform("Disney", "specularTint", a_specularTint); // this allows the specular colour to inherit some of the base colour, to simulate materials like coloured metals.
	shaderManager.SetUniform("Disney", "anisotropic", a_anisotropic); // used to simulate brushed metal (steel, aluminium, gold)
	shaderManager.SetUniform("Disney", "sheen", a_sheen); // sheen is used to simulate fabrics, where light tends to scatter in all directions to create soft highlight (silk, velvet, satin)
	shaderManager.SetUniform("Disney", "sheenTint", a_sheenTint); // this allows the sheen colour to inherit some of the base colour, to simulate materials like coloured fabrics.
	shaderManager.SetUniform("Disney", "clearCoat", a_clearCoat); // simulating a clear coat typically seen on car paint
	shaderManager.SetUniform("Disney", "clearCoatGloss", a_clearcoatGloss); // the glossiness of the clear coat layer, a high value results in a smooth, shiny clear coat where a low value results in a rough matte clear coat.
	shaderManager.SetUniform("Disney", "lightPosition", glm::vec3(animationManager.GetModelMatrixCube2() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))); //light position
	shaderManager.SetUniform("Disney", "lightColour", cubeModel.GetLightColour()); //light colour

	cubeModel.Draw();
}

void Scene::SetMetalCube() // Brushed Metal Cube Preset
{
	if (m_currentShader == "PBR")
	{
		cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f)); // Grey or silver color
		cubeModel.SetCubeMetallic(true); // is metallic
		cubeModel.SetCubeRoughness(0.1f); // Moderate to high
	}
	else if (m_currentShader == "Phong")
	{
		cubeModel.SetCubeAmbientColour(glm::vec3(0.2f, 0.2f, 0.2f));
		cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f)); // Grey or silver color
		cubeModel.SetCubeSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f)); // White
		cubeModel.SetCubeShininess(100.0f); // High
	}
	else if (m_currentShader == "Disney")
	{
		cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f)); // Grey or silver color
		cubeModel.SetCubeMetallic(true); // is metallic
		cubeModel.SetCubeSubsurface(0.0f); // Low
		cubeModel.SetCubeRoughness(0.1f); // Moderate to high
		cubeModel.SetCubeSpecular(1.0f); // High
		cubeModel.SetCubeSpecularTint(0.0f); // Low
		cubeModel.SetCubeAnisotropic(0.0f); // High
		cubeModel.SetCubeSheen(0.0f); // Low
		cubeModel.SetCubeSheenTint(0.0f); // Low
		cubeModel.SetCubeClearCoat(1.0f); // can also be 0.0
		cubeModel.SetCubeClearCoatGloss(1.0f); // can also be 0.0
	}
}

void Scene::SetPlasticCube() // Plastic Cube Preset
{
	if (m_currentShader == "PBR")
	{
		cubeModel.SetCubeColour(glm::vec3(1.0f, 0.0f, 0.0f)); // Red6
		cubeModel.SetCubeMetallic(false); // High
		cubeModel.SetCubeRoughness(0.5f); // Moderate to high
	}
	else if (m_currentShader == "Phong")
	{
		cubeModel.SetCubeAmbientColour(glm::vec3(0.1f, 0.1f, 0.1f));
		cubeModel.SetCubeColour(glm::vec3(1.0f, 0.0f, 0.0f)); // Red
		cubeModel.SetCubeSpecularColour(glm::vec3(0.5f, 0.5f, 0.5f));
		cubeModel.SetCubeShininess(30.0f);
	}
	else if (m_currentShader == "Disney")
	{
		cubeModel.SetCubeColour(glm::vec3(1.0f, 0.0f, 0.0f)); // Red
		cubeModel.SetCubeMetallic(false); // Non Metallic
		cubeModel.SetCubeSubsurface(0.0f); // Low
		cubeModel.SetCubeSpecular(0.5f); // Moderate
		cubeModel.SetCubeRoughness(0.5f); // Moderate to high
		cubeModel.SetCubeSpecularTint(0.1f); // Low
		cubeModel.SetCubeAnisotropic(0.0f); // Low
		cubeModel.SetCubeSheen(0.2f); // Low
		cubeModel.SetCubeSheenTint(0.0f); // Low
		cubeModel.SetCubeClearCoat(0.0f); // Low
		cubeModel.SetCubeClearCoatGloss(0.0f); // Low
	}
}

void Scene::SetVelvetFabricCube() // Velvet Fabric Cube Preset
{
	if (m_currentShader == "PBR")
	{
		cubeModel.SetCubeColour(glm::vec3(0.7f, 0.2f, 0.3f));
		cubeModel.SetCubeMetallic(false); // High
		cubeModel.SetCubeRoughness(0.8f); // Moderate to high
	}
	else if (m_currentShader == "Phong")
	{
		cubeModel.SetCubeAmbientColour(glm::vec3(0.05f, 0.05f, 0.1f));
		cubeModel.SetCubeColour(glm::vec3(0.7f, 0.2f, 0.3f));
		cubeModel.SetCubeSpecularColour(glm::vec3(0.1f, 0.1f, 0.1f));
		cubeModel.SetCubeShininess(5.0f);
	}
	else if (m_currentShader == "Disney")
	{
		cubeModel.SetCubeColour(glm::vec3(0.7f, 0.2f, 0.3f)); // Red
		cubeModel.SetCubeMetallic(false); // Not Metallic
		cubeModel.SetCubeSubsurface(0.4f); //moderate
		cubeModel.SetCubeSpecular(0.2f); // low
		cubeModel.SetCubeRoughness(1.0f); //high
		cubeModel.SetCubeSpecularTint(0.1f); // moderate
		cubeModel.SetCubeAnisotropic(0.0f); // low
		cubeModel.SetCubeSheen(1.0f); // Low
		cubeModel.SetCubeSheenTint(1.0f); // Low
		cubeModel.SetCubeClearCoat(0.0f); // low
		cubeModel.SetCubeClearCoatGloss(0.0f); // low
	}
}

void Scene::DisneyBrushedCube()
{
	cubeModel.SetCubeColour(glm::vec3(0.8f, 0.8f, 0.8f)); // Grey or silver color
	cubeModel.SetCubeMetallic(true); // is metallic
	cubeModel.SetCubeSubsurface(0.0f); // Low
	cubeModel.SetCubeRoughness(0.4f); // Moderate to high
	cubeModel.SetCubeSpecular(1.0f); // High
	cubeModel.SetCubeSpecularTint(0.0f); // Low
	cubeModel.SetCubeAnisotropic(1.0f); // High
	cubeModel.SetCubeSheen(0.0f); // Low
	cubeModel.SetCubeSheenTint(0.0f); // Low
	cubeModel.SetCubeClearCoat(1.0f); // can also be 0.0
	cubeModel.SetCubeClearCoatGloss(1.0f); // can also be 0.0
}

void Scene::DisneyCopperCube()
{
	cubeModel.SetCubeColour(glm::vec3(0.95f, 0.64f, 0.54f)); //Polished Copper
	cubeModel.SetCubeMetallic(true); // is metallic
	cubeModel.SetCubeSubsurface(0.0f); // Low
	cubeModel.SetCubeRoughness(0.1f); // Low for a polished surface
	cubeModel.SetCubeSpecular(1.0f); // High
	cubeModel.SetCubeSpecularTint(0.9f); // High
	cubeModel.SetCubeAnisotropic(0.8f); // High for a polished metal
	cubeModel.SetCubeSheen(0.0f); // Low
	cubeModel.SetCubeSheenTint(0.0f); // Low
	cubeModel.SetCubeClearCoat(0.5f); // Moderate
	cubeModel.SetCubeClearCoatGloss(1.0f); // High
}






