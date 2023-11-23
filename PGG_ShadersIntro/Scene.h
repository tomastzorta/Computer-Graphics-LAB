
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
	void SetCubeDiffuseColour(glm::vec3 value) { _cubeDiffuseColour = value; }
	glm::vec3 GetCubeDiffuseColour() { return _cubeDiffuseColour; }

	void SetCubeSpecularColour(glm::vec3 value) { _cubeSpecularColour = value; }
	glm::vec3 GetCubeSpecularColour() { return _cubeSpecularColour; }

	void SetAnimateLight(bool value) { _animateLight = value; }
	bool GetAnimateLight() { return _animateLight; }

	void SetLightAngle(float value) { _cube2Angle = value; }
	float GetLightAngle() { return _cube2Angle; }

	void SetAnimateCube(bool value) { _animateCentreCube = value; }
	bool GetAnimateCube() { return _animateCentreCube; }

	void SetCubeAngle(float value) { _cube1Angle = value; }
	float GetCubeAngle() { return _cube1Angle; }

	void SetCubeShininess(float value) {_cubeShininess = value;}
	float GetCubeShininess() { return _cubeShininess; }

	void SetCurrentShader(std::string value) { _currentShader = value; }

	void SetMetallic(float value) { _metallic = value; }
	float GetMetallic() { return _metallic; }

	void SetRoughness(float value) { _roughness = value; }
	float GetRoughness() { return _roughness; }


protected:

	Cube _cubeModel;

	// Model matrices for three cubes
	glm::mat4 _modelMatrixCube1;
	glm::mat4 _modelMatrixCube2;
	glm::mat4 _modelMatrixCube3;
		
	// All cubes share the same viewing matrix - this defines the camera's orientation and position
	glm::mat4 _viewMatrix;
	
	// The projection matrix defines the camera's view (like its lens)
	glm::mat4 _projMatrix;


	// Angle of rotation for our cube
	float _cube1Angle;
	float _cube2Angle;
	float _cameraAngleX, _cameraAngleY;
	float _cubeShininess;

	float _metallic;
	float _roughness;
	
	glm::vec3 _cubeDiffuseColour;
	glm::vec3 _cubeSpecularColour;

	// Bools for turning animations on and off 
	bool _animateCentreCube, _animateLight;

	ShaderManager _shaderManager;

	void AnimationMatrices();
	
	void DrawCubePhong(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_diffuseColour, float a_cubeShininess);
	void DrawCubePBR(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_albedo, bool a_metallic, float a_roughness, bool a_bIsLightSource);
	void CheckOpenGLError();

private:
	std::string _currentShader;
};