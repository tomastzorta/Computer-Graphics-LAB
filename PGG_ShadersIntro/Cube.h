
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

	void SetCubeDiffuseColour(glm::vec3 value) { m_cubeDiffuseColour = value; }
	glm::vec3 GetCubeDiffuseColour() const { return m_cubeDiffuseColour; }

	void SetCubeSpecularColour(glm::vec3 value) { m_cubeSpecularColour = value; }
	glm::vec3 GetCubeSpecularColour() const { return m_cubeSpecularColour; }

	void SetCubeShininess(float value) {m_cubeShininess = value;}
	float GetCubeShininess() const { return m_cubeShininess; }

	void SetCubeMetallic(bool value) { m_metallic = value ? 1.0f : 0.0f; }
	float GetCubeMetallic() const { return m_metallic;}

	void SetCubeRoughness(float value) { m_roughness = value; }
	float GetCubeRoughness() const { return m_roughness; }

	void SetCubeSheen(float value) { m_sheen = value; }
	float GetCubeSheen() const { return m_sheen; }


protected:
	GLuint _VAO;
	unsigned int _numVertices;

	float m_cubeShininess;
	float m_metallic;
	float m_roughness;
	float m_sheen;

	glm::vec3 m_cubeDiffuseColour;
	glm::vec3 m_cubeSpecularColour;
};

#endif
