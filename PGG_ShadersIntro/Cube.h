
#ifndef __CUBE_H__
#define __CUBE_H__

// This is the main SDL include file
#include <vector>
#include <GLM/detail/type_mat.hpp>
#include <GLM/detail/type_vec.hpp>
#include <GLM/detail/type_vec3.hpp>
#include "glew.h"

class Cube
{
public:
	Cube();
	~Cube();

	void Draw() const;

	void SetCubeColour(const glm::vec3& a_value) { m_cubeColour = a_value; }
	glm::vec3 GetCubeColour() const { return m_cubeColour; }

	void SetLightColour(const glm::vec3& a_value) { m_lightColour = a_value; }
	glm::vec3 GetLightColour() const { return m_lightColour; }

	void SetCubeShininess(const float a_value) {m_cubeShininess = a_value;}
	float GetCubeShininess() const { return m_cubeShininess; }

	void SetCubeMetallic(const bool a_value) { m_metallic = a_value ? 1.0f : 0.0f; }
	float GetCubeMetallic() const { return m_metallic;}

	void SetCubeRoughness(const float a_value) { m_roughness = a_value; }
	float GetCubeRoughness() const { return m_roughness; }

	void SetCubeSheen(const float a_value) { m_sheen = a_value; }
	float GetCubeSheen() const { return m_sheen; }

	void SetCubeSheenTint(const float a_value) { m_sheenTint = a_value; }
	float GetCubeSheenTint() const { return m_sheenTint; }

	void SetCubeSubsurface(const float a_value) { m_subsurface = a_value; }
	float GetCubeSubsurface() const { return m_subsurface; }

	void SetCubeSpecular(const float a_value) { m_specular = a_value; }
	float GetCubeSpecular() const { return m_specular; }

	void SetCubeSpecularTint(const float a_value) { m_specularTint = a_value; }
	float GetCubeSpecularTint() const { return m_specularTint; }

	void SetCubeAnisotropic(const float a_value) { m_anisotropic = a_value; }
	float GetCubeAnisotropic() const { return m_anisotropic; }

	void SetCubeClearCoat(const float a_value) { m_clearCoat = a_value; }
	float GetCubeClearCoat() const { return m_clearCoat; }

	void SetCubeClearCoatGloss(const float a_value) { m_clearCoatGloss = a_value; }
	float GetCubeClearCoatGloss() const { return m_clearCoatGloss; }

	void SetCubeAmbientColour(const glm::vec3& a_value) { m_cubeAmbientColour = a_value; }
	glm::vec3 GetCubeAmbientColour() const { return m_cubeAmbientColour; }

	void SetCubeSpecularColour(const glm::vec3& a_value) { m_specularColour = a_value; }
	glm::vec3 GetCubeSpecularColour() const { return m_specularColour; }

	int GetNumberOfVertices() const { return _numVertices; }


protected:
	GLuint _VAO;
	unsigned int _numVertices;
	
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
