
#include "Cube.h"
#include "ShaderManager.h"

// The GLM library contains vector and matrix functions and classes for us to use
// They are designed to easily work with OpenGL!
#include <GLM/glm.hpp> // This is the main GLM header
#include <GLM/gtc/type_ptr.hpp> // This one gives us access to a utility function which makes sending data to OpenGL nice and easy

#include "CameraManager.h"
#include "AnimationManager.h"


class Scene
{
public:

	Scene();
	~Scene();

	void Update( float deltaTs );
	void Draw();
	
	void SetCurrentShader(std::string value) { m_currentShader = value; }
	std::string GetCurrentShader() const { return m_currentShader; }

	Cube m_cubeModel;
	ShaderManager m_shaderManager;
	AnimationManager m_animationManager;
	CameraManager m_cameraManager;

	void SetBrushedMetalCube();
	void SetPlasticCube();

protected:
	void DrawCubePhong(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_diffuseColour,float a_cubeShininess);
	void DrawCubePBR(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_albedo, bool a_metallic,float a_roughness);
	void DrawCubeDisney(glm::mat4& a_modelMatrix, glm::vec3& a_emissiveColour, glm::vec3& a_baseColour, bool a_metallic, float a_subsurface, float a_roughness, float a_specular, float a_specularTint, float a_anisotropic, float a_sheen, float a_sheenTint, float a_clearCoat, float a_clearCoatGloss);

private:
	std::string m_currentShader;
};
