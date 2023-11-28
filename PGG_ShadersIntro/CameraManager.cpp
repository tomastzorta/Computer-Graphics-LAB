#include "CameraManager.h"
#include <GLM/gtc/matrix_transform.hpp>

CameraManager::CameraManager()
{
    m_cameraAngleX = 0.0f;
    m_cameraAngleY = 0.0f;
    // Set up the viewing matrix (camera's orientation and position)
    m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-5.0f) );
    
    // Set up a projection matrix
    m_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
    
}

CameraManager::~CameraManager() {}
