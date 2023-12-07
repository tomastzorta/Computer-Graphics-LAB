
#include "Cube.h"
#include "ShaderManager.h"

// The GLM library contains vector and matrix functions and classes for us to use
// They are designed to easily work with OpenGL!
#include <GLM/glm.hpp> // This is the main GLM header
#include <GLM/gtc/type_ptr.hpp> // This one gives us access to a utility function which makes sending data to OpenGL nice and easy

#include "CameraManager.h"
#include "AnimationManager.h"
#include "ShaderAnalyser.h"


class Scene
{
public:
	// Constructor/Destructor
	Scene();
	~Scene();

	// Functions
	void Update( float a_deltaTs );
	void CameraUniforms();
	void DrawPhong();
	void DrawPBR();
	void DrawDisney();

	// Getters/Setters
	void SetCurrentShader(const std::string& a_value) { m_currentShader = a_value; }
	std::string GetCurrentShader() const { return m_currentShader; }

	void SetIsAnalyserActive(const bool a_value) { m_isAnalyserActive = a_value; }
	bool GetIsAnalyserActive() const { return m_isAnalyserActive; }

	// Class Objects
	Cube cubeModel;
	ShaderManager shaderManager;
	AnimationManager animationManager;
	CameraManager cameraManager;
	ShaderAnalyser shaderAnalyser;

	// Preset Cube Functions
	void SetMetalCube();
	void SetPlasticCube();
	void SetVelvetFabricCube();
	void DisneyBrushedCube();
	void DisneyCopperCube();

protected:
	// Draw Functions
	void DrawCubePhong(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_diffuseColour,float a_cubeShininess);
	void DrawCubePBR(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_albedo, bool a_metallic,float a_roughness);
	void DrawCubeDisney(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_baseColour, bool a_metallic, float a_subsurface, float a_roughness, float a_specular, float a_specularTint, float a_anisotropic, float a_sheen, float a_sheenTint, float a_clearCoat, float a_clearCoatGloss);

private:
	// current shader
	std::string m_currentShader;
	bool m_isAnalyserActive;
};
