
#include "Cube.h"
#include "ShaderManager.h"

// The GLM library contains vector and matrix functions and classes for us to use
// They are designed to easily work with OpenGL!
#include <GLM/glm.hpp> // This is the main GLM header
#include <GLM/gtc/matrix_transform.hpp> // This one lets us use matrix transformations
#include <GLM/gtc/type_ptr.hpp> // This one gives us access to a utility function which makes sending data to OpenGL nice and easy



class Scene
{
public:

	Scene();
	~Scene();

	void Update( float deltaTs );
	void Draw();
	void Startup();
	
	// Getters and Setters for various scene object variables
	// These are very specific to the scene, your design should move them out of here
	void SetCubeDiffuseColour(glm::vec3 value) { m_cubeDiffuseColour = value; }
	glm::vec3 GetCubeDiffuseColour() const { return m_cubeDiffuseColour; }

	void SetCubeSpecularColour(glm::vec3 value) { m_cubeSpecularColour = value; }
	glm::vec3 GetCubeSpecularColour() const { return m_cubeSpecularColour; }

	void SetAnimateLight(bool value) { m_animateLight = value; }
	bool GetAnimateLight() const { return m_animateLight; }

	void SetLightAngle(float value) { m_cube2Angle = value; }
	float GetLightAngle() const { return m_cube2Angle; }

	void SetAnimateCube(bool value) { m_animateCentreCube = value; }
	bool GetAnimateCube() const { return m_animateCentreCube; }

	void SetCubeAngle(float value) { m_cube1Angle = value; }
	float GetCubeAngle() const { return m_cube1Angle; }

	void SetCubeShininess(float value) {m_cubeShininess = value;}
	float GetCubeShininess() const { return m_cubeShininess; }

	void SetCurrentShader(std::string value) { m_currentShader = value; }

	void SetMetallic(bool value) { m_metallic = value ? 1.0f : 0.0f; }
	float GetMetallic() const { return m_metallic;}

	void SetRoughness(float value) { m_roughness = value; }
	float GetRoughness() const { return m_roughness; }


protected:
	Cube m_cubeModel;

	// Model matrices for three cubes
	glm::mat4 m_modelMatrixCube1;
	glm::mat4 m_modelMatrixCube2;
	glm::mat4 m_modelMatrixCube3;

	// All cubes share the same viewing matrix - this defines the camera's orientation and position
	glm::mat4 m_viewMatrix;

	// The projection matrix defines the camera's view (like its lens)
	glm::mat4 m_projMatrix;


	// Angle of rotation for our cube
	float m_cube1Angle;
	float m_cube2Angle;
	float m_cameraAngleX, m_cameraAngleY;
	float m_cubeShininess;

	float m_metallic;
	float m_roughness;

	glm::vec3 m_cubeDiffuseColour;
	glm::vec3 m_cubeSpecularColour;

	// Bools for turning animations on and off 
	bool m_animateCentreCube, m_animateLight;

	ShaderManager m_shaderManager;

	void AnimationMatrices();

	void DrawCubePhong(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_diffuseColour,float a_cubeShininess);
	void DrawCubePBR(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_albedo, bool a_metallic,float a_roughness);

private:
	std::string m_currentShader;
};