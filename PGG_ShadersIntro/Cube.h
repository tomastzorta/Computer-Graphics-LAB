
#ifndef __CUBE_H__
#define __CUBE_H__

// This is the main SDL include file
#include <GLM/detail/type_vec.hpp>
#include <GLM/detail/type_vec3.hpp>
#include "glew.h"

class Cube
{
public:
	Cube();
	~Cube();

	void Draw();

	void SetCubeColour(glm::vec3 value) { m_cubeColour = value; }
	glm::vec3 GetCubeColour() const { return m_cubeColour; }

	void SetLightColour(glm::vec3 value) { m_lightColour = value; }
	glm::vec3 GetLightColour() const { return m_lightColour; }

	void SetCubeShininess(float value) {m_cubeShininess = value;}
	float GetCubeShininess() const { return m_cubeShininess; }

	void SetCubeMetallic(bool value) { m_metallic = value ? 1.0f : 0.0f; }
	float GetCubeMetallic() const { return m_metallic;}

	void SetCubeRoughness(float value) { m_roughness = value; }
	float GetCubeRoughness() const { return m_roughness; }

	void SetCubeSheen(float value) { m_sheen = value; }
	float GetCubeSheen() const { return m_sheen; }

	void SetCubeSheenTint(float value) { m_sheenTint = value; }
	float GetCubeSheenTint() const { return m_sheenTint; }

	void SetCubeSubsurface(float value) { m_subsurface = value; }
	float GetCubeSubsurface() const { return m_subsurface; }

	void SetCubeSpecular(float value) { m_specular = value; }
	float GetCubeSpecular() const { return m_specular; }

	void SetCubeSpecularTint(float value) { m_specularTint = value; }
	float GetCubeSpecularTint() const { return m_specularTint; }

	void SetCubeAnisotropic(float value) { m_anisotropic = value; }
	float GetCubeAnisotropic() const { return m_anisotropic; }

	void SetCubeClearcoat(float value) { m_clearCoat = value; }
	float GetCubeClearcoat() const { return m_clearCoat; }

	void SetCubeClearcoatGloss(float value) { m_clearCoatGloss = value; }
	float GetCubeClearcoatGloss() const { return m_clearCoatGloss; }

	void SetCubeAmbientColour(glm::vec3 value) { m_cubeAmbientColour = value; }
	glm::vec3 GetCubeAmbientColour() const { return m_cubeAmbientColour; }

	void SetCubeAO(float value) { m_ao = value; }
	float GetCubeAO() const { return m_ao; }

	void SetCubeSpecularColour(glm::vec3 value) { m_specularColour = value; }
	glm::vec3 GetCubeSpecularColour() const { return m_specularColour; }


protected:
	GLuint _VAO;
	unsigned int _numVertices;

	float m_ao;
	
	float m_metallic;
	float m_subsurface;
	float m_specular;
	float m_roughness;
	float m_specularTint;
	float m_anisotropic;
	float m_sheen;
	float m_sheenTint;
	float m_clearCoat;
	float m_clearCoatGloss;
	float m_cubeShininess;
	
	

	glm::vec3 m_cubeColour;
	glm::vec3 m_cubeAmbientColour;
	glm::vec3 m_specularColour;
	glm::vec3 m_lightColour;
};

#endif
